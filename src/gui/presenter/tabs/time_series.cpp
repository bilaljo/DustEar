#include "gui/presenter/tabs/time_series.h"

#include "helper/logging.h"

#include "gui/model/event_bus.h"

gui::presenter::tabs::TimeSeries::TimeSeries(View& view) noexcept: view(view) {
    auto observer = [this](const std::any& data){
        update_line_chart(std::any_cast<std::unordered_map<std::string, std::pair<std::vector<double>, std::vector<double>>>>(data));
    };
    model::event_bus.register_subscriber(observer, "line_charts");
}

void gui::presenter::tabs::TimeSeries::update_line_chart(const std::unordered_map<std::string, std::pair<std::vector<double>, std::vector<double>>> &data) const noexcept {
    if (data.contains(view.get_name())) {
        const auto [x, y] = data.at(view.get_name());
        view.update_chart(x, y);
    }
}

void gui::presenter::tabs::TimeSeries::update_line_chart(const std::unordered_map<std::string, std::pair<Buffer, Buffer>> &data) const noexcept {
    if (data.contains(view.get_name())) {
        const auto& [x, y] = data.at(view.get_name());
        view.update_chart(x.data(), y.data());
    }
}
