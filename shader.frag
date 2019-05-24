#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 TexCoords;
in vec3 Normal;
in vec3 Position;
in vec3 aNormal;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec3 color;
};

struct Light{
	vec3 color;
	vec3 direction;
	vec3 position;
	float cutoff, exponent, linAtten, quadAtten, ocutoff;
};

uniform samplerCube skybox;
uniform vec3 cameraPos;
uniform int rMode;
void main()
{
	vec3 I = normalize(Position - cameraPos);
	vec3 R = reflect(I, normalize(aNormal));
	color = texture(skybox, TexCoords);
	if ( rMode == 1){
		color = vec4(texture(skybox,R).rgb, 1.0);
		color = vec4(cameraPos,1.0);
	}
}
