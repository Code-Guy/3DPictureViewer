#version 330

layout(points) in;//输入一个点
layout(triangle_strip, max_vertices = 4) out;//输出一个4个点的triangle strip（四边形）

in vec2 G_Size_Angle[];
in vec3 G_Color[];

out vec2 F_UV;
out vec3 F_Color;

uniform mat4 P;

void main()
{
	vec4 center = gl_in[0].gl_Position;

	float size = G_Size_Angle[0].x;
	float angle = G_Size_Angle[0].y;

	float cosVal = cos(45 - angle);
	float sinVal = sin(45 - angle);

	vec4 v0 = vec4(center.xy + vec2(cosVal, sinVal) * size, center.z, 1.0);	//1象限
    vec4 v1 = vec4(center.xy + vec2(-sinVal, cosVal) * size, center.z, 1.0);	//2象限
    vec4 v2 = vec4(center.xy + vec2(-cosVal, -sinVal) * size, center.z, 1.0);  //3象限
   	vec4 v3 = vec4(center.xy + vec2(sinVal, -cosVal) * size, center.z, 1.0);  //4象限

   	F_Color = G_Color[0];

   	//0--1--3--2三角形条带规定顺序
	gl_Position = P * v0;
	F_UV = vec2(1, 1);
	EmitVertex();

	gl_Position = P * v1;
	F_UV =  vec2(0, 1);
	EmitVertex();

	gl_Position = P * v3;
	F_UV =  vec2(1, 0);
	EmitVertex();

	gl_Position = P * v2;
	F_UV = vec2(0, 0);
	EmitVertex();

	EndPrimitive();
}
