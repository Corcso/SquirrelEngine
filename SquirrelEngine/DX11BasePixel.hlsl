
cbuffer CameraBuffer : register(b0)
{
    matrix viewMatrix;
    float3 cameraPosition;
};

cbuffer MaterialBuffer : register(b1)
{
    float3 diffuseColor;
    float3 specularColor;
    unsigned int specularity;
    float smoothness;
};

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
};

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
    float angleThisPixel = acos(max(dot(pixelToLight, spotlightDirection), 0));
    if (angleThisPixel > outerCutoff)
        return 0;
    if (angleThisPixel < innerCutoff)
        return 1;
    return (angleThisPixel - outerCutoff) / (innerCutoff - outerCutoff);
}

float4 main(Input input) : SV_TARGET{
    
    float3 ambientTotal = float3(0, 0, 0);
    float3 diffuseTotal = float3(0, 0, 0);
    float3 specularTotal = float3(0, 0, 0);
    
    for (unsigned int i = 0; i < lightCount; i++)
    {
        float3 lightRayDirection = float3(0, 0, 0);
        float spotlightMultFactor = 1;
        if (lights[i].lightType == 0)
            lightRayDirection = -normalize(lights[i].lightDirection);
        else
            lightRayDirection = normalize(lights[i].lightPosition - input.worldPosition);
        
        if (lights[i].lightType == 2)
            spotlightMultFactor = CalculateSpotlightPower(lightRayDirection, lights[i].lightDirection, lights[i].innerCutoffAngle, lights[i].outerCutoffAngle);
        
        ambientTotal += lights[i].ambientColor * lights[i].ambientIntensity * diffuseColor;
        diffuseTotal += CalculateDiffusePower(input.normal, lightRayDirection) * diffuseColor * spotlightMultFactor;
    }
    //return float4(input.normal / 2.0f + 0.5f, 1);
    return float4(ambientTotal + diffuseTotal, 1);
}