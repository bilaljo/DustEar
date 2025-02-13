#ifndef GUI_MODEL_PROCESSING_MEASUREMENT_H
#define GUI_MODEL_PROCESSING_MEASUREMENT_H

#include "processing/pipeline.h"
#include "helper/histogram.h"
#include "gui/model/driver/daq.h"

#include <csv/csv.hpp>

namespace gui::model::measurement {
    class Processing {
    public:
        using Histogram = helper::Histogram<double>;

        using FilterType =  dustear::processing::Pipeline::FilterType;

        using CalculationMethod = dustear::processing::Pipeline::CalculationMethod;

        using DetectionMethod = dustear::processing::Pipeline::DetectionMethod;

    public:
        Processing();

        void offline(const std::string& file_path) const noexcept;

        void online(const driver::DAQ::ReceivedPackage &data) noexcept;

        static void create_chart(const std::string& file_name, const std::function<void(csv::CSVRow)>& callback);

        void create_histogram(const std::string& file_name);

        void create_line_charts(const std::string& file_name) const;

        void set_destination_folder(const std::string& folder) const noexcept;

        void load_config(const std::string& file_path) noexcept;

        void save_config() const noexcept;

        void set_init_headers() noexcept;

        void save_raw_data(const driver::DAQ::ReceivedPackage &data) const noexcept;

        void online_test(const std::string& file_path) noexcept;

    public:
        static constexpr std::size_t BUFFER_SIZE = 1000;

        static constexpr bool ONLINE_TEST = false;

    private:
        std::unique_ptr<dustear::processing::Pipeline> pipeline;

        Histogram histogram{0, 3000, 30};

        std::vector<double> buffer;

        std::size_t current_sample = 0;

        static constexpr double SAMPLE_FREQUENCY = 2e6f;

        std::vector<double> frequencies;

        std::vector<double> time;

        std::size_t t = 0;
    };
}

#endif  // GUI_MODEL_PROCESSING_MEASUREMENT_H
