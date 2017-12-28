#version 450 core 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
out vec3 colourPixel;
out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform vec3 colour;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	fragPos = vec3(model *  vec4(aPos, 1.0)); //Worldspace coordinate
	colourPixel = colour;
	texCoord = aTexCoord;
	normal = mat3(transpose(inverse(model))) * aNormal; //Prevents scaling from effecting the normal.
}