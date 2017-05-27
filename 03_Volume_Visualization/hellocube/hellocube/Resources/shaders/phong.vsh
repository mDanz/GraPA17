attribute vec3 in_position;
attribute vec3 in_normal;

uniform mat4 projMatrix, mvMatrix, normalMatrix;
uniform vec3 lightPos;

varying vec3 N;
varying vec3 vert;
varying vec3 L;

void main(){
    gl_Position = projMatrix * mvMatrix * vec4(in_position, 1.0);
    vert = vec3(mvMatrix * vec4(in_position, 1.0));
	L = normalize(lightPos - vert);
    N = normalize(vec3(normalMatrix * vec4(in_normal, 0.0)));
}
