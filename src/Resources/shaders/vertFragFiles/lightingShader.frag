#version 430 core

struct Material
{
	sampler2D diffuse;
	float shininess;
};

struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

varying vec2 ex_TexCoord;
varying vec3 ex_FragPos;
varying vec3 ex_Normal;

uniform vec3 in_ViewPos;
uniform DirLight in_DirLight;

uniform int in_NoPointLights;
uniform int in_NoSpotLights;

uniform PointLight in_PointLights[50];
uniform SpotLight in_SpotLights[50];

uniform Material in_Material;

vec3 CalcDirLight(DirLight _light, vec3 _normal, vec3 _viewDir, vec3 diffuseTex, vec3 specularTex);
vec3 CalcPointLight(PointLight light, vec3 _normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex);

void main()
{
	vec3 norm = normalize(ex_Normal);
	vec3 viewDir = normalize(in_ViewPos - ex_FragPos);

	//Pre-calculates the texture values for efficiency
	vec3 diffuseTex = vec3(texture(in_Material.diffuse, ex_TexCoord));
	vec3 specularTex = vec3(1.0, 1.0, 1.0);

	vec3 result = CalcDirLight(in_DirLight, norm, viewDir, diffuseTex, specularTex);
	for(int i = 0; i < in_NoPointLights; i++)
	{
		result += CalcPointLight(in_PointLights[i], norm, ex_FragPos, viewDir, diffuseTex, specularTex); 
	}
	for (int i = 0; i < in_NoSpotLights; i++)
	{
		result += CalcSpotLight(in_SpotLights[i], norm, ex_FragPos, viewDir, diffuseTex, specularTex);
	}

	gl_FragColor = vec4(result, 1);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight _light, vec3 _normal, vec3 _viewDir, vec3 diffuseTex, vec3 specularTex)
{
	vec3 lightDir = normalize(-_light.direction);
	// Diffuse shading
	float diff = max(dot(_normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, _normal);
	float spec = pow(max(dot(_viewDir, reflectDir), 0.0), in_Material.shininess);
	// combine results
	vec3 ambient = _light.ambient * diffuseTex;
	vec3 diffuse = _light.diffuse * diff * diffuseTex;
	vec3 specular = _light.specular * spec * specularTex;
	
	return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), in_Material.shininess);
	// attenuation
	float distance    = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
								light.quadratic * (distance * distance));    
	// combine results
	vec3 ambient  = light.ambient  * diffuseTex;
	vec3 diffuse  = light.diffuse  * diff * diffuseTex;
	vec3 specular = light.specular * spec * specularTex;
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), in_Material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * diffuseTex;
    vec3 diffuse = light.diffuse * diff * diffuseTex;
    vec3 specular = light.specular * spec * specularTex;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}