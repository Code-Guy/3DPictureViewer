#version 330

in vec2 FragTexCoord;
in vec4 LightSpacePosition;

out vec4 FragColor;

uniform sampler2D TextureMap;
uniform sampler2DShadow ShadowMap;

float CalcShadowFactor()                                                  
{   
	vec2 shadowMapSize = textureSize(ShadowMap, 0);
    vec3 projCoord = LightSpacePosition.xyz / LightSpacePosition.w;                                  
    vec2 uvCoord;                                                                          
    uvCoord.x = 0.5 * projCoord.x + 0.5;                                                  
    uvCoord.y = 0.5 * projCoord.y + 0.5;   

    float z = 0.5 * projCoord.z + 0.5;

    float xOffset = 1.0 / shadowMapSize.x;
    float yOffset = 1.0 / shadowMapSize.y;

    float factor = 0.0;

    for (int y = -2; y <= 2 ; y++) 
    {
        for (int x = -2; x <= 2 ; x++) 
        {
            vec2 offset = vec2(x * xOffset, y * yOffset);
            vec3 uv = vec3(uvCoord + offset, z);
            factor += texture(ShadowMap, uv);
        }
    }

    return 0.5 + factor / 50;                                                                   
}

void main()
{
	FragColor = texture(TextureMap, FragTexCoord) * CalcShadowFactor();
}