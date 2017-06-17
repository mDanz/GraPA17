#version 440

layout(location=0) in vec3 vertex;

//uniform mat4 mvMatrix;
//uniform mat4 projMatrix;

uniform float terrainWidthScale;
uniform vec3 cameraPos;

out vec3 vPosition;


void main(void)
{   
    // pass through
    vec3 vPosition = vec3(vertex.x + floor(cameraPos.x/terrainWidthScale), vertex.y, vertex.z + floor(cameraPos.z/terrainWidthScale));
    //vPosition += vec3(cameraPos.x, 0.0f, cameraPos.z);//vec3(floor(cameraPos.x/terrainWidthScale), 0.f, floor(cameraPos.z/terrainWidthScale));
    vPosition.xz *= terrainWidthScale;
    gl_Position = vec4(vPosition, 1.0f);//projMatrix * mvMatrix * vec4(vPosition, 1.0f);
}
