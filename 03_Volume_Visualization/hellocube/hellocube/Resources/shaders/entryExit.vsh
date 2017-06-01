

attribute vec3 in_position;

uniform mat4 mvMatrix;
uniform mat4 projMatrix;

out vec4 modelPos;

void main() {
    vec4 pos = projMatrix * mvMatrix * vec4(in_position, 1);

    modelPos = vec4((in_position + vec3(1.f))/2.f, pos.w/255.f);
    gl_Position = pos;
}
