//--------------------------------------------------------------------------------------
// vs.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.hlsl"
//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------
VS_OUTPUT main(	float4 Pos		:	POSITION,
				float4 Color	:	COLOR,
				float2 Tex		:	TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

    output.Position = mul(Pos, World);
    output.WPos = output.Position;
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
	output.Tex = Tex;

	output.Color = Color;

	return output;
}