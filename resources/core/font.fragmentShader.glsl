#version 330

in vec2 textureCoordinates;
out vec4 fragmentColor;
 
uniform sampler2D image;
uniform vec3 color = vec3(1.0, 1.0, 1.0);

const float width = 0.45;
const float edge = 0.07;

void main()
{
  float sdfValue = texture(image, textureCoordinates).r;

  float distance = 1.0 - sdfValue;
  float alpha = 1.0 - smoothstep(width, width + edge, distance);
  
  if(alpha < 0.1) { //Full transparency
     discard;
  }
  
  vec4 textureColor = vec4(1.0, 1.0, 1.0, sdfValue);
  
  fragmentColor = vec4(color, alpha);  
  
  //if(sdfValue < 0.45) {
  //  discard;
  //}
  
  //fragmentColor = vec4(color, sdfValue);
}