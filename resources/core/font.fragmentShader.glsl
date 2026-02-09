#version 330

in vec2 textureCoordinates;
out vec4 fragmentColor;
 
uniform sampler2D image;
uniform vec3 color = vec3(1.0, 1.0, 1.0);

void main()
{
  vec4 textureColor = vec4(1.0, 1.0, 1.0, texture(image, textureCoordinates).r);
  if(textureColor.a < 0.1) { //Full transparency
     discard;
  }

  fragmentColor = vec4(color, 1.0) * textureColor;

}