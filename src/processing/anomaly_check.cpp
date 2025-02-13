#include "processing/anomaly_check.h"

#include <filesystem>
#include <helper/logging.h>

#undef C10_BUILD_SHARED_LIBS
#include <torch/script.h>

dustear::processing::AnomalyCheck::AnomalyCheck(std::string&& model_path) {
    if (!std::filesystem::exists(model_path)) {
        logging::error("Could not load neuronal network");
    } else {
        try {
            module = torch::jit::load(model_path, c10::DeviceType::CPU);
            module.eval();
            module = torch::jit::optimize_for_inference(module);
            lodaded = true;
        }
        catch (const c10::Error& e) {
            logging::error(e.what_without_backtrace());
        }
    }
}

double dustear::processing::AnomalyCheck::sigmoid(const double &input) noexcept {
    return 1.0 / (1.0 + std::exp(-input));
}

double dustear::processing::AnomalyCheck::classify(std::vector<double> &pulse) {
    if (!lodaded) {  [[unlikely]]
        return -1;
    }
    const auto torch_tensor = torch::from_blob(pulse.data(), pulse.size(), {torch::kDouble}).view({1, -1});
    const std::vector<c10::IValue> inputs{torch_tensor};
    try {
        auto res = module.forward(inputs);
        return sigmoid(res.toTensor().item().toDouble());
    } catch (c10::Error& e) {
        logging::error(e.what_without_backtrace());
        return -1;
    }
}
