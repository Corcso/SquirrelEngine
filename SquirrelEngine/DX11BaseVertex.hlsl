
cbuffer ProjectionBuffer : register(b0) {
	matrix projectionMatrix;
};

cbuffer CameraBuffer : register(b1) {
	matrix viewMatrix;
	float3 cameraPosition;
};

cbuffer WorldBuffer : register(b2) {
	matrix worldMatrix;
	matrix normalWorldMatrix;
};

struct Input {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

struct Output {
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

Output main(Input input) {
	Output output;

    output.position = mul(worldMatrix, float4(input.position, 1));
    output.position = mul(viewMatrix, output.position);
    output.position = mul(projectionMatrix, output.position);

	output.texCoord = input.texCoord;

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	return output;
}