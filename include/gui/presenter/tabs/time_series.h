#ifndef GUI_PRESENTER_TABS_TIME_SERIES_H
#define GUI_PRESENTER_TABS_TIME_SERIES_H

#include <unordered_map>

#include "gui/model/measurement/processing.h"
#include "gui/view/interface/tabs/time_series.h"

namespace gui::presenter::tabs {
    class TimeSeries {
    public:
        using View = view::interfaces::tabs::TimeSeries;

        using Model = model::measurement::Processing;

        using Buffer = helper::RingBuffer<double, model::measurement::Processing::BUFFER_SIZE>;

    public:
        explicit TimeSeries(View& view) noexcept;

        void update_line_chart(const std::unordered_map<std::string, std::pair<std::vector<double>, std::vector<double>>>& data) const noexcept;

        void update_line_chart(const std::unordered_map<std::string, std::pair<Buffer, Buffer>> &data) const noexcept;

    private:
        View& view;
    };
}

#endif  // GUI_PRESENTER_TABS_TIME_SERIES_H
