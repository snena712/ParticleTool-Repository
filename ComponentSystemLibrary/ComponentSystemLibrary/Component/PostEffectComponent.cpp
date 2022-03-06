#include "PostEffectComponent.h"
#include "../DX11System/CDirectxGraphics.h"
#include "../DX11System/ImageResourceManager.h"
#include "AnimationComponent.h"
#include "../DX11System/shaderhashmap.h"
#include "../source/PostEffect/DirectGraphicsTexture.h"
#include "../source/PostEffect/BloomPostEffect.h"
#include "../source/PostEffect/ShimmerPostEffect.h"

using namespace Component;

PostEffectQuad PostEffectComponent::postquad;
unsigned int PostEffectComponent::postcounter;

PostEffectComponent::PostEffectComponent(GameObject* Owner, const char* _filename, const char* _subfilename,std::string _vs, std::string _ps) :
	IRenderBase(Owner, RenderingType::Transparent), texFileName(""), width(1.0f), height(1.0f), uv{ {0.0f, 0.0f}, { 1.0f,0.0f }, { 0.0f,1.0f }, { 1.0f,1.0f } }, color(1.0f, 1.0f, 1.0f, 1.0f),  texSubFileName("")
{
	// �f�o�C�X�̎擾
	m_device = CDirectXGraphics::GetInstance()->GetDXDevice();
	m_devContext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	uv[0].y = 0.7f;
	uv[1].y = 0.7f;	
	uv[2].y = 1.0f;
	uv[3].y = 1.0f;

	if (postcounter == 0) {
		postquad.Init(width, height, color, uv, 0.0f,"shader/vsmultibiilbord.hlsl", "shader/multips.hlsl");
	}

	postcounter++;
	//renderMode = PostEffectQuad::RenderMode::DEFAULT;

	// �摜�̃��[�h
	auto sts = ImageResourceManager::GetInstance()->Load(_filename);
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", _filename);
		MessageBox(nullptr, str, "error", MB_OK);
		return;
	}

	sts = ImageResourceManager::GetInstance()->Load(_subfilename);
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", _subfilename);
		MessageBox(nullptr, str, "error", MB_OK);
		return;
	}

	texFileName = _filename;
	texSubFileName = _subfilename;
}

PostEffectComponent::~PostEffectComponent() {
	postcounter--;

	if (postcounter == 0) {
		postquad.Exit();
	}
}

void PostEffectComponent::Start() {
	SceneManager::AddDrawFunction(mRenderingType, owner);
}

void PostEffectComponent::Update() {

}

void PostEffectComponent::DrawObject() {

	float ClearColor[4] = { 0.5,0.5,0.5, 1.0f }; //red,green,blue,alpha�i�q�f�a�̊e�l��0.0�`1.0�Ŏw��j

	auto _shimmer = PostEffect::ShimmerPostEffect::Instance();

	// �����_�[�^�[�Q�b�g�̐ݒ�@�@(�z���p)
	auto _rtv = _shimmer->GetPostEffectRTV()->GetRTV();
	this->m_devContext->OMSetRenderTargets(1, &_rtv, nullptr);

	// �ʏ�̕`��̐ݒ�@�@// ���Əꏊ�w��
	NormalDraw();

	// �����_�[�^�[�Q�b�g�̐ݒ�@�@(�ʏ�)
	_rtv = _shimmer->GetSRVDefault().GetRTV();
	auto  dsv = CDirectXGraphics::GetInstance()->GetDepthStencilView();
	this->m_devContext->OMSetRenderTargets(1,&_rtv, dsv);
}



// �ʏ�̕`��ݒ�(�z���̏ꏊ)
void PostEffectComponent::NormalDraw()
{
	//texFileName����Ȃ�`�悵�Ȃ�
	if (texFileName != "") {

		Transform* tra = owner->transform;
		DirectX::XMFLOAT4X4 make, mtx, scale;

		// ���[���h�s��̍쐬
		DX11MtxScale(1, 1, 1, scale);
		DX11MtxFromQt(mtx, tra->qt);
		DX11MtxMultiply(make, scale, mtx);	


		// �e�N�X�`�����Z�b�g SRV�̐ݒ�
		ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
		ID3D11ShaderResourceView* srv[] = {
			 ImageResourceManager::GetInstance()->GetResource(texFileName)->GetSRV() ,
		 ImageResourceManager::GetInstance()->GetResource(texSubFileName)->GetSRV() ,
		};

		devcontext->PSSetShaderResources(0, 2, srv);

		if (tra == nullptr) {
			MessageBox(nullptr, "NotFound <Transform> Component", "error!!", MB_OK);
		}

		// �A�j���[�V�����̏����@�@animationCompo�g��
		uv[0].y -= 0.03f;
		uv[1].y -= 0.03f;
		uv[2].y -= 0.03f;
		uv[3].y -= 0.03f;

		if (uv[0].y<0)
		{
			uv[0].y = 0.7f;
			uv[1].y = 0.7f;
			uv[2].y = 1.0f;
			uv[3].y = 1.0f;
		}

		//uv ������Ƃ�
		if (uv != NULL) {
			SetUVPos(uv[0], uv[1], uv[2], uv[3]);
		}
		// �e�����鎞
		if (tra->GetParent() != nullptr) {
			Transform::GetMultiplParentTransform(make, tra);
		}
		
		// �ʏ펞�̕`��ݒ�
		postquad.NormalSetting();

		//  ���X�g�̍Ō�擾���ă��[�v��
		for (auto list : poslist) {
			if (list == NULL)break;
			//���_�f�[�^�A���_�o�b�t�@�X�V
			postquad.SetMtx(make);
			postquad.updateVertex_3D(width, height, color, uv, 0.0f);
			postquad.updateVbuffer();
			postquad.SetPS(m_psmultistr);
			postquad.SetVS(m_vsmultistr);
			// ��`��`��
			postquad.NormalDraw(&list);
		}
	}
}



// UV�̐ݒ�
void PostEffectComponent::SetUVPos(DirectX::XMFLOAT2 left_top, DirectX::XMFLOAT2 right_top, DirectX::XMFLOAT2 left_bottom, DirectX::XMFLOAT2 right_bottom) {
	uv[0] = left_top;
	uv[1] = right_top;
	uv[2] = left_bottom;
	uv[3] = right_bottom;
}

//-----------------------------
// create by Suehara
//-----------------------------