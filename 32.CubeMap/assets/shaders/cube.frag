#version 410 core
out vec4 fragColor;

in vec3 uvw;

uniform sampler2D diffuse;
uniform samplerCube cubeSampler;

void main()
{
        fragColor = texture(cubeSampler, uvw);
}