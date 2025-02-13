#include "gui/view/settings/filter.h"

#include <QGridLayout>

gui::view::settings::Filter::Filter() noexcept:
QGroupBox(),
low_cut_off_frequency_label(new QLabel),
high_cut_off_frequency_label(new QLabel),
low_frequency(new QTextEdit),
high_frequency(new QTextEdit),
frequency_unit_low(new QLabel),
frequency_unit_high(new QLabel),
filter_type_label(new QLabel),
filter_type(new QComboBox),
filter_order_label(new QLabel),
filter_order(new QComboBox),
m_layout(new QGridLayout)
{
    setTitle("Digital Filter Settings");
    init_fonts();
    init_frequency_settings();
    init_labels();
    init_filter_type();
    init_filter_order();
    init_signals();
    setLayout(m_layout.get());
}



void gui::view::settings::Filter::set_low_frequency(const std::string& frequency) const noexcept {
    low_frequency->setText(QString(frequency.c_str()));
}

void gui::view::settings::Filter::set_high_frequency(const std::string& frequency) const noexcept {
    high_frequency->setText(QString(frequency.c_str()));
}

void gui::view::settings::Filter::init_frequency_settings() const noexcept {
    const auto sublayout_low = new QWidget;
    sublayout_low->setLayout(new QHBoxLayout);
    sublayout_low->layout()->addWidget(low_cut_off_frequency_label.get());
    sublayout_low->layout()->addWidget(low_frequency.get());
    sublayout_low->layout()->addWidget(frequency_unit_low.get());

    const auto sublayout_high = new QWidget;
    sublayout_high->setLayout(new QHBoxLayout);
    sublayout_high->layout()->addWidget(high_cut_off_frequency_label.get());
    sublayout_high->layout()->addWidget(high_frequency.get());
    sublayout_high->layout()->addWidget(frequency_unit_high.get());

    sublayout_low->setFixedSize(320, 55);
    sublayout_high->setFixedSize(320, 55);

    m_layout->addWidget(sublayout_low, 0, 0);
    m_layout->addWidget(sublayout_high, 1, 0);
}

void gui::view::settings::Filter::set_filter_type(const std::string &filter) const noexcept {
    filter_type->setCurrentText(filter.c_str());
}

void gui::view::settings::Filter::set_filter_types(const std::vector<std::string> &filters) const noexcept {
    for (const auto& filter: filters) {
        filter_type->addItem(filter.c_str());
    }
}

void gui::view::settings::Filter::init_fonts() noexcept {
    auto font = this->font();
    font.setPointSize(12);
    setFont(font);
    low_cut_off_frequency_label->setFont(font);
    high_cut_off_frequency_label->setFont(font);
    low_frequency->setFont(font);
    high_frequency->setFont(font);
    frequency_unit_low->setFont(font);
    frequency_unit_high->setFont(font);
    filter_type_label->setFont(font);
    filter_type->setFont(font);
    filter_order_label->setFont(font);
    filter_order->setFont(font);
}

void gui::view::settings::Filter::init_labels() const noexcept {
    frequency_unit_low->setText("Hz");
    frequency_unit_high->setText("Hz");
    low_cut_off_frequency_label->setText("Low Cut Off Frequency");
    high_cut_off_frequency_label->setText("High Cut Off Frequency");
    filter_type_label->setText("Filter Type");
    filter_order_label->setText("Filter Order");
}

void gui::view::settings::Filter::init_filter_type() const noexcept {
    const auto sublayout = new QWidget;
    sublayout->setLayout(new QHBoxLayout);
    sublayout->layout()->addWidget(filter_type_label.get());
    sublayout->layout()->addWidget(filter_type.get());
    m_layout->addWidget(sublayout, 0, 1);
}

void gui::view::settings::Filter::init_filter_order() const noexcept {
    const auto sublayout = new QWidget;
    sublayout->setLayout(new QHBoxLayout);
    sublayout->layout()->addWidget(filter_order_label.get());
    sublayout->layout()->addWidget(filter_order.get());
    m_layout->addWidget(sublayout, 1, 1);
}

void gui::view::settings::Filter::set_filter_orders(const std::vector<std::size_t> &orders) const noexcept {
    for (const auto& order: orders) {
        filter_order->addItem(QString(std::to_string(order).c_str()));
    }}

void gui::view::settings::Filter::set_filter_order(const std::size_t order) const noexcept {
    filter_order->setCurrentIndex(static_cast<int>(order) - 1);
}

void gui::view::settings::Filter::init_signals() noexcept {
    connect(low_frequency.get(), &QTextEdit::textChanged,
            [this]{presenter::settings::Filter::update_low_frequency(low_frequency->toPlainText().toStdString());});
     connect(high_frequency.get(), &QTextEdit::textChanged,
            [this]{presenter::settings::Filter::update_high_frequency(high_frequency->toPlainText().toStdString());});
     connect(filter_order.get(), &QComboBox::currentIndexChanged,
            [this]{presenter::settings::Filter::update_filter_order(filter_order->currentText().toStdString());});
     connect(filter_type.get(), &QComboBox::currentIndexChanged,
            [this]{presenter.update_filter_type(filter_type->currentText().toStdString());});
}

void gui::view::settings::Filter::enable_filter_order(const bool state) const noexcept {
    filter_order->setEnabled(state);
}

void gui::view::settings::Filter::enable_frequencies(const bool state) const noexcept {
    low_frequency->setEnabled(state);
    high_frequency->setEnabled(state);
}
