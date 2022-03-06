Texture2D		g_Tex : register(t0);					// �e�N�X�`��
SamplerState	g_SamplerLinear : register(s0);			// �T���v���[

cbuffer ConstantBufferWorld : register(b0)		// ���[���h�ϊ��s��
{
	matrix World;
}

cbuffer ConstantBufferWorld : register(b1) // ���[���h�ϊ��s��
{
    matrix View;
}

cbuffer ConstantBufferProjection : register(b2) // ���[���h�ϊ��s��
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
	float4 LightDirection;			// ���̕���
	float4 EyePos;					// ���_�ʒu
	float4 Ambient;
}

cbuffer ConstantBufferViewPort : register(b5)
{
	uint4	ViewportWidth;						// �r���[�|�[�g��
	uint4	ViewportHeight;						// �r���[�|�[�g����
};

// ���_�V�F�[�_�[�̏o�̓f�[�^
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