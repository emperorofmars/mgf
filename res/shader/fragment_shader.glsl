#version 330 core
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 0) uniform sampler2D lights;
layout (binding = 1) uniform sampler2D tex;

out vec4 color;

struct mMaterial{
	vec4 color;
	float alpha;
	float has_texture;
	float numlights;
};

in VS_OUT{
	vec3 uv;
	vec3 norm;
	vec4 pos;
	mMaterial material;
}fs_in;

void main(void){
	color = vec4(0.5, 0.5, 0.5, 1);
	if(fs_in.material.has_texture > 0.5){
		//color = texture(tex, vec2(fs_in.uv.x, 1 - fs_in.uv.y));
		//color = texture(lights, vec2(0, 1));
	}
	else{
		//color = fs_in.material.color;
	}
	color = texelFetch(lights, ivec2(0, 1), 0);
	//color = texture(lights, vec2(0.25, 0.25));
	//color *= dot(fs_in.norm, normalize(vec3(10, 20, 7) - fs_in.pos.xyz)) *
	//			1 / length(vec3(10, 20, 7) - fs_in.pos.xyz) * 15;
	
	//color = fs_in.material.color;
}
