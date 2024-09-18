#version 410 core
out vec4 fragColor;

uniform sampler2D grassSampler;
uniform sampler2D landSampler;
uniform sampler2D noiseSampler;
uniform sampler2D dogSampler;
in vec2 uv;
in vec3 normal;

uniform vec3 lightDirection;
uniform vec3 lightColor;

void main()
{
        vec4 grassColor = texture(grassSampler, uv);
        vec4 landColor = texture(landSampler, uv);
        vec4 noiseColor = texture(noiseSampler, uv);
        vec4 dogColor = texture(dogSampler, uv);
        float weight = noiseColor.r;

        // 1. 物体颜色
        vec3 objColor = dogColor.rgb;
        // 2. diffuse 数据
        vec3 normalN = normalize(normal);
        vec3 lightDirectionN = normalize(lightDirection);

        float diffuse = clamp(dot(-lightDirectionN, normalN), 0.0f, 1.0f);// cos夹角

        fragColor = vec4(lightColor * diffuse * objColor, 0);
}