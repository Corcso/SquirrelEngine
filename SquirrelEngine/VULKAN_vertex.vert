#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTex;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outTex;
layout(location = 3) out vec3 outTangent;
layout(location = 4) out vec3 outBitangent;

//layout(binding = 0) uniform UniformBufferObject {
//    mat4 model;
//    mat4 view;
//    mat4 proj;
//} ubo;

//layout(set = 1, binding = 1) uniform LightInfo {
//    float intensity;
//} li;

void main() {
    gl_Position = vec4(inPosition, 1.0);
    outWorldPos = inPosition;
    outNormal = inNormal;
    outTex = inTex;
    outTangent = inTangent;
    outBitangent = inBitangent;
}