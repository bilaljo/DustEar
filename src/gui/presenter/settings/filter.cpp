#include "gui/presenter/settings/filter.h"
#include "gui/model/event_bus.h"
#include "gui/model/measurement/processing.h"
#include "helper/logging.h"

#include <format>
#include <regex>

gui::presenter::settings::Filter::Filter(ViewType& view): view(view) {
    auto update_frequency = [this](const std::any& frequencies){
        update_frequencies_text(std::any_cast<const Frequencies&>(frequencies));
    };
    model::event_bus.register_subscriber(update_frequency, "frequencies_changed");
    auto update_filter_order = [this](const std::any& order){
        update_filter_order_view(std::any_cast<const std::size_t>(order));
    };
    model::event_bus.register_subscriber(update_filter_order, "filter_order_changed");
    model::event_bus.register_subscriber(update_frequency, "frequencies_changed");
    auto update_filter_type = [this](const std::any& t){
        update_filter_type_viw(std::any_cast<const model::measurement::Processing::FilterType>(t));
    };
    model::event_bus.register_subscriber(update_filter_type, "filter_type_changed");
    model::event_bus.register_topic("frequencies_changed");
    model::event_bus.register_topic("filter_order_changed");
    model::event_bus.register_topic("low_frequency_update");
    model::event_bus.register_topic("high_frequency_update");
    model::event_bus.register_topic("filter_order_update");
    model::event_bus.register_topic("filter_type_update");
    view.set_filter_types({"Sinc", "Butterworth", "None"});
    view.set_filter_orders({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
}

void gui::presenter::settings::Filter::update_frequencies_text(const Frequencies& frequencies) const noexcept {
    view.set_low_frequency(std::format("{}", frequencies.first));
    view.set_high_frequency(std::format("{}", frequencies.second));
}

void gui::presenter::settings::Filter::update_filter_order_view(const int order) const noexcept {
    view.set_filter_order(order);
}

void gui::presenter::settings::Filter::update_low_frequency(const std::string& frequency) noexcept {
    if (std::regex_match(frequency, std::regex("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"))) [[likely]] {
        if (const auto f = static_cast<std::size_t>(std::stoi(frequency)); f > 0) {
        model::event_bus.publish("low_frequency_update", f);
        } else {
            logging::error("Frequency must be positive number");
        }
    } else {
        logging::error("No number given");
    }
}

void gui::presenter::settings::Filter::update_high_frequency(const std::string& frequency) noexcept {
    if (std::regex_match(frequency, std::regex("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"))) [[likely]] {
        if (const auto f = static_cast<std::size_t>(std::stoi(frequency)); f > 0) {
            model::event_bus.publish("high_frequency_update", f);
        } else {
            logging::error("Frequency must be positive number");
        }
        } else {
        logging::error("No number given");
    }
}

void gui::presenter::settings::Filter::update_filter_order(const std::string &order) noexcept {
    if (order.empty()) {
        logging::error("No number given");
    } else {
        model::event_bus.publish("filter_order_update", std::stoi(order));
    }
}

void gui::presenter::settings::Filter::update_filter_type(const std::string &type) const noexcept {
    if (type == "Sinc") {
        model::event_bus.publish("filter_type_update", model::measurement::Processing::FilterType::Sinc);
        view.enable_filter_order(false);
        view.enable_frequencies(true);
    } else if (type == "Butterworth") {
        model::event_bus.publish("filter_type_update", model::measurement::Processing::FilterType::Butterworth);
        view.enable_filter_order(true);
        view.enable_frequencies(true);
    } else if (type == "None") {
        model::event_bus.publish("filter_type_update", model::measurement::Processing::FilterType::None);
        view.enable_filter_order(false);
        view.enable_frequencies(false);
    }
}

void gui::presenter::settings::Filter::update_filter_type_viw(const model::measurement::Processing::FilterType type) const noexcept {
    switch (type) {
        case model::measurement::Processing::FilterType::Sinc:
            view.set_filter_type("Sinc");
            view.enable_filter_order(false);
            view.enable_frequencies(true);
            break;
        case model::measurement::Processing::FilterType::Butterworth:
            view.set_filter_type("Butterworth");
            view.enable_filter_order(true);
            view.enable_frequencies(true);
            break;
        case model::measurement::Processing::FilterType::None:
            view.set_filter_type("None");
            view.enable_filter_order(false);
            view.enable_frequencies(false);
            break;
    }
}
