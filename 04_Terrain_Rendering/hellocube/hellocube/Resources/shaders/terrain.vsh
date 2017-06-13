#version 400

layout(location=0) in vec3 vertex;

uniform float terrainWidthScale;
uniform vec3 cameraPos;

out vec3 vPosition;

void main(void)
{   
    // pass through
    vPosition = vertex;
    vPosition += vec3(floor(cameraPos.x/terrainWidthScale), 0.f, floor(cameraPos.z/terrainWidthScale));
    vPosition *= terrainWidthScale;
}
