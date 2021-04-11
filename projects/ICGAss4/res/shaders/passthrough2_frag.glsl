#version 420

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 frag_color;
layout(location = 1) out vec4 frag_color2;


layout (binding = 0) uniform sampler2D s_screenTex;
uniform float u_Transparency = 1.0f;

void main() 
{
	vec4 source = texture(s_screenTex, inUV);

	frag_color2.rgb = source.rgb;
	frag_color2.a = source.a * u_Transparency;
}