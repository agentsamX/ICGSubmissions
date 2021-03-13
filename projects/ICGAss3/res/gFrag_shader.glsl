#version 420

layout(location = 0) in vec3 inPos;
layout(location = 3) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
//LECTURE 7
layout(location = 1) in vec2 UV;

uniform sampler2D textureSampler;

layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 pos_color;
layout (location = 2) out vec4 norm_color;




void main() {
		

	frag_color = texture(textureSampler,UV);
	pos_color = vec4(inPos,1.0);
	norm_color = vec4((normalize(inNormal)*0.5)+0.5,1.0);
	// no clue why it looks so much better when i negativeize it
}