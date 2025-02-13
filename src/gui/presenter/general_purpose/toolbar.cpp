#include "gui/presenter/general_purpose/toolbar.h"

#include <thread>

#include "helper/logging.h"
#include "gui/model/event_bus.h"

using namespace gui;

presenter::general_purpose::Toolbar::Toolbar(viewType &view): view(view) {
    auto process_online = [this](const auto data){processing.online(data);};
    auto save_raw_data = [this](const auto data){processing.save_raw_data(data);};
    daq.set_callbacks({save_raw_data, process_online});
}

void presenter::general_purpose::Toolbar::on_settings() const noexcept {
    view.show_settings();
}

void presenter::general_purpose::Toolbar::on_run() noexcept {
    if (daq.is_running()) {
        if (daq.stop()) {
            view.set_stopped();
        }
    } else {
        processing.set_init_headers();
        if (daq.run()) {
            view.set_running();
        }
    }
}

void presenter::general_purpose::Toolbar::on_directory(const std::string& directory) const noexcept {
    if (directory.empty()) {
        logging::error("No Destination Folder set");
    } else {
        model::event_bus.publish("destination_folder", directory);
        processing.set_destination_folder(directory);
    }
}

void presenter::general_purpose::Toolbar::process(const std::string& file_path) noexcept {
    if (file_path.empty()) {
        logging::error("No File given");
    } else {
        if constexpr (model::measurement::Processing::ONLINE_TEST) {
            std::thread{[this](const std::string& file_path){processing.online_test(file_path);}, file_path}.detach();
        } else {
            std::thread{[this](const std::string& file_path){processing.offline(file_path);}, file_path}.detach();
        }
    }
}

void presenter::general_purpose::Toolbar::on_plot(const std::string& file_path) noexcept {
    if (file_path.empty()) {
        logging::error("No File given");
    } else {
        processing.create_histogram(file_path);
        processing.create_line_charts(file_path);
    }
}
