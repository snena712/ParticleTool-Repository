#include "ParticleSystemComponent.h"
#include "../DX11System/CDirectxGraphics.h"
#include "../DX11System/shaderhashmap.h"
#include "../DX11System/DX11Settransform.h"
#include "../DX11System/dx11mathutil.h"
#include "../DX11System/quad.h"

using namespace Component;

std::vector<ParticleSystem::Particle> ParticleSystem::particleArray;// �p�[�e�B�N���̊Ǘ��z��
std::vector<ParticleSystem*> ParticleSystem::particleSystemArray;	// �p�[�e�B�N���V�X�e���̊Ǘ��z��
DirectX::XMFLOAT4X4	ParticleSystem::m_worldmtx;						// ���[���h�ϊ��s��
ComPtr<ID3D11Buffer>	ParticleSystem::m_vertexbuffer;				// ���_�o�b�t�@

ParticleSystem::ParticleSystem(GameObject* Owner) :ComponentBase(Owner) {

	particleSystemArray.emplace_back(this);

}

ParticleSystem::~ParticleSystem() {

	for (int i = 0; i < particleSystemArray.size(); i++) {

		if (particleSystemArray[i] == this) {
			particleSystemArray.erase(particleSystemArray.begin() + i);
			break;
		}
	}
}

void ParticleSystem::SystemInit() {

	DX11MtxIdentity(m_worldmtx);	// �����p��

	// �s�N�Z���V�F�[�_�[�𐶐�
	bool sts = ShaderHashmap::GetInstance()->SetPixelShader("shader/particleps.hlsl");
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error(basicps.hlsl)", "error", MB_OK);
	}

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// �G�������g��
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		"shader/particlevs.hlsl",	// ���_�V�F�[�_�[�t�@�C����
		layout,						// ���_���C�A�E�g
		numElements);				// �G�������g��

	// �f�o�C�X���擾
	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// ���_�o�b�t�@�𐶐�
	sts = CreateVertexBufferWrite(
		device,						// �f�o�C�X�I�u�W�F�N�g
		sizeof(Particle),			// �P���_������o�C�g��
		particleArray.size(),		// ���_��
		&particleArray[0],			// ���_�f�[�^�i�[�������擪�A�h���X
		&m_vertexbuffer				// ���_�o�b�t�@
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
	}

	// �W�I���g���V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetGeometryShader("shader/particlevs.hlsl");
}

void ParticleSystem::SystemUninit() {

}

void ParticleSystem::Update() {

}

void ParticleSystem::Draw() {

	if (particleArray.size() > 0) {
		// �f�o�C�X�R���e�L�X�g���擾
		ID3D11DeviceContext* devcontext;
		devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

		// ���W�ϊ��p�̍s����Z�b�g
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_worldmtx);

		unsigned int stride = sizeof(Particle);	// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
		unsigned  offset = 0;					// �I�t�Z�b�g�l���Z�b�g

		// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
		devcontext->IASetVertexBuffers(
			0,									// �X�^�[�g�X���b�g
			1,									// ���_�o�b�t�@��
			m_vertexbuffer.GetAddressOf(),		// ���_�o�b�t�@�̐擪�A�h���X
			&stride,							// �X�g���C�h
			&offset);							// �I�t�Z�b�g

		// �g�|���W�[���Z�b�g
		devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// ���_�V�F�[�_�[�A�W�I���g���V�F�[�_�[�A�s�N�Z���V�F�[�_�[�擾
		ID3D11VertexShader* vs;
		vs = ShaderHashmap::GetInstance()->GetVertexShader("shader/particlevs.hlsl");
		ID3D11GeometryShader* gs;
		gs = ShaderHashmap::GetInstance()->GetGeometryShader("shader/particlegs.hlsl");
		ID3D11PixelShader* ps;
		ps = ShaderHashmap::GetInstance()->GetPixelShader("shader/particleps.hlsl");

		// ���_���C�A�E�g�擾
		ID3D11InputLayout* layout;
		layout = ShaderHashmap::GetInstance()->GetVertexLayout("shader/particlevs.hlsl");

		devcontext->VSSetShader(vs, nullptr, 0);
		devcontext->GSSetShader(gs, nullptr, 0);
		devcontext->HSSetShader(nullptr, nullptr, 0);
		devcontext->DSSetShader(nullptr, nullptr, 0);
		devcontext->PSSetShader(ps, nullptr, 0);

		// ���_�t�H�[�}�b�g���Z�b�g
		devcontext->IASetInputLayout(layout);

		// �h���[�R�[�����s
		devcontext->Draw(
			particleArray.size(),	// ���_��
			0);						// �J�n�ʒu
	}
}

// ���_�o�b�t�@�X�V
void ParticleSystem::updateVbuffer() {
	D3D11_MAPPED_SUBRESOURCE pData;
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	HRESULT hr = devcontext->Map(m_vertexbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&particleArray[0]), sizeof(Particle) * particleArray.size());
		devcontext->Unmap(m_vertexbuffer.Get(), 0);
	}
}