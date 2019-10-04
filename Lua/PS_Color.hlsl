struct PS_INPUT
{
	float4 inPosition : SV_POSITION;
	float4 Color : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	return float4(input.Color);
}