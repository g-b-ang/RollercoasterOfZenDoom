#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;

uniform mat4 projection;
uniform mat4 modelview;
uniform vec3 ucolor;

out vec3 FragPos;
out vec3 fragNorm;
out vec3 uColor;

void main()
{
	
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * modelview * vec4(position, 1.0);
	vec3 nvec = mat3(transpose(inverse(modelview))) * normals;
	FragPos = vec3(modelview * vec4(position,1.0));
	fragNorm = nvec;
	uColor = ucolor;
}
