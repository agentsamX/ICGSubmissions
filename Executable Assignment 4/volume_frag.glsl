#version 420 

layout(binding = 0) uniform sampler2D s_albedoTex;
layout(binding = 1) uniform sampler2D s_normalsTex;
layout(binding = 2) uniform sampler2D s_specularTex;
layout(binding = 3) uniform sampler2D s_positionTex;

layout(binding = 4) uniform sampler2D s_lightAccumTex;

uniform vec3 u_LightColour;
uniform vec3 u_LightPos
uniform mat4 u_LightSpaceMatrix;
uniform vec3 u_CamPos;

out vec4 frag_color;


void main()
{

    //albedo
    vec4 textureColor=texture(s_albedoTex,inUV);
    //normals
    vec3 inNormal = (normalize(texture(s_normalsTex,inUV).rgb)*2.0)-1.0;
    //spec
    float texSpec= texture(s_specularTex,inUV).r;
    //positions
    vec3 fragPos = texture(s_positionTex,inUV).rgb;

   
    // Diffuse
	vec3 N = normalize(inNormal);
	vec3 lightDir = normalize(u_LightPos-fragPos);
	float dif = max(dot(N, lightDir), 0.0);
	vec3 diffuse = dif * u_LightColour;// add diffuse intensity

	// Specular
	vec3 viewDir  = normalize(u_CamPos - fragPos);
	vec3 h        = normalize(lightDir + viewDir);

	// Get the specular power from the specular map
	float spec = pow(max(dot(N, h), 0.0), 4.0); // Shininess coefficient (can be a uniform)
	vec3 specular = texSpec * spec * u_LightColour; // Can also use a specular color

    vec4 fragPosLightSpace = u_LightSpaceMatrix * vec4(fragPos,1.0);

	vec3 result = (
		(sun._ambientPow * sun._ambientCol.xyz) + // global ambient light //Shadow value
		(diffuse + specular) // light factors from our single light
		); // Object color

    if(textureColor.a<0.31)
    {
        result=vec3(1.0,1.0,1.0);
    }

	frag_color = vec4(result, textureColor.a);
}