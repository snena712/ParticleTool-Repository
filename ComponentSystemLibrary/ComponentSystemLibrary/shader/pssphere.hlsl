#include	"psvscommon.hlsl"
//--------------------------------------------------------------------------------------
// �R���X�^���g�o�b�t�@
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// �\���̒�`
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �s�N�Z���V�F�[�_�[
//--------------------------------------------------------------------------------------
float4 main( VS_OUTPUT input ) : SV_Target
{
		float4 N = input.Normal;
	float4 L = LightDirection;
	N.w = 0.0f;					// �@���̓x�N�g���Ȃ̂łv�̒l���O�ɂ���B
	L.w = 0.0f;					// ���̕����̓x�N�g���Ȃ̂łv�̒l���O�ɂ���B

	N = normalize(N);
	L = normalize(L);

	float d = max(0.0, dot(L, N));
	float4 diffuse = diffuseMaterial * (d + 0.3f);

	float4 H;
	float4 V = normalize(EyePos - input.WPos);
	H = normalize(L + V);

	float s = max(0, dot(N, H));
	s = pow(s, 50);
	float4 specular = s * specularMaterial;

	
	float4 col = Ambient + specular + diffuse;
	return col;
}