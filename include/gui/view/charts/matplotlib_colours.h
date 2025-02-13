#ifndef GUI_VIEW_GENERAL_PURPOSE_MATPLOTLIBCOLOURS_H
#define GUI_VIEW_GENERAL_PURPOSE_MATPLOTLIBCOLOURS_H

#include <array>

namespace gui::view::charts {
    enum class MatplotlibColour {
        blue = 0x1f77b4,
        orange = 0xff7f0e,
        green = 0x2ca02c,
        red = 0xd62728,
        purple = 0x9467bd,
        brown = 0x8c564b,
        pink = 0xe377c2,
        gray = 0x7f7f7f,
        olive = 0xbcbd22,
        cyan = 0x17becf,
    };

    constexpr std::array MatplotlibColourArray{
        0x1f77b4, // blue
        0xff7f0e, // orange
        0x2ca02c, // green
        0xd62728, // red
        0x9467bd, // purple
        0x8c564b, // brown
        0xe377c2, // pink
        0x7f7f7f, // gray
        0xbcbd22, // olive
        0x17becf  // cyan
    };
}

#endif  // GUI_VIEW_GENERAL_PURPOSE_MATPLOTLIBCOLOURS_H
