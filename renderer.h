#pragma once

#include "glnode.h"
#include "glrendering.h"
#include "glinstancedrendering.h"

#include "camera.h"

#include "gen/shaders/bg_vert.h"
#include "gen/shaders/inst_vert.h"
#include "gen/shaders/object_vert.h"

#include "gen/shaders/bg_frag.h"
#include "gen/shaders/color_frag.h"
#include "gen/shaders/texture_frag.h"

#include "gen/images/unicorn64_png.h"
#include "gen/images/brick_png.h"
#include "gen/images/eye_png.h"

#include "props.h"

#include "sprite.h"

using TRSInstancedRendering = lix::InstancedRendering<std::vector<lix::TRS>>;

struct Renderer
{
    static void renderBackground()
    {
        static lix::ShaderProgram bgShader{
            assets::shaders::bg_vert,
            assets::shaders::bg_frag
        };
        bgShader.bind();
        lix::renderScreen();
    }

    static lix::MeshPtr iconMesh(const glm::vec2& dim)
    {
        float w = dim.x * 0.5f;
        float h = dim.y * 0.5f;
        lix::MeshPtr mesh = std::make_shared<lix::Mesh>(
            lix::Attributes{lix::VEC3, lix::VEC3, lix::VEC2}, std::vector<GLfloat>{
                -w, -h, 0.0f,   +0.0f, +0.0f, +1.0f,   0.0f, 0.0f,
                +w, -h, 0.0f,   +0.0f, +0.0f, +1.0f,   1.0f, 0.0f,
                +w, +h, 0.0f,   +0.0f, +0.0f, +1.0f,   1.0f, 1.0f,
                -w, +h, 0.0f,   +0.0f, +0.0f, +1.0f,   0.0f, 1.0f
            }, std::vector<GLuint> {
                0, 1, 2,
                2, 3, 0
            }, GL_TRIANGLES, GL_STATIC_DRAW
        );
        return mesh;
    }

    void init()
    {
        camera.setTarget(glm::vec3{0.0f});

        objectShader.reset(new lix::ShaderProgram(assets::shaders::object_vert, assets::shaders::texture_frag));
        instanceShader.reset(new lix::ShaderProgram(assets::shaders::inst_vert, assets::shaders::texture_frag));
        camera.setupUBO({
            objectShader.get(),
            instanceShader.get()
        });

        static constexpr glm::vec2 eyeSize{128, 128};
        static constexpr Sequence eyeSequence{eyeSize,
            glm::vec2{assets::images::eye_png_rgba::width, assets::images::eye_png_rgba::height}};

        eyeSprite.reset(new Sprite(
            lix::Texture::createTexture<assets::images::eye_png_rgba>(),
            eyeSequence
        ));
        eyeSprite->setScale(glm::vec3{eyeSize, 1.0f});
        eyeSprite->setTranslation(glm::vec3{glm::vec2{776.0f, 364.0f} + eyeSize * 0.5f, 0.0f});
        eyeSprite->addAnimation("idle", {
            {0, 1.4f},
            {1, 0.6f}
        });
    }

    void renderUnicorns()
    {
        instanceShader->bind();
        static lix::TexturePtr texture = lix::Texture::createTexture<assets::images::unicorn64_png_rgba>();
        texture->bind();
        unicornRendering->render(*instanceShader);
        texture->unbind();
    }

    static void bindBrickTexture()
    {
        static auto texture = lix::Texture::createTexture<assets::images::brick_png_rgba>();
        texture->bind();
    }

    void renderSprites()
    {
        objectShader->bind();

        static auto sprites = {eyeSprite};
        for(const auto& sprite : sprites)
        {
            sprite->texture()->bind(GL_TEXTURE0);
            objectShader->setUniform("u_model", sprite->model());
            objectShader->setUniform("u_texture_region", sprite->currentRegion());
            lix::renderQuad();
        }
    }

    std::shared_ptr<TRSInstancedRendering> unicornRendering;
    std::shared_ptr<lix::ShaderProgram> objectShader;
    std::shared_ptr<lix::ShaderProgram> instanceShader;
    Camera camera{
        //glm::perspective(glm::radians(45.0f), props::canvas_vec2.x / props::canvas_vec2.y, 0.1f, 100.0f),
        glm::ortho(0.0f, props::screen_vec2.x, 0.0f, props::screen_vec2.y, 0.1f, 1000.0f),
        glm::vec3{0.0f, 0.0f, 1.0f}};
    std::shared_ptr<Sprite> eyeSprite;
};