#version 410 core
out vec4 fragColor;

in vec2 uv;
uniform sampler2D screenTextureSampler;

void main()
{
        fragColor = texture(screenTextureSampler, uv);
}