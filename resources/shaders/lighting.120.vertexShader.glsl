#version 140 compatibility

attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 textureCoordinate;
attribute vec3 color;

varying vec4 diffuse, ambient, emission;
varying vec3 normalVector, halfVector;

void main() {
	normalVector = normalize(gl_NormalMatrix  * normal);

	//output variables
	//gl_FrontColor =  ambient + emission + diffuse + specular;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex, 1.0); //ftransform();
    gl_TexCoord[0] = vec4(textureCoordinate, 1.0, 1.0);
}