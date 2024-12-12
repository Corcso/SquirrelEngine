// Camera data, for specular highlights
cbuffer CameraBuffer : register(b0)
{
    matrix viewMatrix;
    float3 cameraPosition;
};

// Material buffer, for material properties
cbuffer MaterialBuffer : register(b1)
{
    float3 diffuseColor;
    float3 specularColor;
    unsigned int specularity;
    float smoothness;
};

// Light struct for light data, needs to match the one in GraphicsDX11.h
struct Light
{
    float3 lightPosition;
    float3 lightDirection;
    float3 diffuseColor;
    float3 ambientColor;
    float intensity;
    float ambientIntensity;
    unsigned int lightType;
    float innerCutoffAngle;
    float outerCutoffAngle;
    
    float linearAttenuation;
    float quadraticAttenuation;
    
    float2 p_0;
};

// Light buffer data. 
cbuffer LightsBuffer : register(b2)
{
    Light lights[8];
    unsigned int lightCount;
};

struct Input {
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// Lambert Diffuse Calculation 
float CalculateDiffusePower(float3 pixelNormal, float3 pixelToLight)
{
    return max(dot(pixelToLight, pixelNormal), 0);
}

// Spotlight multiplication factor (De Vries, 2014)
float4 CalculateSpotlightPower(float3 pixelToLight, float3 spotlightDirection, float innerCutoff, float outerCutoff)
{
    float angleThisPixel = acos(max(dot(pixelToLight, -spotlightDirection), 0));
    if (angleThisPixel > outerCutoff)
        return 0;
    if (angleThisPixel < innerCutoff)
        return 1;
    return (angleThisPixel - outerCutoff) / (innerCutoff - outerCutoff);
}

// Blinn-phong specular
float CalculateSpecularPower(float3 pixelNormal, float3 pixelToLight, float3 pixelPosition)
{
    // Get pixel to camera
    float3 viewVector = normalize(cameraPosition - pixelPosition);
    // Get halfway vector
    float3 halfway = normalize(pixelToLight + viewVector);
    // Dot product and put to the power of specularity. 
    return pow(max(dot(pixelNormal, halfway), 0), specularity);
}

float4 main(Input input) : SV_TARGET{
    
    // Totals for ambient, diffuse and specular light
    float3 ambientTotal = float3(0, 0, 0);
    float3 diffuseTotal = float3(0, 0, 0);
    float3 specularTotal = float3(0, 0, 0);
    
    // Loop over every light
    for (unsigned int i = 0; i < lightCount; i++)
    {
        float3 lightRayDirection = float3(0, 0, 0);
        float spotlightMultFactor = 1;
        
        // Get light distance for attenuation
        float distance = length(lights[i].lightPosition - input.worldPosition);
        
        // The light ray direction is the direction from the pixel to the incoming light ray
        // For orthographic directional lights this is the light direction
        // For perspective point and spot lights this is the pixel position to light position. 
        if (lights[i].lightType == 0)
            lightRayDirection = -normalize(lights[i].lightDirection);
        else
            lightRayDirection = normalize(lights[i].lightPosition - input.worldPosition);
        
        // If we are a spotlight replace the 1 spotlight mult factor with the actual spotlight multiplier
        if (lights[i].lightType == 2)
            spotlightMultFactor = CalculateSpotlightPower(lightRayDirection, lights[i].lightDirection, lights[i].innerCutoffAngle, lights[i].outerCutoffAngle);
        
        // Get attenuation factor, this is what all light should be divided by before it is added to totals
        // Constant attenuation = 1, this can be controlled via intensity
        float attenuation = 1 + (distance * lights[i].linearAttenuation + distance * distance * lights[i].quadraticAttenuation);
        
        // Add ambient light to total 
        ambientTotal += lights[i].ambientColor * lights[i].ambientIntensity * diffuseColor / attenuation;
        // Add diffuse light to total
        diffuseTotal += CalculateDiffusePower(input.normal, lightRayDirection) * diffuseColor * lights[i].diffuseColor * spotlightMultFactor * lights[i].intensity / attenuation;
        // Add specular light to total
        specularTotal += CalculateSpecularPower(input.normal, lightRayDirection, input.worldPosition) * 
                                specularColor * lights[i].diffuseColor * spotlightMultFactor * smoothness * lights[i].intensity / attenuation;

    }
    
    // Return ambient + diffuse + specular
    return float4(ambientTotal + diffuseTotal + specularTotal, 1);
}