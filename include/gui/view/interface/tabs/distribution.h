#pragma once

#include "helper/histogram.h"

namespace gui::view::interfaces::tabs {
  class Distribution {
   public:
      virtual ~Distribution() = default;

      virtual void draw(const std::vector<double>& bins, const helper::Histogram<double>::Range& range) noexcept = 0;
  };
}
