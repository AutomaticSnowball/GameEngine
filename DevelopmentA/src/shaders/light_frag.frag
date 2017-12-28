#version 450 core
out vec4 FragColor;
uniform vec3 light_colour;

void main()
{
	FragColor = vec4(light_colour.xyz, 1.0f);
}