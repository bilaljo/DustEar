#include "gui/view/settings/calculation_method.h"
#include "gui/presenter/settings/calculation_method.h"


gui::view::settings::CalculationMethod::CalculationMethod():
QGroupBox(),
change_method_label(new QLabel),
change_method(new QComboBox),
change_detection_method_label(new QLabel),
change_detection_method(new QComboBox),
resonance_frequency_label(new QLabel),
resonance_frequency(new QTextEdit),
resonance_frequency_unit(new QLabel),
time_constant_label(new QLabel),
time_constant(new QTextEdit),
time_constant_unit(new QLabel),
m_layout(new QGridLayout)
{
    setTitle("Parameter Calculation Settings");
    init_fonts();
    init_labels();
    init_change_method();
    init_signals();
    init_initial_guesses();
    setLayout(m_layout.get());
}


void gui::view::settings::CalculationMethod::set_calculation_method(const std::string &method) const noexcept {
    change_method->setCurrentText(method.c_str());
}

void gui::view::settings::CalculationMethod::set_detection_method(const std::string &method) const noexcept {
    change_detection_method->setCurrentText(method.c_str());
}


void gui::view::settings::CalculationMethod::init_labels() const noexcept {
    change_method_label->setText("Parameter Calculation");
    change_detection_method_label->setText("Pulse Detection");
    resonance_frequency_label->setText("Resonance Frequency");
    resonance_frequency_unit->setText("kHz");
    time_constant_label->setText("Time Constant");
    time_constant_unit->setText("µs");
}

void gui::view::settings::CalculationMethod::init_fonts() noexcept {
    auto font = this->font();
    font.setPointSize(12);
    setFont(font);
    change_method_label->setFont(font);
    change_method->setFont(font);
    change_detection_method_label->setFont(font);
    change_detection_method->setFont(font);
    resonance_frequency_label->setFont(font);
    resonance_frequency->setFont(font);
    resonance_frequency_unit->setFont(font);
    time_constant_label->setFont(font);
    time_constant->setFont(font);
    time_constant_unit->setFont(font);
}

void gui::view::settings::CalculationMethod::init_change_method() const noexcept {
    const auto sublayout = new QGridLayout;
    sublayout->addWidget(change_method_label.get(), 0, 0);
    sublayout->addWidget(change_method.get(), 0, 1);
    sublayout->addWidget(change_detection_method_label.get(), 1, 0);
    sublayout->addWidget(change_detection_method.get(), 1, 1);
    const auto container = new QWidget;
    container->setLayout(sublayout);
    m_layout->addWidget(container, 0, 2);
}

void gui::view::settings::CalculationMethod::set_calculation_methods(const std::vector<std::string> &methods) const noexcept {
    for (const auto& method: methods) {
        change_method->addItem(method.c_str());
    }
}

void gui::view::settings::CalculationMethod::set_detection_methods(const std::vector<std::string> &methods) const noexcept {
    for (const auto& method: methods) {
        change_detection_method->addItem(method.c_str());
    }
}

void gui::view::settings::CalculationMethod::set_frequency(const std::string& frequency) const noexcept {
    resonance_frequency->setText(frequency.c_str());
}

void gui::view::settings::CalculationMethod::set_time_constant(const std::string& time_constant) const noexcept {
    this->time_constant->setText(time_constant.c_str());
}

void gui::view::settings::CalculationMethod::init_signals() const noexcept {
    connect(change_method.get(), &QComboBox::currentIndexChanged,
            [this](){
            presenter::settings::CalculationMethod::update_calculation_method_model(change_method->currentText().toStdString());
    });
    connect(change_detection_method.get(), &QComboBox::currentIndexChanged,
            [this](){
            presenter::settings::CalculationMethod::update_detection_method_model(change_detection_method->currentText().toStdString());
    });
    connect(resonance_frequency.get(), &QTextEdit::textChanged,
            [this](){presenter::settings::CalculationMethod::update_resonance_frequency_model(resonance_frequency->toPlainText().toStdString());});
    connect(time_constant.get(), &QTextEdit::textChanged,
            [this](){presenter::settings::CalculationMethod::update_time_constant_model(time_constant->toPlainText().toStdString());});
}

void gui::view::settings::CalculationMethod::init_initial_guesses() const noexcept {
    const auto sublayout = new QWidget;
    const auto grid = new QGridLayout;
    grid->addWidget(resonance_frequency_label.get(), 0, 0);
    grid->addWidget(resonance_frequency.get(), 1, 0);
    grid->addWidget(resonance_frequency_unit.get(), 1, 1);
    grid->addWidget(time_constant_label.get(), 2, 0);
    grid->addWidget(time_constant.get(), 3, 0);
    grid->addWidget(time_constant_unit.get(), 3, 1);
    sublayout->setLayout(grid);
    m_layout->addWidget(sublayout, 0, 1);
}
