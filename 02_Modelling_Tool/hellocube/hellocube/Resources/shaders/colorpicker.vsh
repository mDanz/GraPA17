attribute vec3 in_position;

uniform mat4 projMatrix, mvMatrix;

void main(){
    gl_Position =  projMatrix * mvMatrix * vec4(in_position, 1);
}