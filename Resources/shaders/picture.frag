#version 330

in vec2 FragTexCoord;
in vec4 LightSpacePosition;

out vec4 FragColor;

uniform sampler2D TextureMap;
uniform sampler2DShadow ShadowMap;
uniform bool IsBlur;
uniform float Alpha;

float normpdf(float x, float sigma)
{
	return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}

void GaussianBlur()
{
	const int mSize = 9;
	const int kSize = (mSize-1)/2;
	float kernel[mSize];
	vec3 final_colour = vec3(0.0);
		
	//create the 1-D kernel
	float sigma = 7.0;
	float Z = 0.0;
	for (int j = 0; j <= kSize; ++j)
	{
		kernel[kSize+j] = kernel[kSize-j] = normpdf(float(j), sigma);
	}

	//get the normalization factor (as the gaussian has been clamped)
	for (int j = 0; j < mSize; ++j)
	{
		Z += kernel[j];
	}
	
	vec2 resolution = textureSize(TextureMap, 0);
	//read out the texels
	for (int i=-kSize; i <= kSize; ++i)
	{
		for (int j=-kSize; j <= kSize; ++j)
		{
			final_colour += kernel[kSize+j] * kernel[kSize+i] * texture2D(TextureMap, FragTexCoord + vec2(float(i), float(j)) / resolution).rgb;
		}
	}

	FragColor = vec4(final_colour/(Z*Z), texture(TextureMap, FragTexCoord).a);
	FragColor.rgb = FragColor.rgb * Alpha + vec3(1, 1, 1) * (1 - Alpha);
}

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

    for (int y = -3; y <= 3; y++) 
    {
        for (int x = -3; x <= 3; x++) 
        {
            vec2 offset = vec2(x * xOffset, y * yOffset);
            vec3 uv = vec3(uvCoord + offset, z);
            factor += texture(ShadowMap, uv);
        }
    }

    return 0.7 + factor / 163;                                                                   
}

void main()
{
	if(IsBlur)
		GaussianBlur();
	else
		FragColor = texture(TextureMap, FragTexCoord);

	FragColor *= CalcShadowFactor();
}