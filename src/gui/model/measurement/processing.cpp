#include "gui/model/measurement/processing.h"

#include <filesystem>
#include <variant>
#include <tuple>
#include <fstream>

#include "csv/csv.hpp"

#include "helper/logging.h"
#include "gui/model/event_bus.h"
#include "gui/model/driver/daq.h"


gui::model::measurement::Processing::Processing():
pipeline(new dustear::processing::Pipeline)
{
    event_bus.register_topic("histogram");
    event_bus.register_topic("frequencies_changed");
    auto update_low_frequency = [this](const std::any &f) {
        pipeline->set_low_cut_off_frequency(std::any_cast<const std::size_t>(f));
    };
    event_bus.register_subscriber(update_low_frequency, "low_frequency_update");
    auto update_high_frequency = [this](const std::any &f) {
        pipeline->set_high_cut_off_frequency(std::any_cast<const std::size_t>(f));
    };
    event_bus.register_subscriber(update_high_frequency, "high_frequency_update");
    auto update_filter_order = [this](const std::any &o) { pipeline->set_filter_order(std::any_cast<const int>(o)); };
    event_bus.register_subscriber(update_filter_order, "filter_order_update");
    auto update_filter_type = [this](const std::any &t) {
        pipeline->set_filter_type(std::any_cast<const FilterType>(t));
    };
    event_bus.register_subscriber(update_filter_type, "filter_type_update");
    auto update_trigger_level = [this](const std::any &t) {
        pipeline->set_trigger_level(std::any_cast<const double>(t));
    };
    event_bus.register_subscriber(update_trigger_level, "trigger_level_update");
    auto update_frame_size = [this](const std::any &t) { pipeline->set_dead_time(std::any_cast<const std::size_t>(t)); };
    event_bus.register_subscriber(update_frame_size, "dead_time_update");
    auto update_calculation_method = [this](const std::any &m) {
        pipeline->set_calculation_method(std::any_cast<CalculationMethod>(m));
    };
    event_bus.register_subscriber(update_calculation_method, "calculation_method_update");
    auto update_detection_method = [this](const std::any &m) {
        pipeline->set_detection_method(std::any_cast<DetectionMethod>(m));
    };
    event_bus.register_subscriber(update_detection_method, "detection_method_update");
    auto update_save_pulses = [this](const std::any &p) { pipeline->save_pulses = std::any_cast<bool>(p); };
    event_bus.register_subscriber(update_save_pulses, "save_pulses_update");
    auto update_save_raw_data = [this](const std::any &p) { pipeline->save_raw_data = std::any_cast<bool>(p); };
    event_bus.register_subscriber(update_save_raw_data, "save_raw_data_update");
    auto save_config_subscriber = [this]() { save_config(); };
    event_bus.register_signal_qualifier("save_settings");
    event_bus.register_signal(save_config_subscriber, "save_settings");
    event_bus.register_topic("daq_data");
    auto update_resonance_frequency = [this](const std::any &p) {
        pipeline->set_frequency(std::any_cast<double>(p));
    };
    event_bus.register_subscriber(update_resonance_frequency, "resonance_frequency_update");
    auto update_time_constant = [this](const std::any &p) {
        pipeline->set_time_constant(std::any_cast<double>(p));
    };
    event_bus.register_subscriber(update_time_constant, "time_constant_update");
    load_config("configs/vanilla.json");
}

void gui::model::measurement::Processing::load_config(const std::string& file_path) noexcept {
    pipeline->load_config(file_path);
    buffer.resize(pipeline->get_buffer_size());
    event_bus.publish("frequencies_changed", pipeline->get_bandwith());
    event_bus.publish("filter_order_changed", pipeline->get_filter_order());
    event_bus.publish("filter_type_changed", pipeline->get_filter_type());
    event_bus.publish("trigger_level_changed", pipeline->get_trigger_level());
    event_bus.publish("dead_time_changed", pipeline->get_dead_time());
    event_bus.publish("calculation_method_changed", pipeline->get_calculation_method());
    event_bus.publish("detection_method_changed", pipeline->get_detection_method());
    event_bus.publish("save_pulses_changed", pipeline->save_pulses);
    event_bus.publish("save_raw_data_changed", pipeline->save_raw_data);
    event_bus.publish("resonance_frequency_changed", pipeline->get_frequency());
    event_bus.publish("time_constant_changed", pipeline->get_time_constant());
    event_bus.publish("correlation_enabled", pipeline->get_detection_method() == DetectionMethod::crosscorrelation);
}

void gui::model::measurement::Processing::offline(const std::string& file_path) const noexcept {
    logging::info("Started processing of {}", std::filesystem::path{file_path}.filename().string());
    pipeline->execute_offline(file_path);
    logging::info("Finished processing of {}", std::filesystem::path{file_path}.filename().string());
}

void gui::model::measurement::Processing::create_chart(const std::string& file_name, const std::function<void(csv::CSVRow)>& callback) {
    if (!std::filesystem::exists(file_name)) {
        logging::error("Cannot open {} because it does not exist", file_name);
        return;
    }
    for (csv::CSVReader reader(file_name); const auto& row: reader) {
        try {
            callback(row);
        } catch(const std::runtime_error&) {
            logging::error("Cannot use {} because header are missing", file_name);
            return;
        }
    }
}

void gui::model::measurement::Processing::create_histogram(const std::string& file_name) {
    auto update_histogram = [this](const csv::CSVRow& row) {
        if (row["Amplitude"].is_float()) [[likely]] {
            histogram.update(row["Amplitude"].get<double>() * pipeline->calibration / pipeline->flow_rate);
        }
    };
    create_chart(file_name, update_histogram);
    event_bus.publish("histogram", std::pair{histogram.range(), histogram});
}

void gui::model::measurement::Processing::create_line_charts(const std::string& file_name) const {
    const csv::CSVReader reader(file_name);
    std::vector<double> x;
    std::vector<double> frequencies;
    std::vector<double> time_constants;
    int t;
    auto update_line_charts = [this, &x, &frequencies, &time_constants, &t](const csv::CSVRow& row) {
        if (row["Frequency"].is_float()) [[likely]] {
            frequencies.push_back(row["Frequency"].get<double>());
            x.push_back(t);
        }
        t++;
    };
    create_chart(file_name, update_line_charts);
    std::unordered_map<std::string, std::pair<std::vector<double>, std::vector<double>>> charts;
    charts["Frequency"] = {x, frequencies};
    event_bus.publish("line_charts", charts);
}

void gui::model::measurement::Processing::set_destination_folder(const std::string &folder) const noexcept {
    pipeline->destination_folder = folder;
}

void gui::model::measurement::Processing::save_config() const noexcept {
    pipeline->save_config();
}

void gui::model::measurement::Processing::save_raw_data(const driver::DAQ::ReceivedPackage &data) const noexcept {
    if (pipeline->save_raw_data) {
        std::ofstream file{pipeline->destination_folder + "/raw_data.bin", std::ios::binary | std::ios::app};
        const auto item_size = sizeof(data[0]);
        const auto array_size = static_cast<std::streamsize>(data.size() * item_size);
        file.write(reinterpret_cast<const char*>(data.data()), array_size);
    }
}

void gui::model::measurement::Processing::online_test(const std::string& file_path) noexcept {
    std::ifstream file{file_path, std::ios::in | std::ios::binary};
    file.seekg(0, std::ios::end);
    const std::size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    for (std::size_t i = 0; i < file_size ; i += sizeof(double)) {
        driver::DAQ::ReceivedPackage adc_data;
        for (int j = 0; j < hardware::driver::Network::PACKAGES; j++) {
            double data;
            file.read(std::bit_cast<char*>(&data), sizeof(std::uint16_t));
            //adc_data[j] = static_cast<std::int16_t>(data / driver::DAQ::ADC_VOLTAGE * driver::DAQ::ADC_RESOLUTION);
        }
        online(adc_data);
    }
}

void gui::model::measurement::Processing::online(const driver::DAQ::ReceivedPackage &data) noexcept {
    using ParameterType = dustear::processing::Pipeline::ParameterType;
    using LSQResult = dustear::processing::Pipeline::LSQResult;
    using std::get;
    static int counter = 0;
    for (const auto& sample: data) {
        buffer[current_sample] = driver::DAQ::bit_to_volt<double>(sample);
        current_sample++;
        if (current_sample == pipeline->get_buffer_size()) {
            current_sample = 0;
            auto parameters = pipeline->execute_online(buffer);
            if (pipeline->init_headers) {
                pipeline->init_headers = false;
                t = 0;
            }
            for (const auto& parameter: parameters) {
                if (pipeline->get_detection_method() == DetectionMethod::crosscorrelation) {
                    auto param = std::get<std::pair<double, ParameterType>>(parameter);
                    if (pipeline->get_calculation_method() == CalculationMethod::LSQ) {
                        const auto lsq_result = std::get<LSQResult>(param.second);
                        const auto results = get<0>(lsq_result);
                        histogram.update(results[0] * pipeline->calibration / pipeline->flow_rate);
                        frequencies.push_back(results[1]);
                        time.push_back(t);
                        t++;
                    } else {
                        const auto amplitude = std::get<double>(param);
                        histogram.update(amplitude * pipeline->calibration / pipeline->flow_rate);
                    }
                } else {
                    auto param = std::get<ParameterType>(parameter);
                    if (pipeline->get_calculation_method() == CalculationMethod::LSQ) {
                        const auto lsq_result = std::get<LSQResult>(param);
                        const auto results = get<0>(lsq_result);
                        histogram.update(results[0]);
                        frequencies.push_back(results[1]);
                        time.push_back(t);
                        t++;
                    } else {
                        const auto amplitude = std::get<double>(param);
                        histogram.update(amplitude * pipeline->calibration / pipeline->flow_rate);
                    }
                }
            }
            std::unordered_map<std::string, std::pair<std::vector<double>, std::vector<double>>> charts;
            charts["Frequency"] = {time, frequencies};
            counter++;
            if (counter == 100) {
                event_bus.publish("histogram", std::pair{histogram.range(), histogram});
                event_bus.publish("line_charts", charts);
                counter = 0;
            }
        }
    }
}

void gui::model::measurement::Processing::set_init_headers() noexcept {
    pipeline->init_headers = true;
    current_sample = 0;
    t = 0;
    frequencies.clear();
    time.clear();
}
