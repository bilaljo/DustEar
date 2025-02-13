#ifndef DUSTEAR_PROCESSING_PIPELINE_H
#define DUSTEAR_PROCESSING_PIPELINE_H

#include <string>
#include <variant>
#include <fstream>

#include "nlohmann/json.hpp"

#include "processing/filter/sinc.h"
#include "processing/filter/butterworth.h"

#include "processing/detection/trigger_level.h"
#include "processing/detection/correlation.h"
#include "processing/parameter_calculation/least_squares.h"
#include "processing/parameter_calculation/lsq/functions.h"
//#include "processing/anomaly_check.h"



namespace dustear::processing {
    class Pipeline {
    public:
        enum class FilterType {
            Sinc,
            Butterworth,
            None,
        };

        enum class CalculationMethod {
            peek_to_peek,
            LSQ,
        };

        enum class DetectionMethod {
            trigger_level,
            crosscorrelation,
        };

        struct Config {
            FilterType filter{};
            DetectionMethod detection_method{};
            CalculationMethod calculation_method{};
        };

        static constexpr int PARAMETERS = 3;

        static constexpr std::size_t COVARIANCES = 2;

        using TriggerResult = std::vector<std::vector<double>>;

        using CorrelationResult = std::vector<std::pair<std::vector<double>, double>>;

        using LSQResult = std::tuple<std::array<double, COVARIANCES>, std::array<double, COVARIANCES>, int, double>;

        using DetectionType = std::variant<TriggerResult, CorrelationResult>;

        using ParameterType = std::variant<double, LSQResult>;

        using ResultType = std::variant<ParameterType, std::pair<double, ParameterType>>;

        using LSQ = parameter_calculation::LeastSquares<PARAMETERS, parameter_calculation::lsq::functions::DampedPulseResidual>;

    public:
        Pipeline() = default;

        explicit Pipeline(const std::string& config_path);

        void init_cross_correlation() noexcept;

        std::vector<ResultType> execute_online(std::vector<double>& data) noexcept;

        void execute_offline(const std::string& file_path) noexcept;

        void load_config(const std::string& config_path);

        [[nodiscard]] std::pair<std::size_t, std::size_t> get_bandwith() const noexcept;

        [[nodiscard]] std::size_t get_filter_order() const noexcept;

        [[nodiscard]] FilterType get_filter_type() const noexcept;

        [[nodiscard]] double get_trigger_level() const noexcept;

        [[nodiscard]] std::size_t get_dead_time() const noexcept;

        [[nodiscard]] CalculationMethod get_calculation_method() const noexcept;

        [[nodiscard]] DetectionMethod get_detection_method() const noexcept;

        void set_low_cut_off_frequency(std::size_t f0) noexcept;

        void set_high_cut_off_frequency(std::size_t f1) noexcept;

        void set_filter_order(int new_order) noexcept;

        void set_filter_type(FilterType filter) noexcept;

        void set_trigger_level(double threshold) noexcept;

        void set_dead_time(std::size_t frame_size) noexcept;

        void set_calculation_method(CalculationMethod calculation_method) noexcept;

        void set_detection_method(DetectionMethod detection_method) noexcept;

        void save_config() const noexcept;

        void load_fall_back() noexcept;

        [[nodiscard]] std::size_t get_buffer_size() const noexcept;

        void set_frequency(double frequency);

        void set_time_constant(double time_constant);

        [[nodiscard]] double get_frequency() const noexcept;

        [[nodiscard]] double get_time_constant() const noexcept;

        void reset() noexcept;

    private:
        std::vector<ResultType> execute(std::vector<double>& data) noexcept;

        void filter(std::vector<double>& data) noexcept;

        [[nodiscard]] DetectionType detection(const std::vector<double>& data) noexcept;

        [[nodiscard]] ParameterType parameter_estimation(const std::vector<double>& data) noexcept;

        void load_filter_config(const nlohmann::json& filter_config) noexcept;

        void load_detection_config(const nlohmann::json& detection_config) noexcept;

        void load_parameter_calculation_config(const nlohmann::json& calculation_config) noexcept;

        void load_measurement_settings(const nlohmann::json& calculation_config) noexcept;

        void load_general_settings(const nlohmann::json& general_config) noexcept;

        [[nodiscard]] LSQResult lsq_fit(double amplitude) noexcept;

        [[nodiscard]] nlohmann::json save_algorithm_config() const noexcept;

        [[nodiscard]] nlohmann::json save_measurement_config() const noexcept;

        [[nodiscard]] nlohmann::json save_general_config() const noexcept;

        void save_results(const std::vector<ResultType>& parameters, bool online) noexcept;

        void save_parameters() const noexcept;

        [[nodiscard]] std::array<double, COVARIANCES> get_covariance(const std::array<double, PARAMETERS>& x);

        void save_pulse(const std::vector<double>& pulse) const noexcept;

        void save_anomaly(double value) const noexcept;

        void create_headers(bool online = false) noexcept;

    private:
        filter::Sinc sinc{};

        //AnomalyCheck anomaly_check;

        filter::Butterworth butterworth{};

        detection::TriggerLevel trigger_level{};

        detection::CrossCorrelation<std::vector<double>> cross_correlation{};

        std::size_t buffer_size = 10000;

        std::size_t f0{50000};
        std::size_t f1{150000};

        int order = 2;

        std::string config_path = "vanilla.json";

        std::size_t frame_size{};

        double threshold{};

        std::vector<double> ref;

        double frequency{};

        double time_constant{};

        std::unique_ptr<std::ofstream> anomalies_file = nullptr;

        std::unique_ptr<std::ofstream> parameters_file = nullptr;

        Config config{};

        parameter_calculation::lsq::functions::DampedPulse damped_pulse{};

        parameter_calculation::lsq::functions::DampedPulseResidual damped_pulse_residual{};

        parameter_calculation::lsq::functions::DampedPulse damped_pulse_gradient{};

        LSQ lsq{1};

        std::size_t timer = 0;

        std::size_t seen_pulses = 0;


        double particles_per_second = 0;

        bool online = false;

    public:
        std::string destination_folder = ".";

        std::string file_prefix{};

        bool save_pulses = false;

        bool save_raw_data = false;

        bool init_headers = true;

        double flow_rate = 0;

        double calibration = 19542.89 / 2;
    };
}

#endif  // DUSTEAR_PROCESSING_PIPELINE_H
