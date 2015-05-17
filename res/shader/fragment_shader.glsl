#version 330 core
#extension GL_ARB_shading_language_420pack : enable

uniform sampler2D lights;
uniform sampler2D tex;

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
	if(fs_in.material.has_texture > 0.5){
		color = texture(tex, vec2(fs_in.uv.x, 1 - fs_in.uv.y));
	}
	else{
		color = fs_in.material.color;
	}
	color = texelFetch(lights, ivec2(1, 7), 0);
	//color = texture(lights, vec2(0.25, 0.25));
	//color *= dot(fs_in.norm, normalize(vec3(10, 20, 7) - fs_in.pos.xyz)) *
	//			1 / length(vec3(10, 20, 7) - fs_in.pos.xyz) * 15;
	//color.a = fs_in.material.alpha;
	//color = fs_in.material.color;
}
