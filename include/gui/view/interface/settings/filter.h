#ifndef GUI_VIEW_INTERFACE_SETTINGS_FILTER_H
#define GUI_VIEW_INTERFACE_SETTINGS_FILTER_H

#include <cstdlib>
#include <string>
#include <vector>

namespace gui::view::interfaces::settings {
    class Filter {
    public:
        virtual void set_low_frequency(const std::string& frequency) const noexcept = 0;

        virtual void set_high_frequency(const std::string& frequency) const noexcept = 0;

        virtual void set_filter_type(const std::string& filter) const noexcept = 0;

        virtual void set_filter_types(const std::vector<std::string>& filters) const noexcept = 0;

        virtual void set_filter_orders(const std::vector<std::size_t>& orders) const noexcept = 0;

        virtual void set_filter_order(std::size_t order) const noexcept = 0;

        virtual void enable_filter_order(bool state) const noexcept = 0;

        virtual void enable_frequencies(bool state) const noexcept = 0;
    };
}

#endif  // GUI_VIEW_INTERFACE_SETTINGS_FILTER_H
