#version 420

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outColor;
layout(location = 2) out vec3 outNormal;

uniform mat4 u_ModelViewProjection;
uniform mat4 u_Model;
uniform mat3 u_ModelRotation;


void main() {

	gl_Position = u_ModelViewProjection * vec4(inPosition, 1.0);

	// Lecture 5
	// Pass vertex pos in world space to frag shader
	outPos = (u_Model * vec4(inPosition, 1.0)).xyz;

	// Normals
	outNormal = u_ModelRotation * inNormal;

	///////////
	outColor = vec3(0.5,0.5,0.5);
}
