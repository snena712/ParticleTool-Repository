void main(
	float4 pos : SV_POSITION,
	float4 color : COLOR0,
	out float4 outColor : SV_Target0,
	out float4 outColor2 : SV_Target1)
{
	outColor = color;
	outColor2 = color;
}