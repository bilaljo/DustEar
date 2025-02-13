#ifndef GUI_PRESENTER_TABS_DISTRIBUTION_H
#define GUI_PRESENTER_TABS_DISTRIBUTION_H

#include <unordered_map>

#include "gui/view/interface/tabs/distribution.h"
#include "gui/model/measurement/processing.h"

namespace gui::presenter::tabs {
  class Distribution {
   public:
      using View = view::interfaces::tabs::Distribution;

      using Model = model::measurement::Processing;

   public:
      explicit Distribution(View& view) noexcept;

      void update_histogram(const Model::Histogram::Range& range, const Model::Histogram& histogram) const noexcept;

   private:
      View& view;
  };
}

#endif  // GUI_PRESENTER_TABS_DISTRIBUTION_H
