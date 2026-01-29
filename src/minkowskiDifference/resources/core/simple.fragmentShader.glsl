#version 330

uniform sampler2D textureUnit;

struct Matrices {
	mat4 model;
	mat4 pvm;
	mat3 normal;
};
uniform Matrices matrices;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float alpha;
    float shininess;
};
uniform Material material = Material(vec3(0.3, 0.3, 0.3), vec3(0.3, 0.3, 0.3), vec3(0.3, 0.3, 0.3), 1.0, 32);

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light = Light(vec3(0, 0, 0), vec3(1, 1, 1), vec3(1, 1, 1), vec3(1, 1, 1));


uniform vec3 viewPosition;

in VertexData {
	vec3 normal;
	vec3 color;
	vec2 textureCoordinate;
	vec4 worldPosition;
} inputData;


out vec4 fragmentColor;

void main()
{
	// ambient
	vec3 ambient = light.ambient * material.ambient;

	// diffuse
	vec3 norm = normalize(inputData.normal);
	vec3 lightDir = normalize(light.position - vec3(inputData.worldPosition));
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * material.diffuse;

	// specular
	vec3 viewDir = normalize(viewPosition - vec3(inputData.worldPosition));
	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * light.specular * material.specular;


	//fragmentColor = vec4(ambient + diffuse + specular, 1.0) * texture(textureUnit, inputData.textureCoordinate);
	 //fragmentColor = vec4(inputData.color, 1.0) * vec4(ambient + diffuse + specular, 1.0) * texture(textureUnit, inputData.textureCoordinate);
	fragmentColor = vec4(inputData.color + ambient + diffuse + specular, material.alpha) * texture(textureUnit, inputData.textureCoordinate);
}
