#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <ranges>

#include "processing/pipeline.h"
#include "processing/filter/sinc.h"
#include "helper/histogram.h"
#include "csv/csv.hpp"

/*

std::pair<std::vector<std::size_t>, std::vector<float>> test_counts(std::string_view file_path) {
    dustear::processing::Pipeline algorithm{"configs/vanilla.json"};
    algorithm.execute_offline(file_path.data());
    helper::Histogram<float> histogram{50};
    csv::CSVReader reader("parameters.csv");
    std::vector<float> results;
    int i = 0;
    for (csv::CSVRow& row : reader) {
        if (++i == 0) {
            continue;
        }
        results.push_back(row["Amplitude"].get<float>());
    }
        histogram.update(results);
    return std::make_pair(histogram.get_counts(), histogram.get_values());
}

std::vector<std::vector<float>> read_binary(const std::string& file_name, std::size_t segments, std::size_t block_size) {
    if (!std::filesystem::exists(file_name)) {
        std::cerr << file_name << " not found" << "\n";
        return {};
    }
    std::ifstream file{file_name, std::ios::binary};
    std::vector<std::vector<float>> data;
    for (std::size_t i = 0; i < segments; i++) {
        std::vector<float> segment;
        segment.resize(block_size);
        file.read(reinterpret_cast<char*>(segment.data()), block_size * sizeof(float));
        data.push_back(std::move(segment));
    }
    return data;
}

std::vector<float> read_results(std::string_view path) {
    // Based on https://stackoverflow.com/a/55977930/12585571
    std::vector<float> result;
    std::ifstream file_in(path.data());
    std::string line;
    while (std::getline(file_in, line)) {
        std::istringstream ss(line);
        float amplitude;
        ss >> amplitude;
        result.push_back(amplitude);
    }
    return result;
}

TEST_CASE("Test Sinc Filter against Python (Butonal)") {
    dustear::processing::filter::Sinc filter{{50000, 150000}};
    std::vector<std::vector<float>> filtered_references = read_binary("data/sinc.bin", 3, 2000000);
    std::vector<std::vector<float>> raw_signals = read_binary("data/butanol.bin", 3, 2000000);
    for (int i = 0; i < raw_signals.size(); i++) {
        auto raw_signal = Eigen::Map<Eigen::Matrix<float, -1, 1>>(raw_signals[i].data(), raw_signals[i].size());
        auto filtered_reference = Eigen::Map<Eigen::Matrix<float, -1, 1>>(filtered_references[i].data(), filtered_references[i].size());
        auto filtered_data = filter(raw_signal);
        for (int j = 0; j < filtered_data.size(); j++) {
            CHECK(filtered_data[j] == doctest::Approx(filtered_reference[j]).epsilon(0.0000001));
        }
    }
}

TEST_CASE("Test Trigger Algorithm with Butonal") {
    dustear::processing::filter::Sinc filter{{50000, 150000}};
    dustear::processing::detection::TriggerLevel trigger_level;
    std::ifstream file("configs/vanilla.json");
    nlohmann::json data = nlohmann::json::parse(file);
    std::vector<std::vector<float>> pulses_references = read_binary("data/pulses.bin", 1628, 600);
    std::vector<std::vector<float>> raw_signals = read_binary("data/butanol.bin", 3, 2000000);
    std::vector<Eigen::Matrix<float, -1, 1>> pulses;
    for (auto raw_signal: raw_signals) {
        auto raw_signal_vector = Eigen::Map<Eigen::Matrix<float, -1, 1>>(raw_signal.data(), raw_signal.size());
        auto filtered_data = filter(raw_signal_vector);
        auto pulse = trigger_level.detect_pulses(filtered_data);
        std::ranges::copy(pulse, std::back_inserter(pulses));
    }
    for (int i = 0; i < pulses.size(); i++) {
        auto pulses_reference = Eigen::Map<Eigen::Matrix<float, -1, 1>>(pulses_references[i].data(), pulses_references[i].size());
        for (int j = 0; j < pulses_reference.size(); j++) {
            CHECK(pulses[i][j] == doctest::Approx(pulses_reference[j]).epsilon(0.0000001));
        }
    }
}

TEST_CASE("Test LSQ Algorithm with Butonal") {
    dustear::processing::filter::Sinc filter{{50000, 150000}};
    dustear::processing::detection::TriggerLevel trigger_level;
    std::ifstream file("configs/vanilla.json");
    nlohmann::json data = nlohmann::json::parse(file);
    std::vector<std::vector<float>> pulses_references = read_binary("data/pulses.bin", 1628, 600);
    std::vector<std::vector<float>> raw_signals = read_binary("data/butanol.bin", 3, 2000000);
    std::vector<Eigen::Matrix<float, -1, 1>> pulses;
    for (auto raw_signal: raw_signals) {
        auto raw_signal_vector = Eigen::Map<Eigen::Matrix<float, -1, 1>>(raw_signal.data(), raw_signal.size());
        auto filtered_data = filter(raw_signal_vector);
        auto pulse = trigger_level.detect_pulses(filtered_data);
        std::ranges::copy(pulse, std::back_inserter(pulses));
    }
    for (int i = 0; i < pulses.size(); i++) {
        auto pulses_reference = Eigen::Map<Eigen::Matrix<float, -1, 1>>(pulses_references[i].data(), pulses_references[i].size());
        for (int j = 0; j < pulses_reference.size(); j++) {
            CHECK(pulses[i][j] == doctest::Approx(pulses_reference[j]).epsilon(0.0000001));
        }
    }
}
*/

/*
TEST_CASE("Test Trigger Algorithm with 10 um particles") {
    std::pair<std::array<std::size_t, 50>, std::array<float, 50>> res = test_counts("data/10um.bin");
    helper::Histogram<float, 50> histogram;
    std::vector<float> matlab_amplitudes = read_results("data/matlab_10um.txt");
    histogram.update(matlab_amplitudes);
    auto matlab_results = std::make_pair(histogram.get_counts(), histogram.get_values());
    CHECK(res.first == matlab_results.first);
    CHECK(res.second == matlab_results.second);
}
*/
