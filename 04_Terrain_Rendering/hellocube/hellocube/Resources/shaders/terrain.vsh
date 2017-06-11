#version 440

attribute vec3 in_position;
attribute vec3 in_normal;

uniform mat4 projMatrix, mvMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPos;

varying vec3 N;
varying vec3 vert;
varying vec3 L;

void main(){
	vert = vec3(mvMatrix * vec4(in_position, 1.0));
    gl_Position = projMatrix * vec4(vert, 1.0);
	L = normalize(lightPos - vert);
    N = normalize(normalMatrix * in_normal);
}
