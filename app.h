#pragma once

#include "glapplication.h"

#include "props.h"
#include "renderer.h"

class App : public lix::Application
{
public:
    App();

    virtual void init() override;
    virtual void tick(float dt) override;
    virtual void draw() override;

private:
    std::vector<lix::TRS> _unicorns;

    Renderer _renderer;
};