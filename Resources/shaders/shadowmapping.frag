#version 330

in vec2 FragTexCoord;
uniform sampler2D ShadowMap;

out vec4 FragColor;

void main()
{
    float depth = texture(ShadowMap, FragTexCoord).x;
    depth = 1.0 - (1.0 - depth) * 25.0;
    FragColor = vec4(depth);
}