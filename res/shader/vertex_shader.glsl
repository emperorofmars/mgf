#version 430 core

in vec4 pos;

out VS_OUT{
	vec4 color;
}vs_out;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void){
	gl_Position = proj_matrix * mv_matrix * pos;
	vs_out.color = pos * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
}