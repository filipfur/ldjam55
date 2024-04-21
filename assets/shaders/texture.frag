#version 330 core

precision highp float;

in vec2 uv;

uniform sampler2D u_texture;

out vec4 fragColor;

void main()
{
    fragColor = texture(u_texture, uv);
}