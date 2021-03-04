#version 450 core

layout(location = 0) in ivec4 positionAndType;

out VSOUT
{
	int vType;
} vs_out;

void main()
{
	vs_out.vType = positionAndType.w;
	gl_Position = vec4(positionAndType.xyz, 1.0f);
}