#version 450 core 
in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;
out vec4 FragColor;

struct Material {
	sampler2D diffuse; //textures
	sampler2D specular; //specular textures
	float shininess; //radius of specular highlighting.
};

struct Light { //For a spot light.
	vec3 direction;
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float cutoff;
	float outcutoff;
	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
	//vec3 lightDir = normalize(-light.direction); //Direction of light source. DIRECTIONAL
	vec3 lightDir = normalize(light.position - fragPos); //POINT LIGHT
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutoff - light.outcutoff;
	float intensity = clamp((theta - light.outcutoff) / epsilon, 0.0, 1.0);
	float dist = length(light.position - fragPos);
	float attenuation = 5.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));
	ambient *= attenuation * intensity;
	vec3 norm = normalize(normal);
	float diffuseAmount = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diffuseAmount * vec3(texture(material.diffuse, texCoord));
	diffuse *= attenuation * intensity;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectionDir = reflect(-lightDir, norm);
	//lightDir is negative as it represents fragment to light. we want light to the normal / fragment.
	float spec = pow(max(dot(viewDir, reflectionDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));
	specular *= attenuation * intensity;
	FragColor = vec4((ambient + diffuse + specular), 1.0f);
}