#version 330 core

uniform samplerCube cubemap;
in vec3 TexCoords;

void main()
{
	gl_FragColor = textureCube(cubemap, TexCoords);
}