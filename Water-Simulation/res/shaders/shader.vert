#version 330 core

#define EULERS_NUM 2.71828
#define FREQUENCY(wavelength) (2.0 / (wavelength))
#define WAVE_COUNT 6.0

layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform float time;

void main()
{
	vec3 worldPos = vec3(model * vec4(aPos, 1.0));
	float posY = 0.0;
	for (float i = 1.0; i < WAVE_COUNT; i++)
	{
		vec2 r;
		if (int(i) % 2 == 1)
		{
			r = vec2(1.0, i);
		}
		else
		{
			r = vec2(i, 1.0);
		}
		float f = worldPos.x * r.x + worldPos.z * r.y;
		float freq = FREQUENCY(1.0 - (i / WAVE_COUNT));
		posY += 0.05 * (WAVE_COUNT / i) * pow(EULERS_NUM, sin(freq * time + f) - 1.0);
	}
	vec4 Pos = vec4(aPos.x, posY, aPos.z, 1.0);
	gl_Position = projection * view * model * Pos;
}
