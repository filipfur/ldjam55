#pragma once

#include "glm/glm.hpp"

#include <unordered_map>
#include <string_view>

#include "gltexture.h"

constexpr int countFrames(const glm::vec2 frameSize, const glm::vec2& imageSize)
{
    glm::vec2 v = imageSize / frameSize;
    return static_cast<int>(v.x * v.y);
} 

struct Sequence
{
    constexpr Sequence(const glm::vec2& frameSize_, const glm::vec2& imageSize_)
        : frameSize{frameSize_}, frames{countFrames(frameSize_, imageSize_)}, window{frameSize_ / imageSize_}
    {

    }

    glm::vec4 textureRegion(int frame) const
    {
        return glm::vec4{window, glm::vec2{static_cast<float>(frame) / static_cast<float>(frames), 0.0f}};
    }

    const glm::vec2 frameSize;
    const glm::vec2 window;
    int frames;
};

class Sprite : public lix::TRS
{
public:
    using AnimationFrame = std::pair<uint32_t, float>;
    using Animations = std::unordered_map<std::string_view, std::vector<AnimationFrame>>;

    Sprite(lix::TexturePtr texture, const Sequence& sequence) : _texture{texture}, _sequence{sequence}
    {

    }

    void update(float dt)
    {
        auto& frames = _currentAnimation->second;
        static float timeToNext{frames[_currentFrame].second};
        timeToNext -= dt;
        if(timeToNext <= 0)
        {
            _currentFrame = (_currentFrame + 1) % frames.size();
            timeToNext += frames[_currentFrame].second;
        }
    }

    void setAnimation(const std::string_view& str)
    {
        _currentAnimation = _animations.find(str);
        _currentFrame = 0;
    }

    glm::vec4 currentRegion() const
    {
        return _sequence.textureRegion(_currentAnimation->second[_currentFrame].first);
    }

    void addAnimation(const std::string_view& str, const std::vector<AnimationFrame>& frames)
    {
        _animations.emplace(str, frames);
        _currentAnimation = std::begin(_animations);
    }

    lix::TexturePtr texture()
    {
        return _texture;
    }

private:
    lix::TexturePtr _texture;
    const Sequence& _sequence;
    Animations _animations;
    Animations::iterator _currentAnimation;
    int _currentFrame{0};
};