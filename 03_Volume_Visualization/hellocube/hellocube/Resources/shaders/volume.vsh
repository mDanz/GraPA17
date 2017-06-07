
attribute vec3 in_position;

out vec2 fragPos;

void main() {

    fragPos = (in_position + vec3(1.f)).xy/2.f;
    gl_Position = vec4(in_position, 1.f);
}
