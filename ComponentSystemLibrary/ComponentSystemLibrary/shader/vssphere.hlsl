#include	"psvscommon.hlsl"



//--------------------------------------------------------------------------------------
// Particle Rendering
//--------------------------------------------------------------------------------------

// �p�[�e�B�N���̃f�[�^�\����  �]���ȃf�[�^����
struct Particle {
	float3 position;
	int life;

	float3 vel;
	int Alive;

	float3 scale;
	float depth;

	float3 baseDummy;
	float parentId;
};

// �o�b�t�@
StructuredBuffer<Particle> ParticlesRO : register(t0);


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// ���_�V�F�[�_�[
//--------------------------------------------------------------------------------------
VS_OUTPUT main(float4 Pos : POSITION,
	float4 Normal : NORMAL,
	float4 Color : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//uint ID : SV_InstanceID

	//// ���[���h�̍��W���M��H
	//float3 _position = ParticlesRO[ID].position;

	output.Position = mul(Pos, World);
	output.WPos = output.Position;
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	float4 N = Normal;
	N.w = 0.0f;					// �@���̓x�N�g���Ȃ̂łv�̒l���O�ɂ���B

	N = mul(N, World);
	N = normalize(N);

	output.Normal = N;
	output.Color = float4(1,1,1,1);

	return output;
}






