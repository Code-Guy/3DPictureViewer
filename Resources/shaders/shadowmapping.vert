#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;

out vec2 FragTexCoord;

uniform mat4 LightMVP;

void main()
{                                                                               
    gl_Position = LightMVP * vec4(Position, 1.0);
    FragTexCoord = TexCoord;
}