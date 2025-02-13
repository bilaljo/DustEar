#pragma once

#include <torch/torch.h>

namespace dustear::processing {
    class CleanDirtyCheck {
        public:
        CleanDirtyCheck();

        double classify (std::vector<double>& pulse);

    private:
        static double sigmoid(const double& input) noexcept;

    private:
        torch::jit::script::Module module;

        std::string_view model_path = "model.jit";
    };
}
