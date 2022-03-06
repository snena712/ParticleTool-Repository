#include "psvscommon.hlsl"

Texture2D       g_SyntheticTexture : register(t1);	// ‡¬F

cbuffer ConstantBufferWorld : register(b6) // Š„‡
{
	float Volume;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(VS_OUTPUT_MULTI input) : SV_Target
{

	float4 base = g_Tex.Sample(g_SamplerLinear, input.Tex);
	float4 src = g_SyntheticTexture.Sample(g_SamplerLinear, input.Alp);

	//float r = base.r * (src.r *Volume)*input.Color.r;
	//float g = base.g * (src.g *Volume)*input.Color.g;
	//float b = base.b * (src.b *Volume)*input.Color.b;

	float r = base.r * (src.r * 1) * input.Color.r;
	float g = base.g * (src.g * 1) * input.Color.g;
	float b = base.b * (src.b * 1) * input.Color.b;

	float4 result = float4(r, g, b, base.a*input.Color.a);
	// æZ‡¬
	return result;
}

