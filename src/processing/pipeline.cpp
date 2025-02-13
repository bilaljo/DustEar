#include "processing/pipeline.h"

#include <filesystem>
#include <fstream>
#include <processing/event_bus.h>

#include "helper/logging.h"
#include "nlohmann/json.hpp"

#include "processing/parameter_calculation/estimation.h"


using LSQ = dustear::processing::Pipeline::LSQ;
using ParameterType = dustear::processing::Pipeline::ParameterType;
using LSQResult = dustear::processing::Pipeline::LSQResult;
using DetectionType = dustear::processing::Pipeline::DetectionType;
using ResultType = dustear::processing::Pipeline::ResultType;

dustear::processing::Pipeline::Pipeline(const std::string &config_path) {
    load_config(config_path);
    event_bus.register_topic("particle_concentration");
}

void dustear::processing::Pipeline::init_cross_correlation() noexcept {
    ref.resize(frame_size);
    const std::array parameters{1.0, frequency, 0.0};
    damped_pulse.time_constant = time_constant;
    std::vector<std::vector<double>> references;
    for (std::size_t i = 0; i < frame_size; i++) {
        ref[i] = damped_pulse(parameters.data(), i);
    }
    cross_correlation.update_parameters(frame_size, std::move(ref));
}

void dustear::processing::Pipeline::load_config(const std::string &config_path) {
    if (!std::filesystem::exists(config_path)) {
        logging::error("Could not load {}", config_path);
        logging::warning("Fallback to default config which is Sinc-Filter -> Trigger Detection -> Peek to Peek");
        load_fall_back();
    } else {
        this->config_path = config_path;
        std::ifstream file(config_path);
        nlohmann::json data = nlohmann::json::parse(file);
        logging::debug("Algorithm config loaded");
        const parameter_calculation::lsq::functions::Scales scales = {
            .amplitude = data["Algorithm"]["Parameters"]["k_A"].get<double>(),
            .frequency = data["Algorithm"]["Parameters"]["k_f"].get<double>(),
            .time_constant = data["Algorithm"]["Parameters"]["k_tau"].get<double>(),
        };
        LSQ::model.damped_pulse.scales = scales;
        //LSQ::gradient.scales = scales;
        damped_pulse.scales = scales;
        set_time_constant(data["Algorithm"]["Parameters"]["Time Constant"].get<double>());
        set_frequency(data["Algorithm"]["Parameters"]["Resonance Frequency"].get<double>());
        load_filter_config(data["Algorithm"]["Filter"]);
        load_detection_config(data["Algorithm"]["Pulse Detection"]);
        load_parameter_calculation_config(data["Algorithm"]["Parameter Calculation"]);
        load_measurement_settings(data["Measurement"]);
        load_general_settings(data["General"]);
    }
    init_cross_correlation();
}

double dustear::processing::Pipeline::get_frequency() const noexcept {
    return frequency;
}

double dustear::processing::Pipeline::get_time_constant() const noexcept {
    return time_constant;
}

void dustear::processing::Pipeline::set_frequency(const double frequency) {
    this->frequency = frequency;
    init_cross_correlation();
}

void dustear::processing::Pipeline::set_time_constant(const double time_constant) {
    this->time_constant = time_constant;
    LSQ::model.damped_pulse.time_constant = time_constant;
    //LSQ::gradient.time_constant = time_constant;
    init_cross_correlation();
}

void dustear::processing::Pipeline::load_fall_back() noexcept {
    config = {};
    buffer_size = filter::Sinc::BUFFER_SIZE;  // Sinc needs a huge buffer for the FFT
    threshold = 0.01;
    trigger_level.threshold = 0.01;
    frame_size = 1000;
    trigger_level.set_frame_size(1000);
    f0 = 50000;
    f1 = 150000;
    sinc.update_bandwith({f0, f1});
}

nlohmann::json dustear::processing::Pipeline::save_algorithm_config() const noexcept {
    nlohmann::json algorithm_config;
    algorithm_config["Parameters"] = {
        // https://stackoverflow.com/a/57459521/12585571
        {"Time Constant", std::ceil(time_constant * 1e8) / 1e8},
        {"Resonance Frequency", std::ceil(frequency * 100.0) / 100.0},
        {"k_A", damped_pulse.scales.amplitude},
        {"k_tau", damped_pulse.scales.time_constant},
        {"k_f", damped_pulse.scales.frequency},
    };
    algorithm_config["Filter"] = {{"Sinc", config.filter == FilterType::Sinc},
                                  {"Butterworth", config.filter == FilterType::Butterworth}};
    algorithm_config["Pulse Detection"] = {{"Trigger Level", config.detection_method == DetectionMethod::trigger_level},
                                             {"Crosscorrelation", config.detection_method == DetectionMethod::crosscorrelation}};
    algorithm_config["Parameter Calculation"] = {
            {"Peek-to-Peek", config.calculation_method == CalculationMethod::peek_to_peek},
            {"LSQ", config.calculation_method == CalculationMethod::LSQ},
        };
    return algorithm_config;
}

nlohmann::json dustear::processing::Pipeline::save_measurement_config() const noexcept {
    nlohmann::json measurement_config;
    measurement_config["band_pass"] = {{"f0", f0}, {"f1", f1}, {"order", order}};
    measurement_config["trigger_level"] = threshold;
    measurement_config["frame_size"] = frame_size;
    measurement_config["flow_rate"] = flow_rate;
    measurement_config["calibration"] = calibration;
    return measurement_config;
}

nlohmann::json dustear::processing::Pipeline::save_general_config() const noexcept {
    nlohmann::json general_config{{"save_pulses", save_pulses},
                                  {"save_raw_data", save_raw_data}};
    return general_config;
}

void dustear::processing::Pipeline::save_config() const noexcept {
    nlohmann::json json_config;
    json_config["Algorithm"] = save_algorithm_config();
    json_config["Measurement"] = save_measurement_config();
    json_config["General"] = save_general_config();
    std::ofstream config_file{config_path};
    config_file << json_config.dump(4);
}

void dustear::processing::Pipeline::load_filter_config(const nlohmann::json& filter_config) noexcept {
    config.filter = {};
    if (filter_config["Sinc"].get<bool>()) {
        logging::debug("Filter set to Sinc Filter");
        config.filter = FilterType::Sinc;
        buffer_size = filter::Sinc::BUFFER_SIZE;  // Sinc needs a huge buffer for the FFT
    } else if (filter_config["Butterworth"].get<bool>()) {
        logging::debug("Filter set to Butterworh Filter");
        config.filter = FilterType::Butterworth;
        buffer_size = 10000;
    } else {
        logging::debug("No Filter will be used");
        config.filter = FilterType::None;
        buffer_size = detection::CrossCorrelation<std::vector<double>>::BUFFER_SIZE;
    }
}

void dustear::processing::Pipeline::load_detection_config(const nlohmann::json &detection_config) noexcept {
    if (detection_config["Trigger Level"].get<bool>()) {
        logging::debug("Pulse Detection set to Trigger Level");
        config.detection_method = DetectionMethod::trigger_level;
    } else if (detection_config["Crosscorrelation"].get<bool>()) {
        logging::debug("Pulse Detection set to Crosscorrelation");
        config.detection_method = DetectionMethod::crosscorrelation;
    } else {  [[unlikely]]
        logging::error("No pulse detection set or entry unknown");
        logging::warning("Will use trigger level as fallback");
        config.detection_method = DetectionMethod::trigger_level;
    }
}

void dustear::processing::Pipeline::load_parameter_calculation_config(const nlohmann::json &calculation_config) noexcept {
    if (calculation_config["Peek-to-Peek"].get<bool>()) {
        logging::debug("Parameter Calculation set to Peek-to-Peek");
        config.calculation_method = CalculationMethod::peek_to_peek;
    } else if (calculation_config["LSQ"].get<bool>()) {
        logging::debug("Parameter Calculation set to LSQ");
        config.calculation_method = CalculationMethod::LSQ;
    }
}

void dustear::processing::Pipeline::load_measurement_settings(const nlohmann::json &calculation_config) noexcept {
    set_trigger_level(calculation_config["trigger_level"].get<double>());
    set_dead_time(calculation_config["frame_size"].get<std::size_t>());
    f0 = calculation_config["band_pass"]["f0"].get<std::size_t>();
    f1 = calculation_config["band_pass"]["f1"].get<std::size_t>();
    sinc.update_bandwith({f0, f1});
    butterworth.set_cut_off_frequencies({f0, f1});
    order = calculation_config["band_pass"]["order"].get<int>();
    butterworth.set_order(order);
    flow_rate = calculation_config["flow_rate"].get<double>();
    calibration = calculation_config["calibration"].get<double>();
}

void dustear::processing::Pipeline::load_general_settings(const nlohmann::json& general_config) noexcept {
    save_pulses = general_config["save_pulses"].get<bool>();
    save_raw_data = general_config["save_raw_data"].get<bool>();
}

std::pair<std::size_t, std::size_t> dustear::processing::Pipeline::get_bandwith() const noexcept {
    return std::make_pair(f0, f1);
}

std::size_t dustear::processing::Pipeline::get_filter_order() const noexcept {
    return order;
}

void dustear::processing::Pipeline::execute_offline(const std::string& file_path) noexcept {
    online = false;
    create_headers(online);
    std::ifstream file{file_path, std::ios::in | std::ios::binary};
    alignas(32) std::vector<double> raw_data(buffer_size);
    file.seekg(0, std::ios::end);
    const std::size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<ResultType> parameters;
    for (std::size_t i = 0; i < file_size ; i += buffer_size * sizeof(double)) {
        if (i + buffer_size * sizeof(double) > file_size) {  [[unlikely]]
            // The last segment does fit in memory anymore
            break;
        }
        file.read(std::bit_cast<char*>(raw_data.data()),
                static_cast<std::streamsize>(buffer_size * sizeof(double)));
        auto res = execute(raw_data);
        std::ranges::copy(res, std::back_inserter(parameters));
    }
    save_results(parameters, false);
}

std::vector<ResultType> dustear::processing::Pipeline::execute_online(std::vector<double> &data) noexcept {
    online = true;
    auto parameters = execute(data);
    save_results(parameters, true);
    return parameters;
}

void dustear::processing::Pipeline::save_results(const std::vector<ResultType>& parameters, const bool online) noexcept {
    if (init_headers) {  [[unlikely]]  // Has to be done only once
        create_headers(online);
    }
    const auto now = std::chrono::system_clock::now();
    auto local_time = std::chrono::current_zone()->to_local(now);
    const auto time = std::format("{:%Y-%m-%d,%H:%M:%OS}", local_time);
    for (const auto& parameter: parameters) {
        if (online) {
            *parameters_file << time << ",";
        }
        if (config.detection_method == DetectionMethod::crosscorrelation) {
            if (config.calculation_method == CalculationMethod::LSQ) {
                auto param = std::get<std::pair<double, ParameterType>>(parameter);
                auto [probability, lsq_result] = param;
                auto [results, errors, status, standard_error] = std::get<LSQResult>(lsq_result);
                for (const auto& result: results) {
                    *parameters_file << result << ",";
                }
                *parameters_file << probability << "," << standard_error << "," << status << "\n";
            } else if (config.calculation_method == CalculationMethod::peek_to_peek) {
                auto param = std::get<std::pair<double, ParameterType>>(parameter);
                const auto [ prob, amplitude] = param;
                *parameters_file << std::get<double>(amplitude) << "," <<  prob << "\n";
            }
        } else {
            if (config.calculation_method == CalculationMethod::LSQ) {
                auto param = std::get<ParameterType>(parameter);
                auto [results, errors, status, standard_error] = std::get<LSQResult>(param);
                for (const auto& result: results) {
                    *parameters_file << result << ",";
                }
                *parameters_file << standard_error << "," << status << "\n";
            } else if (config.calculation_method == CalculationMethod::peek_to_peek) {
                auto param = std::get<ParameterType>(parameter);
                const auto amplitude = std::get<double>(param);
                *parameters_file << amplitude << "\n";
            }
        }
    }
    if (!online) {
        parameters_file->close();
    }
}

void dustear::processing::Pipeline::create_headers(const bool online) noexcept {
    if (online) {
        const auto now = std::chrono::system_clock::now();
        auto local_time = std::chrono::current_zone()->to_local(now);
        auto date = std::format("{:%Y_%m_%d_%H_%M_%OS}", local_time);
        parameters_file = std::make_unique<std::ofstream>(std::format("{}/{}_parameters.csv", destination_folder, date), std::ios::out);
        anomalies_file = std::make_unique<std::ofstream>(std::format("{}/{}_anomaly.csv", destination_folder, date), std::ios::out);
        *parameters_file << "Date" << "," << "Time" << ",";
        *anomalies_file << "Date" << "," << "Time" << "," << "Anomaly" << "\n" << "Y-M-D,H:M:S,%,\n";
    } else {
        parameters_file = std::make_unique<std::ofstream>(std::format("{}/{}parameters.csv", destination_folder, file_prefix), std::ios::out);
        anomalies_file = std::make_unique<std::ofstream>(std::format("{}/{}anomaly.csv", destination_folder, file_prefix), std::ios::out);
        *anomalies_file << "Anomaly\n%\n";
    }
    if (config.calculation_method == CalculationMethod::peek_to_peek) {
        if (config.detection_method == DetectionMethod::trigger_level) {
            if (online) {
                *parameters_file << "Amplitude\nY-M-D,H:M:S,V\n";
            } else {
                *parameters_file << "Amplitude\nV\n";
            }
        } else {
            if (online) {
                *parameters_file << "Amplitude,Probability\nY-D-M,H:M:S,V,1\n";
            } else {
                *parameters_file << "Amplitude,Probability\nV,1\n";
            }
        }
    } else {
        if (config.detection_method == DetectionMethod::crosscorrelation) {
            *parameters_file << "Amplitude,Frequency,Probability,Standard Error,Status\n";
            if (online) {
                *parameters_file << "Y-M-D,H:M:S,V,kHz,%,V,1\n";
            } else {
                *parameters_file << "V,kHz,%,V,1\n";
            }
        } else {
            if (online) {
                *parameters_file << "Amplitude\nY-D-M,H:M:S,V\n";
            } else {
                *parameters_file << "Amplitude\nV\n";
            }
        }
    }
}

void dustear::processing::Pipeline::save_pulse(const std::vector<double>& pulse) const noexcept {
    std::ofstream pulse_file{std::format("{}/pulses.bin", this->destination_folder),
                 std::ios::binary | std::ios::app};
    pulse_file.write(std::bit_cast<char *>(pulse.data()),
                     static_cast<std::streamsize>(sizeof(double) * pulse.size()));
}

void dustear::processing::Pipeline::save_anomaly(const double value) const noexcept {
    if (online) {
        auto now = std::chrono::system_clock::now();
        auto local_time = std::chrono::current_zone()->to_local(now);
        *anomalies_file << std::format("{:%Y-%m-%d,%H:%M:%OS}", local_time) << "," << value * 1e2 << "\n";
    } else {
        *anomalies_file << std::format("{}\n", value * 1e2);
    }
}

std::vector<ResultType> dustear::processing::Pipeline::execute(std::vector<double>& data) noexcept {
    filter(data);
    std::vector<ResultType> parameters;
    auto detection_result = detection(data);
    if (config.detection_method == DetectionMethod::trigger_level) {
        for (auto pulses = std::get<TriggerResult>(detection_result); auto& pulse: pulses) {
            if (this->save_pulses) {
                save_pulse(pulse);
            }
            parameters.emplace_back(parameter_estimation(pulse));
        }
    } else if (config.detection_method == DetectionMethod::crosscorrelation) {
        for (auto results = std::move(std::get<CorrelationResult>(detection_result));
             auto& [pulse, certainty]: results) {
            timer += buffer_size;
            seen_pulses++;
            if (this->save_pulses) {
                save_pulse(pulse);
            }
            // Roughly 1 s
            if (timer >= parameter_calculation::lsq::functions::SAMPLE_FREQUENCY) {
                //const auto anomaly = anomaly_check.classify(pulse);
                //save_anomaly(anomaly);
                particles_per_second = seen_pulses / flow_rate;
                timer = 0;
                seen_pulses = 0;
                if (online) {
                    event_bus.publish("particle_concentration", particles_per_second);
                    //event_bus.publish("anomaly_probability", anomaly);
                }
            }
            ResultType res{std::pair(certainty, parameter_estimation(pulse))};
            parameters.push_back(res);
        }
    }
    return parameters;
}

void dustear::processing::Pipeline::filter(std::vector<double>& data) noexcept {
    switch (config.filter) {
        case FilterType::Sinc:
            sinc(data);
            break;
        case FilterType::Butterworth:
            butterworth.filter(data);
            break;
        default:
            break;
    }
}

DetectionType dustear::processing::Pipeline::detection(const std::vector<double>& data) noexcept {
    switch (config.detection_method) {
        case DetectionMethod::trigger_level:
            return trigger_level.detect_pulses(data);
        case DetectionMethod::crosscorrelation:
            return cross_correlation.detect_pulses(data);
        default:
            return {};
    }
}

ParameterType dustear::processing::Pipeline::parameter_estimation(const std::vector<double>& data) noexcept {
    double amplitude = parameter_calculation::esimtate_amplitude(data);
    switch (config.calculation_method) {
        case CalculationMethod::peek_to_peek:
            return amplitude;
        default:
            LSQ::model.set_target(std::make_shared<const std::vector<double>>(data));
            return lsq_fit(amplitude);
    }
}

std::array<double, dustear::processing::Pipeline::COVARIANCES> dustear::processing::Pipeline::get_covariance(const std::array<double, PARAMETERS>& x) {
    lsq.calculate_covariance(x);
    auto covariance = lsq.get_covariance();
    return {std::sqrtf(covariance(0, 0)), std::sqrtf(covariance(1, 1))};
}

LSQResult dustear::processing::Pipeline::lsq_fit(const double amplitude) noexcept {
    std::array x{amplitude, frequency, 0.0};
    const int status = lsq.fit(x);
    std::array parameters{x[0], x[1]};
    std::array<double, COVARIANCES> errors{};
    //auto errors = get_covariance(x);
    if (std::isnan(parameters[0]) || std::isnan(parameters[1])) {
        std::ranges::fill(parameters, -1);
    }
    /*for (const auto error: errors) {
        if (std::isnan(error)) {
            std::ranges::fill(errors, -1);
            break;
        }
    }*/
    return LSQResult{parameters, errors, status, lsq.get_variance_estimation(x)};
}

void dustear::processing::Pipeline::set_low_cut_off_frequency(const std::size_t f0) noexcept {
    if (f0 >= f1) {
        return;
    }
    this->f0 = f0;
    sinc.update_bandwith({f0, f1});
    butterworth.set_cut_off_frequencies({f0, f1});
}

void dustear::processing::Pipeline::set_high_cut_off_frequency(const std::size_t f1) noexcept {
    if (f1 <= f0) {
        return;
    }
    this->f1 = f1;
    sinc.update_bandwith({f0, f1});
    butterworth.set_cut_off_frequencies({f0, f1});
}

void dustear::processing::Pipeline::set_filter_order(const int new_order) noexcept {
    order = new_order;
    butterworth.set_order(new_order);
}

void dustear::processing::Pipeline::set_filter_type(const FilterType filter) noexcept {
    config.filter = filter;
}

dustear::processing::Pipeline::FilterType dustear::processing::Pipeline::get_filter_type() const noexcept {
    return config.filter;
}

double dustear::processing::Pipeline::get_trigger_level() const noexcept {
    return trigger_level.threshold;
}

std::size_t dustear::processing::Pipeline::get_dead_time() const noexcept {
    return frame_size;
}

void dustear::processing::Pipeline::set_trigger_level(const double threshold) noexcept {
    trigger_level.threshold = threshold;
    cross_correlation.trigger_level = threshold;
    this->threshold = threshold;
}

void dustear::processing::Pipeline::set_dead_time(const std::size_t frame_size) noexcept {
    this->frame_size = frame_size;
    trigger_level.set_frame_size(this->frame_size);
    lsq.set_samples(static_cast<int>(frame_size));
    ref.resize(frame_size);
    init_cross_correlation();
}

dustear::processing::Pipeline::CalculationMethod dustear::processing::Pipeline::get_calculation_method() const noexcept {
    return config.calculation_method;
}

dustear::processing::Pipeline::DetectionMethod dustear::processing::Pipeline::get_detection_method() const noexcept {
    return config.detection_method;
}

void dustear::processing::Pipeline::set_calculation_method(const CalculationMethod calculation_method) noexcept {
    config.calculation_method = calculation_method;
}

void dustear::processing::Pipeline::set_detection_method(const DetectionMethod detection_method) noexcept {
    config.detection_method = detection_method;
    buffer_size = detection::CrossCorrelation<std::vector<double>>::BUFFER_SIZE;
}

std::size_t dustear::processing::Pipeline::get_buffer_size() const noexcept {
    return buffer_size;
}

void dustear::processing::Pipeline::reset() noexcept {
    init_headers = true;
    timer = 0;
    seen_pulses = 0;
}
