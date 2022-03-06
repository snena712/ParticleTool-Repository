#include "Quad2DMultiRenderComponent.h"
#include "../DX11System/CDirectxGraphics.h"
#include "../DX11System/ImageResourceManager.h"
#include "AnimationComponent.h"
#include "ImGuiComponent.h"
#include "SceneManager.h"

using namespace Component;

Quad2DMultiRender::Quad2DMultiRender(GameObject* _owner, const char* _filename, const char* _subfilename) :
	Quad2DRender(_owner, _filename),
	texSubFileName(_subfilename)
{
	bool sts;

	sts = ImageResourceManager::GetInstance()->Load(_subfilename);
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", _subfilename);
		MessageBox(nullptr, str, "error", MB_OK);
		return;
	}
	colorVolume.volume = 1;
	ConstBufferInit();

	texSubFileName = _subfilename;
}

Quad2DMultiRender::~Quad2DMultiRender() {

	//�p�����ŃJ�E���^���}�C�i�X���邽�߁A�g�p���Ȃ��Ȃ鎞�̃J�E���^�̒l�́u�P�v
	if (counter == 1) {
		m_ColorVolumeBuffer->Release();
		m_ColorVolumeBuffer = nullptr;
	}
}

void Quad2DMultiRender::Start()
{
	SceneManager::AddDrawFunction(mRenderingType, owner);
}

void Quad2DMultiRender::DrawUI() {

	//texFileName����Ȃ�`�悵�Ȃ�
	if (texFileName != "" && texSubFileName != "") {

		Transform* tra = owner->transform;
		DirectX::XMFLOAT4X4 make, mtx, scale;

		DX11MtxScale(tra->scale.x, tra->scale.y, tra->scale.z, scale);
		DX11MtxFromQt(mtx, tra->qt);
		DX11MtxMultiply(make, scale, mtx);

		UpdatePosition(make, tra->position);

		if (tra->GetParent() != nullptr) {
			Transform::GetMultiplParentTransform(make, tra);
		}

		// �e�N�X�`�����e��Z�b�g 
		ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
		std::vector<ID3D11ShaderResourceView*> srv = { {
		 ImageResourceManager::GetInstance()->GetResource(texFileName)->GetSRV() ,
		 ImageResourceManager::GetInstance()->GetResource(texSubFileName)->GetSRV() ,
		 } };
		devcontext->PSSetShaderResources(0, static_cast<UINT>(srv.size()), srv.data());

		if (tra == nullptr) {
			MessageBox(nullptr, "NotFound <Transform> Component", "error!!", MB_OK);
		}

		Component::Animation* ani = this->owner->GetComponent<Component::Animation>();

		if (ani != nullptr)
		{
			// UV�̍��W���ς������
			if (ani->UvCheck())
			{	// ���݂̐e�A�j���[�V������uv�擾�AUV�̃Z�b�g
				SetUVPos(ani->UvGet().left, ani->UvGet().right, ani->UvGet().up, ani->UvGet().down);
			}
		}
		else {
			if (uv != NULL) {
				SetUVPos(uv[0], uv[1], uv[2], uv[3]);
			}
			else {
				SetUVPos(0, 1, 0, 1);
			}
		}

		//uvState�̓��e�ɉ�����UV���W�𔽓]������
		if (uvswitchCall) {
			switch (uvState)
			{
			case UV_STATE::TURN_X:
				std::swap(uv[0], uv[1]);
				std::swap(uv[2], uv[3]);
				break;

			case UV_STATE::TURN_Y:
				std::swap(uv[0], uv[2]);
				std::swap(uv[1], uv[3]);
				break;

			case UV_STATE::TURN_XY:
				std::swap(uv[0], uv[3]);
				std::swap(uv[1], uv[2]);
				break;

			default:
				break;
			}
			uvswitchCall = false;
		}

		//���_�f�[�^�A���_�o�b�t�@�X�V
		quad.updateVertex(width, height, color, uv);
		quad.updateVbuffer();

		// �����F�̊����ύX
		devcontext->UpdateSubresource(m_ColorVolumeBuffer, 0, nullptr, &colorVolume, 0, 0);
		devcontext->PSSetConstantBuffers(6, 1, &m_ColorVolumeBuffer);

		// �ʒu�Ɗp�x��ݒ�
		if (tra != nullptr) {
			quad.SetMtx(make);
		}

		// ��`��`��
		quad.MultiDraw();
	}
}

void Quad2DMultiRender::ChangeSubTexture(const char* _fileName) {

	bool sts;

	sts = ImageResourceManager::GetInstance()->Load(_fileName);
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", _fileName);
		MessageBox(nullptr, str, "error", MB_OK);
		return;
	}

	texSubFileName = _fileName;
}


void Quad2DMultiRender::ConstBufferInit()
{
	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// �R���X�^���g�o�b�t�@�쐬
	bool sts = CreateConstantBuffer(
		device,			// �f�o�C�X
		sizeof(ColorVolume),	// �T�C�Y
		&m_ColorVolumeBuffer);			// �R���X�^���g�o�b�t�@�Q
	if (!sts) {
		MessageBox(NULL, "CreateBuffer(constant buffer world) error", "Error", MB_OK);
	}

}