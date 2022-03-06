//--------------------------------------------------------------------------------------
// vs.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.hlsl"

static const float2 g_texcoords[4] = { float2(0, 1), float2(1, 1), float2(0, 0), float2(1, 0) };


//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------
VS_OUTPUT_MULTI main(
	float4 Pos	: POSITION,
	float4 Color : COLOR,
	float2 Tex : TEXCOORD,
	uint id : SV_VertexID)
{
	VS_OUTPUT_MULTI output = (VS_OUTPUT_MULTI)0;

	output.Position = mul(Pos, World);
	output.WPos = output.Position;
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	output.Tex = Tex;
	output.Alp = g_texcoords[id];

	output.Color = Color;

	return output;
}