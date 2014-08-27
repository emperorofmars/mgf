#version 430 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec3 uv;

struct mMaterial{
	vec3 color;
	float alpha;
	float has_texture;
};

out VS_OUT{
	vec3 uv;
	mMaterial material;
}vs_out;

uniform mat4 model_mat;
uniform mat4 proj_matrix;
uniform mMaterial material;

void main(void){
	//memoryBarrierAtomicCounter();	//makes it go slow
	gl_Position = proj_matrix * model_mat * pos;
	//vs_out.color = sin(pos * 4.0) + vec4(0.5, 0.5, 0.5, 0.0);
	//atomicCounterIncrement(count);	//makes it go slow
	vs_out.uv = uv;
	vs_out.material = material;
}