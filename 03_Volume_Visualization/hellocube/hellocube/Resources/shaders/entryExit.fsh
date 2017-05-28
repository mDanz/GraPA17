#version 400

in vec4 modelPos;
out vec4 outColor;

void main() {
    outColor = modelPos;
}
