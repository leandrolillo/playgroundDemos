#version 330

layout (location = 0) in vec4 vertex;
out vec2 textureCoordinates;


uniform mat4 projectionView;

void main()
{
    gl_Position = projectionView * vec4(vertex.xy, 0.0, 1.0);
    textureCoordinates = vertex.zw;
}
