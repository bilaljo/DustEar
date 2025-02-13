#ifndef GUI_PRESENTER_SETTINS_FILTER_H
#define GUI_PRESENTER_SETTINS_FILTER_H

#include "gui/view/interface/settings/filter.h"
#include "gui/model/measurement/processing.h"

namespace gui::presenter::settings {
    class Filter {
    public:
        using ViewType = view::interfaces::settings::Filter;

    public:
        explicit Filter(ViewType& view);

        static void update_low_frequency(const std::string& frequency) noexcept;

        static void update_high_frequency(const std::string& frequency) noexcept;

        static void update_filter_order(const std::string& order) noexcept;

        void update_filter_type(const std::string &type) const noexcept;

        void update_filter_type_viw(model::measurement::Processing::FilterType type) const noexcept;

    private:
        using Frequencies = std::pair<std::size_t, std::size_t>;

        void update_frequencies_text(const Frequencies& frequencies) const noexcept;

        void update_filter_order_view(int order) const noexcept;

    private:
        ViewType& view;
    };
}

#endif  // GUI_PRESENTER_SETTINS_FILTER_H
