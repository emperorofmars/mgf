#version 430 core

in vec4 pos;

out VS_OUT{
	vec4 color;
}vs_out;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout(binding = 0) uniform atomic_uint count;

void main(void){
	memoryBarrierAtomicCounter();	//makes it go slow
	gl_Position = proj_matrix * mv_matrix * pos;
	vs_out.color = sin(pos * 4.0) + vec4(0.5, 0.5, 0.5, 0.0);
	atomicCounterIncrement(count);	//makes it go slow
}