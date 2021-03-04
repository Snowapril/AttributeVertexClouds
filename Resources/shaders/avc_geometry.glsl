#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 14) out;

uniform float blockSize;

in VSOUT
{
	int vType;
} gs_in[];

out GSOUT
{
	flat vec3 normal;
	flat int type;
	vec3 localCoord;
} gs_out;

layout(std140) uniform CamMatrices
{
	mat4 projection;
	mat4 view;
	mat4 viewProj;
};

void emit(in vec3 position, in vec3 normal, in vec3 localCoord)
{
	gl_Position = viewProj * vec4(position, 1.0);
	gs_out.normal = normal;
	gs_out.type = gs_in[0].vType;
	gs_out.localCoord = localCoord;
	EmitVertex();
}

void main()
{
	vec3 center = (gl_in[0].gl_Position.xyz) * blockSize;
	vec3 llf = center - blockSize * 0.5;
	vec3 urb = center + blockSize * 0.5;
	//! Emit triangle strip for generating cube
	emit(vec3(llf.x, urb.y, llf.z), vec3(0.0, 1.0, 0.0), vec3(-1.0, 1.0, -1.0));
	emit(vec3(llf.x, urb.y, urb.z), vec3(0.0, 1.0, 0.0), vec3(-1.0, 1.0,  1.0));
	emit(vec3(urb.x, urb.y, llf.z), vec3(0.0, 1.0, 0.0), vec3(1.0, 1.0, -1.0));
	emit(vec3(urb.x, urb.y, urb.z), vec3(0.0, 1.0, 0.0), vec3(1.0, 1.0, 1.0));
	emit(vec3(urb.x, llf.y, urb.z), vec3(1.0, 0.0, 0.0), vec3(1.0, -1.0, 1.0));
	emit(vec3(llf.x, urb.y, urb.z), vec3(0.0, 0.0, 1.0), vec3(-1.0, 1.0, 1.0));
	emit(vec3(llf.x, llf.y, urb.z), vec3(0.0, 0.0, 1.0), vec3(-1.0, -1.0, 1.0));
	emit(vec3(llf.x, urb.y, llf.z), vec3(-1.0, 0.0, 0.0), vec3(-1.0, 1.0, -1.0));
	emit(vec3(llf.x, llf.y, llf.z), vec3(-1.0, 0.0, 0.0), vec3(-1.0, -1.0, -1.0));
	emit(vec3(urb.x, urb.y, llf.z), vec3(0.0, 0.0, -1.0), vec3(1.0, 1.0, -1.0));
	emit(vec3(urb.x, llf.y, llf.z), vec3(0.0, 0.0, -1.0), vec3(1.0, -1.0, -1.0));
	emit(vec3(urb.x, llf.y, urb.z), vec3(1.0, 0.0, 0.0), vec3(1.0, -1.0, 1.0));
	emit(vec3(llf.x, llf.y, llf.z), vec3(0.0, -1.0, 0.0), vec3(-1.0, -1.0, -1.0));
	emit(vec3(llf.x, llf.y, urb.z), vec3(0.0, -1.0, 0.0), vec3(-1.0, -1.0, 1.0));
	EndPrimitive();
}