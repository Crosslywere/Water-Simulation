#version 330 core

#define RIGHT vec3(-1.0, 0.0, 0.0)

layout (triangles) in;
layout (triangle_strip, max_vertices = 5) out;

out vec3 normal;

void main()
{
	for (int i = 0; i < 4; i++) {
		gl_Position = gl_in[i].gl_Position;
		normal = cross(RIGHT, cross(vec3(gl_in[(i + 1) % 3].gl_Position), vec3(gl_in[i].gl_Position)));
		EmitVertex();
	}
	EndPrimitive();
}