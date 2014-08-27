#version 430 core

layout (binding = 0) uniform sampler2D tex;

out vec4 color;

struct mMaterial{
	vec3 color;
	float alpha;
	float has_texture;
};

in VS_OUT{
	vec3 uv;
	mMaterial material;
}fs_in;

void main(void){
	//color = fs_in.uv;
	//color = texelFetch(tex, ivec2(gl_FragCoord.xy), 0);
	if(fs_in.material.has_texture > 0.5){
		color = texture(tex, vec2(fs_in.uv.x, 1 - fs_in.uv.y));
	}
	else{
		color = vec4(fs_in.material.color, fs_in.material.alpha);
	}
	//color = vec4(fs_in.material.color, 1);
	//color = texture(tex, vec2(fs_in.uv.x, 1 - fs_in.uv.y));
	//color = vec4(1, 0.5, 0, 1);
}
