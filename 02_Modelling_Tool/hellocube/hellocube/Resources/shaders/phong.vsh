attribute vec3 in_position;
attribute vec3 in_normal;

uniform mat4 projMatrix, mvMatrix, normalMatrix;

varying vec3 normal;
varying vec3 vert;

void main(){
    gl_Position = projMatrix * mvMatrix * vec4(in_position, 1.0);
    vec4 vertPos4 = mvMatrix * vec4(in_position, 1.0);
    vert = vec3(vertPos4) / vertPos4.w;
    normal = normalize(vec3(normalMatrix * vec4(in_normal, 0.0)));
}
