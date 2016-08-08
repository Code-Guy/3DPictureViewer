#version 330

layout(location = 0) in vec3 Pos;//位置
layout(location = 1) in vec2 Size_Angle;//大小-位置
layout(location = 2) in vec3 Color;//颜色

uniform mat4 MV;//模型视图矩阵

out vec2 G_Size_Angle;
out vec3 G_Color;

void main()                     
{
	G_Size_Angle = Size_Angle;
	G_Color = Color;
    gl_Position = MV * vec4(Pos, 1.0);
}