#version 330 core

in vec3 normal;
in vec3 fragPos;
uniform vec3 lightPos;
uniform vec3 surfaceColor;
uniform vec3 viewPos;
uniform samplerCube cubemap;

void main()
{
	vec3 col = surfaceColor;

	vec3 ambient = col * 0.1;

	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = col * diff;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = spec * vec3(1.0);

	vec3 color = ambient + diffuse + specular;
	// Outputting an orange color
	gl_FragColor = vec4(color, 1.0);
}