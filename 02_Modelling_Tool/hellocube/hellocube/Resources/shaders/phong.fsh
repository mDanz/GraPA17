
varying vec3 normal;
varying vec3 vert;

uniform vec3 lightPos;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float specularExp;

//const vec3 ambientColor = vec3(1.0, 0.0, 0.0);
//const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
//const vec3 specularColor = vec3(1.0, 1.0, 1.0);
//const float specularExp = 50.0;

void main() {
    vec3 N = normal;
    vec3 L = normalize(lightPos - vert);
    vec3 R = reflect(-L, N);
    vec3 V = normalize(-vert);

    float lambertian = max(dot(L,N), 0.0);
    float specular = 0.0;

    if(lambertian > 0.0) {
       float specAngle = max(dot(R, V), 0.0);
       specular = pow(specAngle, specularExp);
    }
    gl_FragColor = vec4(ambient, 1.0);// + specular*specularColor, 1.0);


}
