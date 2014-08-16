#version 430 core

layout (binding = 0) uniform sampler2D tex;

out vec4 color;

in VS_OUT{
	vec3 uv;
}fs_in;

void main(void){
    //color = fs_in.uv;
    //color = texelFetch(tex, ivec2(gl_FragCoord.xy), 0);
	color = texture(tex, vec2(fs_in.uv.x, fs_in.uv.y));
}
