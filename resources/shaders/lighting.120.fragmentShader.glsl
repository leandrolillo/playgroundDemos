#version 140 compatibility
uniform sampler2D color_texture;

varying vec3 normalVector;

void main(void)
{
	vec3 normal = normalize(normalVector);
	
	float NdotL = max(dot(normal, normalize(vec3(gl_LightSource[0].position))), 0.0);
	float NdotHV = max(dot(normal, normalize(vec3(gl_LightSource[0].halfVector))), 0.0);
	
	vec4 ambient = gl_FrontMaterial.ambient * gl_LightModel.ambient + gl_LightSource[0].ambient * gl_LightModel.ambient;
	vec4 emission = gl_FrontMaterial.emission;
	vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * NdotL;
	float shininess;
	vec4 specular;
	
	if(NdotL != 0.0) {
		shininess = gl_FrontMaterial.shininess;
		specular = gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV, shininess);
	} else {
		specular = vec4(0.0, 0.0, 0.0, 0.0);
	}
	
	vec4 color =  	ambient 
					+ emission 
					+ diffuse 
					+ specular ;

	//gl_FragColor =  color;
    gl_FragColor =  texture2D(color_texture, gl_TexCoord[0].st) * color;
}