#version 450 core 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
out vec3 colourPixel;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	colourPixel = aColour;
}