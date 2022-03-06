struct InstancedParam {
	float3 offset;
	float pad;
	float4 color;
};
StructuredBuffer<InstancedParam> offsets : register(t0);

struct Input
{
	float4 pos : POSITION;
	uint instanceID : SV_InstanceID;
};

struct Output
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
};

Output main(Input input)
{
	Output output;
	output.pos = input.pos;
	output.pos.xyz *= 0.5f;
	output.pos.xyz += offsets[input.instanceID].offset;
	output.color = offsets[input.instanceID].color;

	return output;
}