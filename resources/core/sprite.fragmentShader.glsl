#version 330

in vec2 textureCoordinates;
out vec4 fragmentColor;
 
uniform sampler2D image;
uniform vec3 color = vec3(1.0, 1.0, 1.0);

void main()
{
	fragmentColor = vec4(color, 1.0) * texture(image, textureCoordinates);
}