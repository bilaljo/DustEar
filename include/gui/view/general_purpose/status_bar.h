#ifndef GUI_VIEW_GENERAL_PURPOSE_STATUS_BAR_H
#define GUI_VIEW_GENERAL_PURPOSE_STATUS_BAR_H

#include <memory>

#include <QStatusBar>
#include <QLabel>

#include "gui/view/interface/general_purpose/status_bar.h"
#include "gui/presenter/general_purpose/status_bar.h"

namespace gui::view::general_purpose {
    class StatusBar: public QStatusBar, public interfaces::general_purpose::StatusBar {
    public:
        StatusBar();

        void update_directory(const std::string& path) const noexcept override;

        void update_particle_concentration(const std::string& number) const noexcept override;

    private:
        void set_font(const std::unique_ptr<QLabel> &label) const noexcept;

    private:
        std::unique_ptr<QLabel> directory;

        std::unique_ptr<QLabel> particle_conentration;

        std::unique_ptr<QLabel> particle_conentration_unit;

        presenter::general_purpose::StatusBar presenter{*this};
    };
}

#endif  // GUI_VIEW_GENERAL_PURPOSE_STATUS_BAR_H
