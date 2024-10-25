
cbuffer MaterialBuffer : register(b0) {
    float3 diffuseColor;
    float3 specularColor;
    unsigned int specularity;
    float smoothness;
};

struct Input {
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

float4 main(Input input) : SV_TARGET{
	return float4(1, 1, 1, 1);
}