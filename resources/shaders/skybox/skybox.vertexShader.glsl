#version 330

struct SkyBoxMatrices {
	mat4 p;
	mat4 v;
};
uniform SkyBoxMatrices matrices;

uniform float boxSize = 300;

in vec3 position;

out SkyBoxVertexData {
	vec3 textureCoordinate;
} outputData;

void main()
{
    gl_Position = matrices.p * vec4(mat3(matrices.v) * (position * boxSize), 1.0f);
    outputData.textureCoordinate =  position;
}
