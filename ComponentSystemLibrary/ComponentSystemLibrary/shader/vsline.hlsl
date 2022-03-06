//--------------------------------------------------------------------------------------
//		vs.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.hlsl"

//--------------------------------------------------------------------------------------
//		���_�V�F�[�_�[
//--------------------------------------------------------------------------------------
VS_OUTPUT main(
	float4 Pos		:	POSITION,
	float4 color	: COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	Pos.w = 1.0f;
	output.Position = mul(Pos, View);
	output.Position = mul(output.Position, Projection);

	output.Color = color;

	return output;
}