#version 330

struct Matrices {
	mat4 model;
	mat4 pvm;
	mat3 normal;
};
uniform Matrices matrices;

in vec3 position;
in vec3 normal;
in vec2 textureCoordinate;
in vec3 color;

out VertexData {
	vec3 normal;
	vec3 color;
	vec2 textureCoordinate;
	vec4 worldPosition;
} outputData;

void main()
{
    gl_Position = matrices.pvm * vec4(position, 1.0f);
    outputData.worldPosition = matrices.model * vec4(position, 1.0f);
    outputData.normal = normalize(matrices.normal * normal);
    outputData.textureCoordinate =  textureCoordinate; //vec4(textureCoordinate, 1.0, 1.0);
    outputData.color = color;
}
