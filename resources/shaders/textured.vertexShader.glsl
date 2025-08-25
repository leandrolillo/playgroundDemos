#version 120

attribute vec2 vertex;
attribute vec2 textureCoordinate;
attribute vec2 normal;
attribute vec3 color;

void main()
{
   gl_FrontColor = vec4(color, 1.0);
   gl_Position = ftransform();
   gl_TexCoord[0] = vec4(textureCoordinate, 1.0, 1.0);
}