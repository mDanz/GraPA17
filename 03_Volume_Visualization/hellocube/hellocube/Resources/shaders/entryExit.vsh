#version 400

layout(location = 0) in vec3 vertex;

uniform mat4 mvMatrix;
uniform mat4 projMatrix;

out vec4 modelPos;

void main() {
    vec4 pos = projMatrix * mvMatrix * vec4(vertex, 1);

    modelPos = vec4((vertex + vec3(1.f))/2.f, pos.w/255.f);
    gl_Position = pos;
}
