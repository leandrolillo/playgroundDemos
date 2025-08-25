#version 140 compatibility

uniform vec3 vertex;
uniform vec3 normal;
uniform vec2 textureCoordinate;
uniform vec3 color;

//varying vec4 diffuse, ambient, emission;
out vec3 normalVector; //, halfVector;

void main() {
	normalVector = normalize(gl_NormalMatrix  * normal);

	//output variables
	//gl_FrontColor =  ambient + emission + diffuse + specular;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex, 1.0); //ftransform();
    gl_TexCoord[0] = vec4(textureCoordinate, 1.0, 1.0);
}