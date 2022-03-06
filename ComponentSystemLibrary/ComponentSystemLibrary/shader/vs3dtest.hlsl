//--------------------------------------------------------------------------------------
// Particle Rendering
//--------------------------------------------------------------------------------------
struct VS_OUTPUT {
	float4 Position : SV_POSITION;
	float4 Color	: COLOR;
	float2 Tex		: TEXCOORD;
	float4 WPos		: TEXCOORD1;
	float4 Normal	: TEXCOORD2;
};
struct InDate {

	float4 Pos : POSITION;
	float4 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	uint InstanceId : SV_InstanceID;
};
//--------------------------------------------------------------------------------------
// vsstage.fx
//--------------------------------------------------------------------------------------
//#include	"psvscommonOther.hlsl"
#include	"ComputeShaderHeader.hlsl"
//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------
VS_OUTPUT main(InDate IN)   // インスタンスＩＤ)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	int ID = IN.InstanceId;
	float3 _pos = ParticlesRO[ID].pos;
	float3 _scale = ParticlesRO[ID].scale;

	//float4x4 posMtx = 0, scaleMtx = 0;
	//posMtx._14 = _pos.x;
	//posMtx._24 = _pos.y;
	//posMtx._34 = _pos.z;

	//scaleMtx._11 = 1;
	//scaleMtx._22 = 1;
	//scaleMtx._33 = 1;
	//scaleMtx._44 = 1;

	matrix test = World;
	test._41 = _pos.x;
	test._42 = _pos.y;
	test._43 = _pos.z; 
	test._11 = _scale.x; 
	test._22 = _scale.y; 
	test._33 = _scale.z; 

	output.Position = mul(IN.Pos, test);

	output.WPos = output.Position;
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	output.Tex = IN.Tex;


	output.Color = ParticlesRO[ID].color;

	float4 N = IN.Normal;
	N.w = 0.0f;					// 法線はベクトルなのでＷの値を０にする。

	N = mul(N, test);
	N = normalize(N);

	output.Normal = N;

	return output;
}