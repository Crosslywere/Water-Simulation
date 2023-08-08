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
	float yoffset = 0.0;
	vec2 rot = vec2(1.0);
	for (float i = 1.0; i <= 5.0; i++)
	{
		rot = vec2(1.0, i);
		float posOffset = rot.x * worldPos.x + rot.y * worldPos.z;
		float frequency = FREQUENCY(i / 5.0);
		yoffset += 0.3 * (1.0 / i) * sin((frequency * (aPos.y + time)) + posOffset);
	}

	// The new vertex position after the offset has been calculated
	vec3 newPos = vec3(aPos.x, yoffset, aPos.z);

	gl_Position = projection * view * model * vec4(newPos, 1.0);
}
