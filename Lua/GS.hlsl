struct GSOutput
{
	float4 outPosition : SV_POSITION;
	float3 Normal : NORMAL;
	float3 worldPos : WORLDPOS;
	float3 camPos : CAMPOS;
};
struct GSInput
{
	float4 inputPosition : SV_POSITION;
	float3 camPos : CAMPOS;
	float4x4 world : WORLD;
};
[maxvertexcount(3)]
void main(triangle GSInput input[3],
	inout TriangleStream<GSOutput> OutputStream)
{
	GSOutput output;
	float3 u = input[1].inputPosition - input[0].inputPosition;
	float3 v = input[2].inputPosition - input[0].inputPosition;
	float3 normal = normalize(cross(u, v));
	for (uint i = 0; i < 3; i++)
	{
		output.worldPos = normalize(mul(input[i].world, input[i].inputPosition));
		output.Normal = normal;
		output.camPos = input[i].camPos;
		output.outPosition = input[i].inputPosition;
		OutputStream.Append(output);
	}
	OutputStream.RestartStrip();
}