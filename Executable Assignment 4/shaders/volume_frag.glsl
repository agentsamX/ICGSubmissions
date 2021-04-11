#version 420 

layout(binding = 0) uniform sampler2D s_albedoTex;
layout(binding = 1) uniform sampler2D s_normalsTex;
layout(binding = 2) uniform sampler2D s_specularTex;

layout(binding = 3) uniform sampler2D s_positionTex;

layout(binding = 4) uniform sampler2D s_lightAccumTex;

uniform vec4 u_LightColour;
uniform vec4 u_LightPos;
uniform mat4 u_LightSpaceMatrix;
uniform vec4 u_CamPos;
uniform float u_LightStrength;

out vec4 frag_color;


void main()
{
	float SpecularLightStrength=1.0f;
    //albedo
    vec4 textureColor=texture(s_albedoTex,vec2(gl_FragCoord.x/1280.f,gl_FragCoord.y/720.f));
    //normals
    vec3 inNormal = (normalize(texture(s_normalsTex,vec2(gl_FragCoord.x/1280.f,gl_FragCoord.y/720.f)).rgb)*2.0)-1.0;
    //spec
    //float texSpec= texture(s_specularTex,vec2(gl_FragCoord.x/1280.f,gl_FragCoord.y/720.f)).r;
    //positions
    vec3 fragPos = texture(s_positionTex,vec2(gl_FragCoord.x/1280.f,gl_FragCoord.y/720.f)).rgb;

   
    // Diffuse
	vec3 N = normalize(inNormal);
	vec3 lightDir = normalize(u_LightPos.xyz-fragPos);
	float dif = max(dot(N, lightDir), 0.0);
	vec3 diffuse = dif * u_LightColour.xyz;// add diffuse intensity
	diffuse= diffuse/length(u_LightPos.xyz-fragPos);

	// Specular
	vec3 viewDir  = normalize(u_CamPos.xyz - fragPos);
	vec3 h        = normalize(lightDir + viewDir);

	// Get the specular power from the specular map
	float spec = pow(max(dot(N, h), 0.0), 4.0); // Shininess coefficient (can be a uniform)
	vec3 specular = SpecularLightStrength * spec * u_LightColour.xyz; // Can also use a specular color

    vec4 fragPosLightSpace = u_LightSpaceMatrix * vec4(fragPos,1.0);

	vec3 result = ( u_LightStrength*// global ambient light //Shadow value
		(diffuse + specular) // light factors from our single light
		); // Object color

    if(textureColor.a<0.31)
    {
        result=vec3(1.0,1.0,1.0);
    }

	
	frag_color =(texture(s_lightAccumTex,vec2(gl_FragCoord.x/1280.f,gl_FragCoord.y/720.f))+vec4(result, textureColor.a));
}