#version 440

layout(location=0) in vec3 vertex;

uniform mat4 mvMatrix;
uniform mat4 projMatrix;


void main(void)
{   
    // pass through
    gl_Position = projMatrix * mvMatrix * vec4(vertex, 1.0f);
}
