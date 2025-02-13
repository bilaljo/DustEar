#include "gui/view/general_purpose/status_bar.h"
#include "gui/presenter/general_purpose/status_bar.h"




gui::view::general_purpose::StatusBar::StatusBar():
directory(new QLabel),
particle_conentration(new QLabel),
particle_conentration_unit(new QLabel)
{
    this->addWidget(directory.get());
    set_font(directory);
    set_font(particle_conentration);
    set_font(particle_conentration_unit);
    this->addPermanentWidget(particle_conentration.get());
    particle_conentration->setText("0");
    particle_conentration_unit->setText("Particles per m³");
    this->addPermanentWidget(particle_conentration_unit.get());
}

void gui::view::general_purpose::StatusBar::set_font(const std::unique_ptr<QLabel> &label) const noexcept {
    auto font = this->font();
    font.setPointSize(11);
    label->setFont(font);
}

void gui::view::general_purpose::StatusBar::update_directory(const std::string &path) const noexcept {
    set_font(directory);
    directory->setText(path.c_str());
}

void gui::view::general_purpose::StatusBar::update_particle_concentration(const std::string& number) const noexcept {
    set_font(particle_conentration);
    particle_conentration->setText(number.c_str());
}
