
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


float m_Param;     // �h�炬��W��

sampler s0 : register(s0);   // �z���e�N�X�`���[
//sampler s1 : register(s1);   // �o�b�N�o�b�t�@�e�N�X�`���[

Texture2D		g_Tex : register(t0);					// �e�N�X�`��
Texture2D		g_Tex2 : register(t1);					// �e�N�X�`��

struct VS_OUTPUT
{
	float4 Pos    : POSITION;
	float2 Tex    : TEXCOORD;
};

//ps
float4 main(VS_OUTPUT In) : SV_Target
{
	//�z���e�N�X�`���[����F�����擾���A�e�N�Z���̈ړ��ʂ��v�Z����
	float4 ShimmerColor = g_Tex2.Sample(s0, In.Tex);

	//�e�N�X�`���[�ɂ� 0.0f �` 1.0f �͈̔͂Ŋi�[����Ă���̂� -1.0f �` 1.0f �ɕϊ�����
	ShimmerColor.xy = (ShimmerColor.xy - 0.5f) * 2.0f;

	return g_Tex.Sample(s0, In.Tex - (ShimmerColor.xy) * ShimmerColor.z * 0.08f);
}
