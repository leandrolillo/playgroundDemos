#version 330

layout (location = 0) in vec3 vertex;

out vec2 textureCoordinates;


mat4 projectionView;

void main()
{
    gl_Position = matrices.projectionView * vec4(position.xy, 0.0f, 1.0f);
    textureCoordinates = position.xy;
}
