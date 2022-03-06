Texture2D		g_Tex : register(t0);					// テクスチャ
SamplerState	g_SamplerLinear : register(s0);			// サンプラー

cbuffer ConstantBufferWorld : register(b0)		// ワールド変換行列
{
	matrix World;
}

cbuffer ConstantBufferWorld : register(b1) // ワールド変換行列
{
    matrix View;
}

cbuffer ConstantBufferProjection : register(b2) // ワールド変換行列
{
    matrix Projection;
}

cbuffer ConstantBufferMaterial : register(b3)
{
	float4	ambientMaterial;
	float4	diffuseMaterial;
	float4	specularMaterial;
}

cbuffer ConstantBufferLight : register(b4)
{
	float4 LightDirection;			// 光の方向
	float4 EyePos;					// 視点位置
	float4 Ambient;
}

cbuffer ConstantBufferViewPort : register(b5)
{
	uint4	ViewportWidth;						// ビューポート幅
	uint4	ViewportHeight;						// ビューポート高さ
};

// 頂点シェーダーの出力データ
struct VS_OUTPUT {
	float4 Position : SV_POSITION;
	float4 Color	: COLOR;
	float2 Tex		: TEXCOORD;
	float4 WPos		: TEXCOORD1;
	float4 Normal	: TEXCOORD2;
};

struct GS_OUTPUT {
	float4 Position : SV_POSITION;
	float4 Color	: COLOR;
	float2 Tex		: TEXCOORD;
	float4 WPos		: TEXCOORD1;
	float4 Normal	: TEXCOORD2;
};

struct VS_OUTPUT_MULTI {
	float4 Position : SV_POSITION;
	float4 Color	: COLOR;
	float2 Tex		: TEXCOORD;
	float4 WPos		: TEXCOORD1;
	float4 Normal	: TEXCOORD2;
	float2 Alp		: ALPHA;
};