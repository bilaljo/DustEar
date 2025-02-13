#pragma once

#undef C10_BUILD_SHARED_LIBS
#undef slots
#include <torch/script.h>

namespace dustear::processing {
    class AnomalyCheck {
        public:
        explicit AnomalyCheck(std::string&& model_path = "model.pt");

        double classify(std::vector<double>& pulse);

    private:
        static double sigmoid(const double& input) noexcept;

    private:
        torch::jit::script::Module module;

        bool lodaded = false;
    };
}
