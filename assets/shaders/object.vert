#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aUV;

out vec2 uv;
out vec3 color;
out vec3 position;
out vec3 normal;
out vec3 eyePos;

layout (std140) uniform CameraBlock
{
    mat4 u_projection;
    mat4 u_view;
    vec3 u_eye_pos;
};
uniform mat4 u_model;

uniform vec4 u_texture_region;

void main()
{
  uv = aUV * u_texture_region.xy + u_texture_region.zw;
  position = vec3(u_model * vec4(aPos, 1.0));
  normal = normalize(mat3(u_model) * aNormal);
  eyePos = u_eye_pos;
  color = vec3(1.0);
    
  gl_Position = u_projection * u_view * vec4(position, 1.0);
}