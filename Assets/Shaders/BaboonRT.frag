#version 330 core
in vec2 uv;
out vec4 FragColor;

uniform sampler2D uScreen;

void main()
{
    FragColor = texture(uScreen, vec2(uv.x, 1.0 - uv.y));
}