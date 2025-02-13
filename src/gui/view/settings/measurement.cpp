#include "gui/view/settings/measurement.h"
#include "gui/presenter/settings/measurement.h"

gui::view::settings::Measurement::Measurement():
QGroupBox(),
trigger_level_label(new QLabel), trigger_level(new QTextEdit), trigger_level_unit(new QLabel),
dead_time_label(new QLabel), dead_time(new QTextEdit), dead_time_unit(new QLabel),
m_layout(new QGridLayout)
{
    setTitle("Measurement Settings");
    init_fonts();
    init_labels();
    init_trigger_level();
    init_dead_time();
    init_signals();
    setLayout(m_layout.get());
}

void gui::view::settings::Measurement::set_trigger_level(const std::string &trigger_level_str) const noexcept {
    trigger_level->setText(trigger_level_str.c_str());
}

void gui::view::settings::Measurement::set_dead_time(const std::string &dead_time_str) const noexcept {
    dead_time->setText(dead_time_str.c_str());
}

void gui::view::settings::Measurement::set_trigger_level_unit(const std::string& unit) const noexcept {
    trigger_level_unit->setText(unit.c_str());
}

void gui::view::settings::Measurement::init_fonts() {
    auto font = this->font();
    font.setPointSize(12);
    setFont(font);
    dead_time->setFont(font);
    dead_time_label->setFont(font);
    trigger_level->setFont(font);
    trigger_level_label->setFont(font);
    trigger_level_unit->setFont(font);
    dead_time_unit->setFont(font);
}

void gui::view::settings::Measurement::init_labels() {
    trigger_level_label->setText("Trigger Level");
    trigger_level_unit->setText("V");
    dead_time_label->setText("Dead Time");
    dead_time_unit->setText("Samples");
}

void gui::view::settings::Measurement::init_trigger_level() {
    auto sublayout = new QWidget;
    sublayout->setLayout(new QHBoxLayout);
    sublayout->layout()->addWidget(trigger_level_label.get());
    sublayout->layout()->addWidget(trigger_level.get());
    sublayout->layout()->addWidget(trigger_level_unit.get());
    sublayout->setFixedSize(320, 55);
    m_layout->addWidget(sublayout, 0, 0);
}

void gui::view::settings::Measurement::init_dead_time() {
    auto sublayout = new QWidget;
    sublayout->setLayout(new QHBoxLayout);
    sublayout->layout()->addWidget(dead_time_label.get());
    sublayout->layout()->addWidget(dead_time.get());
    sublayout->layout()->addWidget(dead_time_unit.get());
    sublayout->setFixedSize(320, 55);
    m_layout->addWidget(sublayout, 1, 0);
}

void gui::view::settings::Measurement::init_signals() const noexcept {
    connect(trigger_level.get(), &QTextEdit::textChanged,
            [this](){
                presenter.update_trigger_level(trigger_level->toPlainText().toStdString());
    });
    connect(dead_time.get(), &QTextEdit::textChanged,
            [this](){Presenter::update_dead_time(dead_time->toPlainText().toStdString());});
}
