cbuffer mycBuffer : register(b0)
{
    float4x4 mat;
	//float3 camPos;
}; 
cbuffer myBuffer : register(b1)
{
	float4x4 world;
};
struct VS_INPUT
{
    float3 inPos : POSITION;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
	//float4x4 world : WORLD;
	//float3 camPos : CAMPOS;
};

VS_OUTPUT main(VS_INPUT input) 
{
    VS_OUTPUT output;
	output.outPosition = mul(float4(input.inPos, 1.0f), world);
	output.outPosition = mul(output.outPosition, mat);
	//output.world = world;
	//output.camPos = camPos;
    return output;
}