#pragma once

#include "helper/histogram.h"

#include <vector>
#include <map>

namespace gui::view::interfaces::tabs {
  class TimeSeries {
   public:
    virtual ~TimeSeries() = default;

    virtual void update_chart(const std::vector<double>& x, const std::vector<double>& y) const noexcept = 0;

    [[nodiscard]] virtual std::string get_name() const noexcept = 0;
  };
}
