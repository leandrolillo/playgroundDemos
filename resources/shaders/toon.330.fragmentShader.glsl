//
// toon.frag -- from lighthouse3d.com
//
#version 330

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;


in VertexData {
	vec3 normal;
	vec3 color;
	vec2 textureCoordinate;
	vec4 worldPosition;
} inputData;

out vec4 fragmentColor;

void main (void)
{
  vec4 color;
  vec3 n = normalize (inputData.normal);
  vec3 lightDir = normalize(light.position - vec3(inputData.worldPosition));

  float intensity = dot (lightDir, n);

  if (intensity > 0.95)
    color = vec4 (1.0, 0.5, 0.5, 1.0);
  else if (intensity > 0.5)
    color = vec4 (0.6, 0.3, 0.3, 1.0);
  else if( intensity > 0.25 )
    color = vec4 (0.4, 0.2, 0.2, 1.0);
  else
    color = vec4 (0.2, 0.1, 0.1, 1.0);

  fragmentColor = color;
}
