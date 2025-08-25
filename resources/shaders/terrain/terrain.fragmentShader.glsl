#version 330

uniform sampler2D blendMap;
uniform sampler2D textureR;
uniform sampler2D textureG;
uniform sampler2D textureB;
uniform sampler2D background;

struct TerrainMatrices {
	mat4 model;
	mat4 pvm;
	mat3 normal;
};
uniform TerrainMatrices matrices;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material = Material(vec3(0.2, 0.2, 0.2), vec3(1, 1, 1), vec3(1, 1, 1), 32);

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light = Light(vec3(0, 0, 0), vec3(1, 1, 1), vec3(0, 0, 0), vec3(0, 0, 0));


uniform vec3 viewPosition;

in TerrainVertexData {
	vec3 normal;
	vec3 color;
	vec2 textureCoordinate;
	vec4 worldPosition;
} inputData;


out vec4 fragmentColor;

void main()
{
	/**
	 * Lighting
	 */
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

	/**
	 * Multitexturing
	 */

	vec4 blendMapColor = texture(blendMap, inputData.textureCoordinate);

	float backAmount = 1.0f - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords = inputData.textureCoordinate * 30.0;

	vec4 backgroundColor = texture(background, tiledCoords) * backAmount;
	vec4 textureRColor = texture(textureR, tiledCoords) * blendMapColor.r;
	vec4 textureGColor = texture(textureG, tiledCoords) * blendMapColor.g;
	vec4 textureBColor = texture(textureB, tiledCoords) * blendMapColor.b;

	fragmentColor = backgroundColor + textureRColor + textureGColor + textureBColor + vec4(inputData.color + ambient + diffuse + specular, 1.0);
}
