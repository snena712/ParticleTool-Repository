#include "psvscommon.hlsl"

float gammacorrect(float gamma, float x);
//--------------------------------------------------------------------------------------
//ÉÇÉmÉgÅ[Éì Pixel Shader
//--------------------------------------------------------------------------------------
float4 main( VS_OUTPUT input) : SV_Target
{
	float4 texcol = g_Tex.Sample(g_SamplerLinear,input.Tex);
	float4 col = texcol * input.Color;

	//ãtÉKÉìÉ}ï‚ê≥
	col.r = gammacorrect(1 / 2.2, col.r);
	col.g = gammacorrect(1 / 2.2, col.g);
	col.b = gammacorrect(1 / 2.2, col.b);

	float monocol = 0.2126*col.r + 0.7152*col.g + 0.0722*col.b;

	//ÉKÉìÉ}ï‚ê≥
	monocol = gammacorrect(2.2, monocol);

	return float4(monocol, monocol, monocol, 1.0);
}

float gammacorrect(float gamma, float x)
{
	return pow(x, 1.0 / gamma);
}