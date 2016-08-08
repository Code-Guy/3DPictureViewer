#version 330

in vec2 F_UV;
in vec3 F_Color;

out vec4 FragmentColor;

uniform sampler2D TextureMap;//纹理图

void main()
{
	FragmentColor = vec4(F_Color, texture2D(TextureMap, F_UV).a);
}