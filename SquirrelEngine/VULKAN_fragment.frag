#version 450 

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTex;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

layout(location = 0) out vec4 outColor;

// Camera data, for specular highlights
layout(binding = 1) uniform Camerabuffer
{
    mat4 viewMatrix;
    vec3 cameraPosition;
} camera;

// Material buffer, for material properties
layout(binding = 2) uniform MaterialBuffer
{
    vec3 diffuseColor;
    vec3 specularColor;
    uint specularity;
    float smoothness;
} material;

// Light struct for light data, needs to match the one in GraphicsVulkan.h
struct Light
{
    vec3 lightPosition;
    vec3 lightDirection;
    vec3 diffuseColor;
    vec3 ambientColor;
    float intensity;
    float ambientIntensity;
    uint lightType;
    float innerCutoffAngle;
    float outerCutoffAngle;
    
    float linearAttenuation;
    float quadraticAttenuation;
    
};

layout(binding = 3) uniform LightBuffer
{
    Light lights[8];
    uint lightCount;
} lights;


// Lambert Diffuse Calculation 
float CalculateDiffusePower(vec3 pixelNormal, vec3 pixelToLight)
{
    return max(dot(pixelToLight, pixelNormal), 0);
}

// Spotlight multiplication factor (De Vries, 2014)
float CalculateSpotlightPower(vec3 pixelToLight, vec3 spotlightDirection, float innerCutoff, float outerCutoff)
{
    float angleThisPixel = acos(max(dot(pixelToLight, -spotlightDirection), 0));
    if (angleThisPixel > outerCutoff) return 0;
    if (angleThisPixel < innerCutoff)
        return 1;
    return (angleThisPixel - outerCutoff) / (innerCutoff - outerCutoff);
}

// Blinn-phong specular
float CalculateSpecularPower(vec3 pixelNormal, vec3 pixelToLight, vec3 pixelPosition)
{
    // Get pixel to camera
    vec3 viewVector = normalize(camera.cameraPosition - pixelPosition);
    // Get halfway vector
    vec3 halfway = normalize(pixelToLight + viewVector);
    // Dot product and put to the power of specularity.
    return pow(max(dot(pixelNormal, halfway), 0), material.specularity);
}

void main() {
     // Totals for ambient, diffuse and specular light
    vec3 ambientTotal = vec3(0, 0, 0);
    vec3 diffuseTotal = vec3(0, 0, 0);
    vec3 specularTotal = vec3(0, 0, 0);
    
    // Loop over every light
    for (uint i = 0; i < lights.lightCount; i++)
    {
        vec3 lightRayDirection = vec3(0, 0, 0);
        float spotlightMultFactor = 1;
        
        // Get light distance for attenuation
        float lightDistance = length(lights.lights[i].lightPosition - inWorldPos);
        
        // The light ray direction is the direction from the pixel to the incoming light ray
        // For orthographic directional lights this is the light direction
        // For perspective point and spot lights this is the pixel position to light position. 
        if (lights.lights[i].lightType == 0)
            lightRayDirection = -normalize(lights.lights[i].lightDirection);
        else
            lightRayDirection = normalize(lights.lights[i].lightPosition - inWorldPos);
        
        // If we are a spotlight replace the 1 spotlight mult factor with the actual spotlight multiplier
        if (lights.lights[i].lightType == 2)
            spotlightMultFactor = CalculateSpotlightPower(lightRayDirection, lights.lights[i].lightDirection, lights.lights[i].innerCutoffAngle, lights.lights[i].outerCutoffAngle);
        
        // Get attenuation factor, this is what all light should be divided by before it is added to totals
        // Constant attenuation = 1, this can be controlled via intensity
        float attenuation = 1 + (lightDistance * lights.lights[i].linearAttenuation + lightDistance * lightDistance * lights.lights[i].quadraticAttenuation);
        
        // Add ambient light to total 
        ambientTotal += lights.lights[i].ambientColor * lights.lights[i].ambientIntensity * material.diffuseColor / attenuation;
        // Add diffuse light to total
        diffuseTotal += CalculateDiffusePower(inNormal, lightRayDirection) * material.diffuseColor * lights.lights[i].diffuseColor * spotlightMultFactor * lights.lights[i].intensity / attenuation;
        // Add specular light to total (Specular doesn't attenuate)
        specularTotal += CalculateSpecularPower(inNormal, lightRayDirection, inWorldPos) * 
                                material.specularColor * lights.lights[i].diffuseColor * spotlightMultFactor * material.smoothness * lights.lights[i].intensity;

    }
    
    // Return ambient + diffuse + specular
    outColor = vec4(ambientTotal + diffuseTotal + specularTotal, 1);
    //outColor = vec4(inNormal /0.5 + 0.5, 1);
}