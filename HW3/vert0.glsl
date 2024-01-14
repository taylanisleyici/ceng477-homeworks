#version 120

uniform mat4 modelingMat;
uniform mat4 perspectiveMat;
uniform mat4 viewingMat;

attribute vec3 inVertex;

varying vec4 worldPos;

void main(void)
{
    worldPos = modelingMat * vec4(inVertex, 1.0);
    gl_Position = perspectiveMat * viewingMat * worldPos;
}