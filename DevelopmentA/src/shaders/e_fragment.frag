#version 450 core 
in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;
out vec4 FragColor;

struct Material {
	sampler2D tDiffuse0; //textures
	sampler2D tSpecular0; //specular textures
	float shininess; //radius of specular highlighting.
};

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 frag_pos, vec3 viewDir);

#define NB_POINT_LIGHTS 100
uniform Material material;
uniform PointLight l_points[NB_POINT_LIGHTS];
uniform DirLight l_directional;
uniform vec3 viewPos;

void main()
{
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 result = CalcDirLight(l_directional, norm, viewDir);
    for(int i = 0; i < NB_POINT_LIGHTS; i++)
		if (l_points[i].constant > 0) {
			result += CalcPointLight(l_points[i], norm, fragPos, viewDir);  
		}
	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3  l_dir = normalize(-light.direction);
	float diffuse = max(dot(normal, l_dir), 0.0);
	vec3 reflectDir = reflect(-l_dir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	vec3 ambient = light.ambient * vec3(texture(material.tDiffuse0, texCoord));
	vec3 diffusion = light.diffuse * diffuse * vec3(texture(material.tDiffuse0, texCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.tSpecular0, texCoord));
	return (ambient + diffusion + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 frag_pos, vec3 viewDir) {
	vec3 l_dir = normalize(light.position - frag_pos);
	float diff = max(dot(normal, l_dir), 0.0);
	vec3 reflectDir = reflect(-l_dir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	float dist = length(light.position - frag_pos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 ambient = light.ambient * vec3(texture(material.tDiffuse0, texCoord));
	vec3 diffusion = light.diffuse * diff * vec3(texture(material.tDiffuse0, texCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.tSpecular0, texCoord));
	ambient *= attenuation;
	diffusion *= attenuation;
	specular *= attenuation;
	return (ambient + diffusion + specular);
}