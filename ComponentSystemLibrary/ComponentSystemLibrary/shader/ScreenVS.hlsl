//--------------------------------------------------------------------------------------
// vs.fx
//-------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};
 
//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------
VS_OUTPUT main(
	float4 Pos		:	POSITION,
	float4 Color : COLOR,
	float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Position = Pos;
	output.TexCoord = Tex;
	//output.Alp = Alpha;

	return output;
}