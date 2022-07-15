#version 450 core

layout (location = 0) in vec2 aPos;

out vec2 FragPos;

uniform float aspect;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    FragPos = vec2(aspect * aPos.x, aPos.y);
}