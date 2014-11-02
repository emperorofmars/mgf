#version 430 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 norm;
layout (location = 2) in vec3 uv;

struct mMaterial{
	vec4 color;
	float alpha;
	float has_texture;
};

out VS_OUT{
	vec3 uv;
	mMaterial material;
}vs_out;

uniform mat4 m_mat;
uniform mat4 vp_mat;
uniform mMaterial material;

void main(void){
	gl_Position = vp_mat * m_mat * pos;
	vec4 m_norm = m_mat * norm;
	
	vs_out.uv = uv;
	vs_out.material = material;
	vs_out.material.color *= dot(m_norm, normalize(vec4(2, 3, 1, 1) - m_mat * pos)) *
						1 / length(vec4(2, 3, 1, 1) - m_mat * pos) * 10;
}