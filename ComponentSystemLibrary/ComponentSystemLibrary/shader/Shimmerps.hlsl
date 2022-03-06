
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


float m_Param;     // 揺らぎ具合係数

sampler s0 : register(s0);   // 陽炎テクスチャー
//sampler s1 : register(s1);   // バックバッファテクスチャー

Texture2D		g_Tex : register(t0);					// テクスチャ
Texture2D		g_Tex2 : register(t1);					// テクスチャ

struct VS_OUTPUT
{
	float4 Pos    : POSITION;
	float2 Tex    : TEXCOORD;
};

//ps
float4 main(VS_OUTPUT In) : SV_Target
{
	//陽炎テクスチャーから色情報を取得し、テクセルの移動量を計算する
	float4 ShimmerColor = g_Tex2.Sample(s0, In.Tex);

	//テクスチャーには 0.0f ～ 1.0f の範囲で格納されているので -1.0f ～ 1.0f に変換する
	ShimmerColor.xy = (ShimmerColor.xy - 0.5f) * 2.0f;

	return g_Tex.Sample(s0, In.Tex - (ShimmerColor.xy) * ShimmerColor.z * 0.08f);
}
