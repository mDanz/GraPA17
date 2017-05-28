
varying vec3 N;
varying vec3 L;
varying vec3 vert;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float specularExp;
uniform vec3 idColor;

//const vec3 ambientColor = vec3(1.0, 0.0, 0.0);
//const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
//const vec3 specularColor = vec3(1.0, 1.0, 1.0);
//const float specularExp = 50.0;

void main() {
    vec3 R = reflect(-L, N);
    vec3 V = normalize(-vert);

    float lambertian = clamp(dot(L,N), 0.0, 1.0);
    
	float specular = 0.0;
	if(lambertian > 0.0) 
  {
    float specAngle = clamp(dot(R, V), 0.0, 1.0);
    specular = pow(specAngle, specularExp);
  }
  
  gl_FragData[0] = vec4(ambientColor + lambertian*diffuseColor + specular*specularColor, 1.0);


	gl_FragData[1] = vec4(idColor, 1.0);

}
