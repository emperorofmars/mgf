#version 430 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec3 uv;

out VS_OUT{
	vec3 uv;
}vs_out;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout(binding = 0) uniform atomic_uint count;

void main(void){
	//memoryBarrierAtomicCounter();	//makes it go slow
	gl_Position = proj_matrix * mv_matrix * pos;
	//vs_out.color = sin(pos * 4.0) + vec4(0.5, 0.5, 0.5, 0.0);
	//atomicCounterIncrement(count);	//makes it go slow
	vs_out.uv = uv;
}