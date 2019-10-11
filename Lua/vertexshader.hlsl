cbuffer mycBuffer : register(b0)
{
    float4x4 mat;
}; 
cbuffer myBuffer : register(b1)
{
	float4x4 world;
};
struct VS_INPUT
{
    float3 inPos : POSITION;
	//float4 inColor : COLOR;
  //  float2 inTexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
	//float4 outColor :COLOR;
   // float2 outTexCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input) 
{
    VS_OUTPUT output;
	output.outPosition = mul(float4(input.inPos, 1.0f), world);
	output.outPosition = mul(output.outPosition, mat);
	//float4x4 wpvmatrix = mat * world;
   // output.outPosition = mul(float4(input.inPos, 1.0f), wpvmatrix);
    return output;
}