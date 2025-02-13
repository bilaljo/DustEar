#include "gui/view/general_purpose/progress_bar.h"


gui::view::general_purpose::ProgressBar::ProgressBar(int min_value, int max_value): QProgressBar() {
    setMinimum(min_value);
    setMaximum(max_value);
    setRange(min_value, max_value);
}

void gui::view::general_purpose::ProgressBar::update(int value) noexcept {
    setValue(value);
}

void gui::view::general_purpose::ProgressBar::reset() noexcept {
    QProgressBar::reset();
}
