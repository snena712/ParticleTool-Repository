#include <string>
#include "ModelRenderInstance.h"
#include "CameraComponent.h"
#include "../DX11System/ModelResourceManager.h"
#include "../DX11System/shaderhashmap.h"
#include "../DX11System/DX11Settransform.h"
#include "../DX11System/dx11mathutil.h"
#include "SceneManager.h"
#include "ImGuiComponent.h"

Component::ModelRenderInstance::ModelRenderInstance(
	GameObject* _owner,
	const char* _filename,
	const char* _vsfile,
	const char* _psfile,
	const char* _texfolder) :IRenderBase(_owner, RenderingType::Opaque) {

	bool sts;

	sts = ModelResourceManager::GetInstance()->Load(_filename, _texfolder);
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", _filename);
		MessageBox(nullptr, str, "error", MB_OK);
		return;
	}

	m_assimpfileName = _filename;	//�t�@�C������ۑ�

	m_filetype = eASSIMPFILE;

	//// ���_�f�[�^�̒�`�i�A�j���[�V�����Ή��j
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "OFFSET", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 100 },
	};
	unsigned int numElements = ARRAYSIZE(layout);


	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		_vsfile,
		layout,
		numElements);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return;
	}
	m_VertexShaderName = _vsfile;	//�t�@�C������ۑ�

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetPixelShader(_psfile);
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return;
	}
	m_PixelShaderName = _psfile;	//�t�@�C������ۑ�

	SceneManager::AddDrawFunction(mRenderingType, owner);
}


Component::ModelRenderInstance::~ModelRenderInstance() {

}

void Component::ModelRenderInstance::DrawSetting()
{
	ID3D11DeviceContext* devcontext;			// �f�o�C�X�R���e�L�X�g
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	// ���_�t�H�[�}�b�g���Z�b�g
	devcontext->IASetInputLayout(ShaderHashmap::GetInstance()->GetVertexLayout(m_VertexShaderName));
	// ���_�V�F�[�_�[���Z�b�g
	devcontext->VSSetShader(ShaderHashmap::GetInstance()->GetVertexShader(m_VertexShaderName), nullptr, 0);
	// �s�N�Z���V�F�[�_�[���Z�b�g
	devcontext->PSSetShader(ShaderHashmap::GetInstance()->GetPixelShader(m_PixelShaderName), nullptr, 0);
}

void Component::ModelRenderInstance::Draw(int _num) {

	ID3D11DeviceContext* devcontext;			// �f�o�C�X�R���e�L�X�g
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �`��
	ModelData* md = ModelResourceManager::GetInstance()->GetResource(m_assimpfileName);

	DirectX::XMFLOAT4X4 mtx;
	DX11MtxFromQt(mtx, XMFLOAT4(0, 0, 0, 1));


	Transform* tra = owner->transform;
	DirectX::XMFLOAT4X4 make, scale;

	DX11MtxScale(1, 1, 1, scale);

	DX11MtxMultiply(make, scale, mtx);

	if (md != nullptr) {
		md->Draw(devcontext, make, _num);
	}
}

#ifdef DEBUG

void Component::ModelRenderInstance::ImGuiDebug()
{
	//�v���X���Ă��Q������
	std::string name = owner->GetName() + "Mo";
	auto* name2 = name.c_str();

	ImGui::BeginChild(name2, ImVec2(0, 120), true);
	ImGui::Text(u8"*Model");

	// ���f�����@�@���f���}�l�W���[�o������
	ImGui::Checkbox("�t�@�C����", (bool*)this);

	ImGui::EndChild();
}

#endif


//-----------------------------
// create by Suehara
//-----------------------------