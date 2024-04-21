#include "app.h"

#include "glm/glm.hpp"

template<size_t W, size_t H>
struct Wall
{
    constexpr Wall(const glm::vec3& p0)
    {
        for(size_t y{0}; y < H; ++y)
        {
            for(size_t x{0}; x < W; ++x)
            {
                _bricks[y * W + x] = p0 + glm::vec3{(y % 2) * 0.5f * props::brick_size.x + x * (props::brick_size.x + 1.0f), y * (props::brick_size.y + 1.0f), 0.0f};
            }
        }
    }

    void render(std::shared_ptr<lix::Mesh> mesh, lix::ShaderProgram& shaderProgram) const
    {
        static lix::InstancedRendering<const std::array<glm::vec3, W * H>> rendering{mesh, _bricks};
        rendering.render(shaderProgram);
    }

    std::array<glm::vec3, W * H> _bricks;
};


constexpr Wall<3, 8> wall_01{{glm::vec2{380.0f, 100.0f} + props::brick_size * 0.5f, 0.0f}};
constexpr Wall<4, 12> wall_02{{glm::vec2{580.0f, 100.0f} + props::brick_size * 0.5f, 0.0f}};
constexpr Wall<5, 24> wall_03{{glm::vec2{780.0f, 100.0f} + props::brick_size * 0.5f, 0.0f}};


App::App() : lix::Application{props::screen_width, props::screen_height, "Unicorn Tower Attack"}
{
    
}

void App::init()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    glClearColor(0.06f, 0.06f, 0.06f, 1.0f);

    // glm::angleAxis(glm::radians(135.0f), glm::vec3{0.0f, 1.0f, 0.0f})
    _unicorns.push_back({glm::vec3{glm::vec2{100.0f, 100.0f} + props::unicorn_size * 0.5f, 0.0f}, glm::quat{1.0f, 0.0f, 0.0f, 0.0f}, glm::vec3{1.0f}});

    _renderer.unicornRendering.reset(new TRSInstancedRendering(Renderer::iconMesh(props::unicorn_size),
        _unicorns
    ));

    _renderer.init();
}

void App::tick(float dt)
{
    _renderer.camera.refresh(dt);
    _renderer.eyeSprite->update(dt);
}

void App::draw()
{
    //constexpr int paddingX = (props::screen_width - props::canvas_width) / 2;
    //constexpr int paddingY = 100;
    //glViewport(paddingX, paddingY, props::canvas_width, props::canvas_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Renderer::renderBackground();
    glClear(GL_DEPTH_BUFFER_BIT);
    _renderer.renderUnicorns();
    _renderer.instanceShader->bind();
    Renderer::bindBrickTexture();
    wall_01.render(Renderer::iconMesh(props::brick_size), *_renderer.instanceShader);
    wall_02.render(Renderer::iconMesh(props::brick_size), *_renderer.instanceShader);
    wall_03.render(Renderer::iconMesh(props::brick_size), *_renderer.instanceShader);

    _renderer.renderSprites();
}