#version 450 core 

const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 3;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in ivec3 aJointIndices;
layout (location = 4) in vec3 aJointWeights;

out vec3 colourPixel;
out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform vec3 colour;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 jointTransforms[MAX_JOINTS];

void main()
{
	vec4 localPos = vec4(0.0);
	vec4 localNormal = vec4(0.0);
	
	for (int i=0; i < 1; i++) {
	mat4 jointTransform;
		if (i == 0) {
			jointTransform = jointTransforms[aJointIndices.x];
		} else if (i == 1) {
			jointTransform = jointTransforms[aJointIndices.y];
		} else if (i == 2) {
			jointTransform = jointTransforms[aJointIndices.z];
		}
		
		vec4 posePos = jointTransform * vec4(aPos, 1.0);
		localPos += posePos * aJointWeights[i];
		
		vec4 worldNormal = jointTransform * vec4(aNormal, 0.0);
		localNormal += worldNormal * aJointWeights[i];

	}
	gl_Position = projection * view * model * localPos;
	fragPos = vec3(model *  vec4(aPos, 1.0)); //Worldspace coordinate
	colourPixel = colour;
	texCoord = aTexCoord;
	normal = mat3(transpose(inverse(model))) * localNormal.xyz; //Prevents scaling from effecting the normal.
}