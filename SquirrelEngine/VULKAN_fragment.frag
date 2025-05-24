#version 450 

layout(location = 0) in vec3 outWorldPos;
layout(location = 1) in vec3 outNormal;
layout(location = 2) in vec2 outTex;
layout(location = 3) in vec3 outTangent;
layout(location = 4) in vec3 outBitangent;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(outNormal, 1.0);
}