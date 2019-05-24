#version 330 core

in vec4 vertexColor;
in vec3 fragNorm;
in vec3 FragPos;
in mat4 modelview;
in vec3 uColor;

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

uniform Light light;
uniform Material material;

uniform int lightMode;
uniform int oMode;
uniform vec3 ucolor;

uniform samplerCube skybox;
uniform vec3 cameraPos;
uniform int rMode;


void main()
{
	vec3 tlPos = vec3(light.position.x, light.position.y, light.position.z - 20);
	// vec3 tlPos = light.position;

	vec3 objectColor = vec3(0.31, 0.16, 0.23);

	vec3 norm = normalize(fragNorm);

	vec3 lightDir = vec3(1.0);

	float attenuation = 1.0;

	float theta = 1;

	vec3 I = normalize(FragPos - cameraPos);
    vec3 R = reflect(I, normalize(fragNorm));

	if (lightMode == 1){
		lightDir = normalize(-light.direction);

	}
	if (lightMode == 2){
		lightDir = normalize( tlPos - FragPos );
		
		// Calculate Linear Attenuation
		float distance = length(tlPos - FragPos);
		attenuation = 2.0 / (1.0 + light.linAtten * distance);
	}
	if (lightMode == 3){
		lightDir = normalize( tlPos - FragPos );
		theta = dot(lightDir, normalize(-light.direction));

		// Calculate Quadratic Attenuation
		float distance = length(tlPos - FragPos);
		attenuation = 2.0 / (1.0 + light.linAtten * distance + light.quadAtten * (distance * distance));

	}

	// Calculate ambient

	vec3 ambient = material.ambient * light.color;

	// Calculate diffuse

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.color * material.diffuse;

	// Calculate specular

	vec3 viewPos = vec3(0.0, 0.0, 20.0);

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular * spec * light.color;

	vec3 result = attenuation * (ambient + diffuse + specular) * material.color;

	color = vec4(result, 1.0);

	
	if (lightMode == 3){
		if (theta > light.cutoff){
			float intensity = clamp((theta - light.ocutoff) / (light.cutoff - light.ocutoff), 0.0, 1.0);
			// intensity = dot()

			result = vec3(attenuation *(ambient + diffuse*intensity + specular * intensity) * material.color);

			//result = vec3(result.x * intensity, result.y * intensity, result.z * intensity);
			
			color = vec4(result, 1.0);
		} else {
			color = vec4(vec3(0.0) , 1.0);
		}
	}

    // Color based on normal if rMode == 1, based on Phuong if else

	color = vec4(uColor, 1.0);
	if (rMode == 1){
		color = vec4(texture(skybox, R).rgb, 1.0);
	}
	

}
