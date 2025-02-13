#include "gui/presenter/tabs/distribution.h"

#include "helper/logging.h"

#include "gui/model/event_bus.h"

gui::presenter::tabs::Distribution::Distribution(View& view) noexcept: view(view) {
    auto observer = [this](const std::any& data){
        auto [min_max, histogram] = std::any_cast<std::pair<Model::Histogram::Range, Model::Histogram>>(data);
        update_histogram(min_max, histogram);
    };
    model::event_bus.register_subscriber(observer, "histogram");

}

void gui::presenter::tabs::Distribution::update_histogram(const Model::Histogram::Range& range, const Model::Histogram& histogram) const noexcept {
    view.draw(histogram.get_bins(), range);
}
