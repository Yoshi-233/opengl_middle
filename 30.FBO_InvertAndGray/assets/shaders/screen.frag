#version 410 core
out vec4 FragColor;

in vec2 uv;
uniform sampler2D screenTextureSampler;

vec3 colorInvert(vec3 color)
{
        return vec3(1.0) - color;
}

vec3 gray(vec3 color)
{
        float gray = (color.r + color.g + color.b) / 3.0;
        return vec3(gray);
}

/* 人眼对绿色的敏感度较高 */
vec3 grayCorrect(vec3 color)
{
        float gray = color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722;
        return vec3(gray);
}

void main()
{
        vec3 color = grayCorrect(texture(screenTextureSampler, uv).rgb);

        FragColor = vec4(color, 1.0);
}