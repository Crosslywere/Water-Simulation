#version 330 core

#define FREQUENCY(wavelength) 2.0 / (wavelength)

layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform float time;

void main()
{
	vec4 worldPos = model * vec4(aPos, 1.0);
	float posOffset = worldPos.x + worldPos.z;

	float freq = FREQUENCY(0.5);

	// The offset for the y-axis calculated for each vertex
	float yOffset = 0.25 * sin((aPos.y * freq + time * freq) + posOffset);

	// The new vertex position after the offset has been calculated
	vec3 newPos = vec3(aPos.x, yOffset, aPos.z);

	gl_Position = projection * view * model * vec4(newPos, 1.0);
}
