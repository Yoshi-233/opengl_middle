#version 410 core
out vec4 fragColor;

uniform sampler2D sampler;
uniform sampler2D specularMaskSampler;
in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

// 光源參數
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 ambientColor;//环境光

uniform float k2;
uniform float k1;
uniform float kc;

// 相机世界位置
uniform vec3 cameraPosition;

uniform float specularIntensity;

uniform float shininess;

void main()
{
        // 1. 计算光照的通用数据
        vec3 objColor = texture(sampler, uv).rgb;//物体颜色
        // diffuse 数据
        vec3 normalN = normalize(normal);
        vec3 lightDirN = normalize(worldPosition - lightPosition);
        // 摄像机到物体向量 数据
        vec3 viewDir = normalize(worldPosition - cameraPosition);

        // 计算衰减值
        float dist = length(worldPosition - lightPosition);
        float attenuation = 1.0 / (kc + k1 * dist + k2 * dist * dist);

        // 2. 准备漫反射相关数据
        float diffuse = clamp(dot(-lightDirN, normalN), 0.0f, 1.0f);// cos夹角
        vec3 diffuseColor = lightColor * diffuse * objColor;

        // 3. 计算specular
        // 防止背面光的照入
        float dotResult = dot(-lightDirN, normalN);
        float specularFlag = step(0.0f, dotResult);// 大于0返回1，否则返回0，类似if

        vec3 lightReflect = normalize(reflect(lightDirN, normalN));
        float specular = max(dot(lightReflect, -viewDir), 0.0f);
        specular = pow(specular, shininess);
        // 高光蒙板
        float specularMask = texture(specularMaskSampler, uv).r;

        // 这里并不需要objColor，光经过物体反射出去了，漫反射是物体吸收
        // specularIntensity控制光斑呈现的亮度
        vec3 specularColor = lightColor * specular * specularFlag * specularIntensity * specularMask;

        // 4. 环境光
        vec3 ambientColor = objColor * ambientColor;

        // 最终计算
        vec3 finalColor = (diffuseColor + specularColor) * attenuation + ambientColor;

        fragColor = vec4(finalColor, 1.0);
}