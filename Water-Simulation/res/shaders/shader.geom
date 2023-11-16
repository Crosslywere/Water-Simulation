#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 model;
out vec3 fragPos;
out vec3 normal;

void main()
{
	// Tangent
	vec3 T = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	// Binormal
	vec3 B = vec3(gl_in[0].gl_Position) - vec3(gl_in[2].gl_Position);
	for (int i = 0; i < 4; i++) {
		gl_Position = gl_in[i % 3].gl_Position;
		normal = normalize(cross(B, T));
		fragPos = vec3(transpose(inverse(model)) * gl_Position);
		EmitVertex();
	}
}