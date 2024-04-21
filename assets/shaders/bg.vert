#version 330 core

layout(location=0) in vec2 aPos;
layout(location=1) in vec2 aUV;

out vec2 uv;

void main()
{
    uv = aUV; 
    gl_Position = vec4(aPos * vec2(0.8333, 0.5) - vec2(0.0, 0.22), 0.0, 1.0);
}