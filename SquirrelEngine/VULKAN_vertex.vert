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

layout(binding = 0) uniform UniformBufferObject {
    mat4 proj;
    mat4 view;
    mat4 world;
} ubo;

//layout(set = 1, binding = 1) uniform LightInfo {
//    float intensity;
//} li;

void main() {

    gl_Position = ubo.proj * ubo.view * ubo.world * vec4(inPosition, 1.0);
    outWorldPos = (ubo.world * vec4(inPosition, 1.0)).xyz;
    outNormal = normalize(ubo.world * vec4(inNormal, 0.0)).xyz;
    outTex = inTex;
    outTangent = normalize(ubo.world * vec4(inTangent, 0.0)).xyz;
    outBitangent = normalize(ubo.world * vec4(inBitangent, 0.0)).xyz;
}