#version 330

layout (location = 0) in vec3 position;
layout (location = 3) in vec2 textureCoordinate;

out vec2 textureCoordinates;


struct Matrices {
    mat4 model;
    mat4 projectionView;
};

uniform Matrices matrices;


void main()
{
    gl_Position = matrices.projectionView * matrices.model * vec4(position, 1.0f);
    textureCoordinates = textureCoordinate;
}
