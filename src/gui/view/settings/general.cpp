#include "gui/view/settings/general.h"
#include "gui/presenter/settings/general.h"


gui::view::settings::General::General():
QGroupBox(),
save_raw_data(new widgets::AnimatedToggle),
save_raw_data_label(new QLabel),
save_pulses(new widgets::AnimatedToggle),
save_pulses_label(new QLabel),
save_settings(new QPushButton{"Save Settings"}),
load_settings(new QPushButton{"Load Settings"}),
m_layout(new QGridLayout)
{
    setTitle("General Settings");
    init_labels();
    init_fonts();
    init_save_raw_data();
    init_pulses();
    init_settings_buttons();
    init_signals();
    setLayout(m_layout.get());
    setFixedSize(450, 150);
}

void gui::view::settings::General::set_save_raw_data(const bool state) const noexcept {
    save_raw_data->setChecked(state);
}

void gui::view::settings::General::set_save_pulses(const bool state) const noexcept {
    save_pulses->setChecked(state);
}

void gui::view::settings::General::init_labels() const noexcept {
    save_raw_data_label->setText("Save Raw Data");
    save_pulses_label->setText("Save Detected Pulses");
}

void gui::view::settings::General::init_fonts() noexcept {
    auto font = this->font();
    font.setPointSize(12);
    save_raw_data_label->setFont(font);
    save_pulses_label->setFont(font);
    save_settings->setFont(font);
    load_settings->setFont(font);
    setFont(font);
}

void gui::view::settings::General::init_save_raw_data() const noexcept {
    m_layout->addWidget(save_raw_data_label.get(), 0, 0);
    m_layout->addWidget(save_raw_data.get(), 0, 1);
}

void gui::view::settings::General::init_pulses() const noexcept {
    m_layout->addWidget(save_pulses_label.get(), 0, 2);
    m_layout->addWidget(save_pulses.get(), 0, 3);
}

void gui::view::settings::General::init_settings_buttons() const noexcept {
    auto sub_layout = new QWidget;
    sub_layout->setLayout(new QHBoxLayout);
    sub_layout->layout()->addWidget(save_settings.get());
    sub_layout->layout()->addWidget(load_settings.get());
    m_layout->addWidget(sub_layout, 1, 0, 1, 4);
}

void gui::view::settings::General::init_signals() const noexcept {
    connect(save_raw_data.get(), &widgets::AnimatedToggle::checkStateChanged,
            [this](){Presenter::update_save_raw_data_model(save_raw_data->isChecked());});
    connect(save_pulses.get(), &widgets::AnimatedToggle::checkStateChanged,
            [this](){Presenter::update_save_pulses_model(save_pulses->isChecked());});
    connect(save_settings.get(), &QPushButton::clicked,
            [this](){Presenter::save_settings_model();});
    connect(load_settings.get(), &QPushButton::clicked,
            [this](){});
}
