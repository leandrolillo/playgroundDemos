#version 330

uniform samplerCube textureUnit;
//
in SkyBoxVertexData {
	vec3 textureCoordinate;
} inputData;

out vec4 fragmentColor;

void main()
{
	//fragmentColor = vec4(inputData.textureCoordinate, 1.0);
	fragmentColor = texture(textureUnit, inputData.textureCoordinate);
}
