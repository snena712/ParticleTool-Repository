#include "psvscommon.hlsl"

Texture2D		g_SecondTex : register(t1);					// �e�N�X�`��

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main( VS_OUTPUT input) : SV_Target
{
	float4 texcol = g_Tex.Sample(g_SamplerLinear,input.Tex);
	return texcol*input.Color;
}
