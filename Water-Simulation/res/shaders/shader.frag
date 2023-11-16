#version 330 core

in vec3 normal;
in vec3 fragPos;
uniform vec3 lightPos;

void main()
{
	vec3 col = vec3(1.0, 0.5, 0.0);

	vec3 ambient = col * 0.1;

	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = col * diff;

	vec3 color = ambient + diffuse;
	// Outputting an orange color
	gl_FragColor = vec4(color, 1.0);
}