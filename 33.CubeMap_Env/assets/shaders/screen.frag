#version 410 core
out vec4 FragColor;

in vec2 uv;
uniform sampler2D screenTextureSampler;

uniform float textureWidth;
uniform float textureHeight;

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
vec3 blur()
{
        float du = 1.0 / textureWidth;
        float dv = 1.0 / textureHeight;

        vec2 offsets[9] = vec2[](
                vec2(-du, dv), // 左上
                vec2(0.0, -dv), // 上
                vec2(du, dv), // 右上
                vec2(-du, 0.0), // 左
                vec2(0.0, 0.0), // 中央
                vec2(du, 0.0), // 右
                vec2(-du, -dv), // 左下
                vec2(0.0, -dv), // 下
                vec2(du, -dv) // 右下
        );

        float kernel[9] = float[](
                1.0, 2.0, 1.0,
                2.0, 4.0, 2.0,
                1.0, 2.0, 1.0
        );

        vec3 sumColor = vec3(0.0);
        for(int i = 0; i < 9; i++) {
                // 采样9次
                vec3 sampleColor = texture(screenTextureSampler, uv + offsets[i]).rgb;
                sumColor += sampleColor * kernel[i];
        }

        return sumColor / 16.0;
}

void main()
{
        vec3 color = blur();

        FragColor = vec4(color, 1.0);
}