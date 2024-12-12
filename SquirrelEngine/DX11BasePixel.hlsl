
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
    float3 diffuseColor;
    float3 ambientColor;
    float intensity;
    float ambientIntensity;
    
    float3 p_0; // Used so struct aligns with 16 bytes.
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
float CalculateDiffusePower(float3 pixelNormal, float3 lightPosition, float3 pixelPosition)
{
    float3 lightDirection = normalize(lightPosition - pixelPosition);
    return max(dot(lightDirection, pixelNormal), 0);
}

float4 main(Input input) : SV_TARGET{
    
    float3 ambientTotal = float3(0, 0, 0);
    float3 diffuseTotal = float3(0, 0, 0);
    float3 specularTotal = float3(0, 0, 0);
    
    for (unsigned int i = 0; i < lightCount; i++)
    {
        ambientTotal += lights[i].ambientColor * lights[i].ambientIntensity * diffuseColor;
        diffuseTotal += CalculateDiffusePower(input.normal, lights[i].lightPosition, input.worldPosition) * diffuseColor;
    }
    //return float4(input.normal / 2.0f + 0.5f, 1);
    return float4(ambientTotal + diffuseTotal, 1);
}