#ifndef GUI_VIEW_SETTINGS_FILTER_H
#define GUI_VIEW_SETTINGS_FILTER_H

#include "gui/view/interface/settings/filter.h"
#include "gui/presenter/settings/filter.h"

#include <memory>

#include <QGroupBox>
#include <QTextEdit>
#include <QLabel>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>


namespace gui::view::settings {
    class Filter final : public interfaces::settings::Filter, public QGroupBox{
    public:
        Filter() noexcept;

        ~Filter() override = default;

        void set_low_frequency(const std::string& frequency) const noexcept override;

        void set_high_frequency(const std::string& frequency) const noexcept override;

        void set_filter_types(const std::vector<std::string>& filters) const noexcept override;

        void set_filter_type(const std::string& filter) const noexcept override;

        void set_filter_orders(const std::vector<std::size_t>& orders) const noexcept override;

        void set_filter_order(std::size_t order) const noexcept override;

        void enable_filter_order(bool state) const noexcept override;

        void enable_frequencies(bool state) const noexcept override;

    private:
        void init_frequency_settings() const noexcept;

        void init_fonts() noexcept;

        void init_labels() const noexcept;

        void init_filter_type() const noexcept;

        void init_filter_order() const noexcept;

        void init_signals() noexcept;

    private:
        std::unique_ptr<QLabel> low_cut_off_frequency_label = nullptr;

        std::unique_ptr<QLabel> high_cut_off_frequency_label = nullptr;

        std::unique_ptr<QTextEdit> low_frequency = nullptr;

        std::unique_ptr<QTextEdit> high_frequency = nullptr;

        std::unique_ptr<QLabel> frequency_unit_low = nullptr;

        std::unique_ptr<QLabel> frequency_unit_high = nullptr;

        std::unique_ptr<QLabel> filter_type_label = nullptr;

        std::unique_ptr<QComboBox> filter_type = nullptr;

        std::unique_ptr<QLabel> filter_order_label = nullptr;

        std::unique_ptr<QComboBox> filter_order = nullptr;

        std::unique_ptr<QGridLayout> m_layout = nullptr;

        presenter::settings::Filter presenter{*this};
    };
}


#endif  // GUI_VIEW_SETTINGS_FILTER_H
