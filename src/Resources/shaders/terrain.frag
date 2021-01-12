#version 430 core

out vec4 FragColor;

struct Material
{
	sampler2D blendMap;
	sampler2D bg_diffuse;
	sampler2D r_diffuse;
	sampler2D g_diffuse;
	sampler2D b_diffuse;
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

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
} fs_in;

uniform vec3 in_ViewPos;
uniform DirLight in_DirLight;

const float in_ShadowDistance = 50.0;
const float in_TransitionDistance = 8.0;

const int pcfCount = 2;
const float totalTexels = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);

uniform int in_NoPointLights;
uniform int in_NoSpotLights;

uniform PointLight in_PointLights[50];
uniform SpotLight in_SpotLights[50];

uniform Material in_Material;
uniform sampler2D in_ShadowMap;

vec3 CalcDiffuse();
vec3 CalcDirLight(DirLight _light, vec3 _normal, vec3 _viewDir, vec3 diffuseTex, vec3 specularTex);
vec3 CalcPointLight(PointLight light, vec3 _normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex);
float PCFCalculation(vec3 normal, vec3 lightDir);

void main()
{
	vec3 norm = normalize(fs_in.Normal);
	vec3 viewDir = normalize(in_ViewPos - fs_in.FragPos);

	//Pre-calculates the texture values
	vec3 diffuseTex = CalcDiffuse();
	
	//hmm, I should change this
	vec3 specularTex = vec3(0.2, 0.2, 0.2);





	vec3 result = CalcDirLight(in_DirLight, norm, viewDir, diffuseTex, specularTex);
	for(int i = 0; i < in_NoPointLights; i++)
	{
		result += CalcPointLight(in_PointLights[i], norm, fs_in.FragPos, viewDir, diffuseTex, specularTex); 
	}
	for (int i = 0; i < in_NoSpotLights; i++)
	{
		result += CalcSpotLight(in_SpotLights[i], norm, fs_in.FragPos, viewDir, diffuseTex, specularTex);
	}

	FragColor = vec4(result, 1);
}

//Calculates the diffuse fragment colour to be used 
vec3 CalcDiffuse()
{
	vec4 blendMapColor = texture(in_Material.blendMap, fs_in.TexCoord);
	vec2 tiledCoords = fs_in.TexCoord * 40.0;
	float bgTexAmount = 1.0 - (blendMapColor.r + blendMapColor.g, blendMapColor.b);
	vec4 bgTexColor = texture(in_Material.bg_diffuse, tiledCoords) * bgTexAmount;
	vec4 rTexColor = texture(in_Material.r_diffuse, tiledCoords) * blendMapColor.r;
	vec4 gTexColor = texture(in_Material.g_diffuse, tiledCoords) * blendMapColor.g;
	vec4 bTexColor = texture(in_Material.b_diffuse, tiledCoords) * blendMapColor.b;
	
	vec4 totalColor = bgTexColor + rTexColor + gTexColor + bTexColor;
	return vec3(totalColor);
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
	
	float shadow = PCFCalculation(_normal, lightDir);
	return (ambient + (1.0 - shadow) * (diffuse + specular));
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

float PCFCalculation(vec3 normal, vec3 lightDir)
{
	// perform perspective divide
    vec3 projCoords = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;
	// transforms coordinates to the range [0,1]
	projCoords = projCoords * 0.5 + 0.5;
	float bias = 0.005;
	//float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); 

	float mapSize = 2048.0;
	float texelSize = 1.0 / mapSize;
	float total = 0.0;
	for(int x = -pcfCount; x <= pcfCount; x++)
	{
		for(int y = -pcfCount; y <= pcfCount; y++)
		{
			float nearestDepth = texture(in_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			if(projCoords.z - bias > nearestDepth)
			{
				if(projCoords.z > 1.0 && nearestDepth == 1.0)
				{}
				else
				{
					total += 1.0;
				}
			}
		}
	}
	total /= totalTexels;

	//shadow distance fading
	float distance = length((fs_in.FragPos - in_ViewPos).xyz);
	distance = distance - (in_ShadowDistance - in_TransitionDistance);
	distance = distance / in_TransitionDistance;
	if(total > 0.0)
	{
		distance = clamp(1.0 - distance, 0.0, 1.0);
		total *= distance;
	}
	return total;
}