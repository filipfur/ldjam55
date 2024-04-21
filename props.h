#pragma once

#include "glm/glm.hpp"

namespace props
{
    static const int screen_width{960};
    static const int screen_height{720};
    inline static constexpr glm::vec2 screen_vec2{static_cast<float>(props::screen_width),
        static_cast<float>(props::screen_height)};

    static const int canvas_width{900};
    static const int canvas_height{360};
    inline static constexpr glm::vec2 canvas_vec2{canvas_width, canvas_height};

    inline static constexpr glm::vec2 brick_size{20.0f, 10.0f};
    inline static constexpr glm::vec2 unicorn_size{64.0f, 64.0f};
}