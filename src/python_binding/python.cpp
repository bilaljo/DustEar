#include <numbers>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <format>

#include <string>

#include "processing/pipeline.h"
#include "processing/parameter_calculation/least_squares.h"

namespace py = pybind11;


namespace {
    using DampedPulseResidual = dustear::processing::parameter_calculation::lsq::functions::DampedPulseResidual;
    using DampedPulseResidualGradient = dustear::processing::parameter_calculation::lsq::functions::DampedPulseGradient;
    using LSQ = dustear::processing::parameter_calculation::LeastSquares<3, DampedPulseResidual>;

    class Algorithms {
    public:
        explicit Algorithms(const std::string& file_name = "config.json")
        : pipeline(dustear::processing::Pipeline(file_name)) {
        }

        void execute_offline(const std::string& file_name) {
            pipeline.execute_offline(file_name);
        }

        void load_config(const std::string& file_name) {
            pipeline.load_config(file_name);
        }

        [[nodiscard]] auto lsq_fit(const py::array_t<double, py::array::c_style>& pulse, const double amplitude) const {
            std::array x{amplitude, pipeline.get_frequency(), 0.0};
            const py::buffer_info buf1 = pulse.request();
            const auto pulse_ptr = static_cast<double*>(buf1.ptr);
            const auto data = std::vector(pulse_ptr, pulse_ptr + pipeline.get_dead_time());
            LSQ::model.set_target(std::make_shared<const std::vector<double>>(data));
            LSQ lsq{static_cast<int>(pipeline.get_dead_time())};
            lsq.fit(x);
            return py::array_t(x.size(), x.data());
        }

        [[nodiscard]] double average_cost(const py::array_t<double, py::array::c_style>& pulses,
                                          const double k_A, const double k_tau, const double k_f) const {
            namespace parameter_calculation = dustear::processing::parameter_calculation;
            std::array x{0.0, pipeline.get_frequency(), 0.0};
            LSQ lsq{static_cast<int>(pipeline.get_dead_time())};
            const py::buffer_info buf1 = pulses.request();
            if (pulses.ndim() != 2) {
                throw py::value_error(std::format("Array has invalide depth. It should be 2 but is {}", pulses.ndim()));
            }
            if (pulses.shape(1) != pipeline.get_dead_time()) {
                throw py::value_error(std::format("Segmentsize is {} instead of {}", pulses.shape(1), pipeline.get_dead_time()));
            }
            double loss = 0;
            for (int i = 0; i < pulses.size(); i += pipeline.get_dead_time()) {
                const auto data = static_cast<double*>(buf1.ptr);
                auto pulse = std::vector(data + i, data + i + pipeline.get_dead_time());
                auto [min, max] = std::ranges::minmax_element(pulse);
                x[0] = (*max - *min) / 2;  // Amplitude guess
                LSQ::model.set_target(std::make_shared<const std::vector<double>>(pulse));
                lsq.fit(x);
                loss += lsq.get_variance_estimation(x);
            }
            return loss / static_cast<double>(pulses.shape(0));
        }

        [[nodiscard]] std::string representation() const noexcept {
            return std::format("Algorithms(frequency={}, time_constant={}, frame_size={}, trigger_level={})",
                               pipeline.get_frequency(), pipeline.get_time_constant(), pipeline.get_dead_time(),
                               pipeline.get_trigger_level());
        }

        void set_file_prefix(std::string&& file_name) {
            pipeline.file_prefix = std::move(file_name);
        }

        [[nodiscard]] auto get_file_prefix() const noexcept {
            return pipeline.file_prefix;
        }

        void set_destination_folder(std::string&& destination_folder) {
            pipeline.destination_folder = std::move(destination_folder);
        }

        [[nodiscard]] auto get_destination_folder() const noexcept {
            return pipeline.destination_folder;
        }

    private:
        void init_cross_correlation() {
            pipeline.init_cross_correlation();
        }

    private:
        dustear::processing::Pipeline pipeline;
    };
}

PYBIND11_MODULE(dustear, m) {
    py::class_<Algorithms>(m, "Algorithms")
        .def(pybind11::init<const std::string&>(), py::arg("filename") = "config.json")
        .def("execute_offline", &Algorithms::execute_offline, py::arg("filename"))
        .def("load_config", &Algorithms::load_config, py::arg("filename") = "config.json")
        .def("lsq_fit", &Algorithms::lsq_fit, py::arg("data") ,py::arg("amplitude"))
        .def("average_cost", &Algorithms::average_cost, py::arg("pulses"), py::arg("k_A"),
             py::arg("k_tau"), py::arg("k_f"))
        .def_property("file_prefix", &Algorithms::get_file_prefix, &Algorithms::set_file_prefix)
        .def_property("destination_folder", &Algorithms::get_destination_folder, &Algorithms::set_destination_folder)
        .def("__repr__", &Algorithms::representation)
    ;
}
