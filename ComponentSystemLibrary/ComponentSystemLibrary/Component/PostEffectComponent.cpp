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
	// デバイスの取得
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

	// 画像のロード
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

	float ClearColor[4] = { 0.5,0.5,0.5, 1.0f }; //red,green,blue,alpha（ＲＧＢの各値を0.0～1.0で指定）

	auto _shimmer = PostEffect::ShimmerPostEffect::Instance();

	// レンダーターゲットの設定　　(陽炎用)
	auto _rtv = _shimmer->GetPostEffectRTV()->GetRTV();
	this->m_devContext->OMSetRenderTargets(1, &_rtv, nullptr);

	// 通常の描画の設定　　// 個数と場所指定
	NormalDraw();

	// レンダーターゲットの設定　　(通常)
	_rtv = _shimmer->GetSRVDefault().GetRTV();
	auto  dsv = CDirectXGraphics::GetInstance()->GetDepthStencilView();
	this->m_devContext->OMSetRenderTargets(1,&_rtv, dsv);
}



// 通常の描画設定(陽炎の場所)
void PostEffectComponent::NormalDraw()
{
	//texFileNameが空なら描画しない
	if (texFileName != "") {

		Transform* tra = owner->transform;
		DirectX::XMFLOAT4X4 make, mtx, scale;

		// ワールド行列の作成
		DX11MtxScale(1, 1, 1, scale);
		DX11MtxFromQt(mtx, tra->qt);
		DX11MtxMultiply(make, scale, mtx);	


		// テクスチャをセット SRVの設定
		ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
		ID3D11ShaderResourceView* srv[] = {
			 ImageResourceManager::GetInstance()->GetResource(texFileName)->GetSRV() ,
		 ImageResourceManager::GetInstance()->GetResource(texSubFileName)->GetSRV() ,
		};

		devcontext->PSSetShaderResources(0, 2, srv);

		if (tra == nullptr) {
			MessageBox(nullptr, "NotFound <Transform> Component", "error!!", MB_OK);
		}

		// アニメーションの処理　　animationCompo使う
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

		//uv があるとき
		if (uv != NULL) {
			SetUVPos(uv[0], uv[1], uv[2], uv[3]);
		}
		// 親がいる時
		if (tra->GetParent() != nullptr) {
			Transform::GetMultiplParentTransform(make, tra);
		}
		
		// 通常時の描画設定
		postquad.NormalSetting();

		//  リストの最後取得してループ回す
		for (auto list : poslist) {
			if (list == NULL)break;
			//頂点データ、頂点バッファ更新
			postquad.SetMtx(make);
			postquad.updateVertex_3D(width, height, color, uv, 0.0f);
			postquad.updateVbuffer();
			postquad.SetPS(m_psmultistr);
			postquad.SetVS(m_vsmultistr);
			// 矩形を描画
			postquad.NormalDraw(&list);
		}
	}
}



// UVの設定
void PostEffectComponent::SetUVPos(DirectX::XMFLOAT2 left_top, DirectX::XMFLOAT2 right_top, DirectX::XMFLOAT2 left_bottom, DirectX::XMFLOAT2 right_bottom) {
	uv[0] = left_top;
	uv[1] = right_top;
	uv[2] = left_bottom;
	uv[3] = right_bottom;
}

//-----------------------------
// create by Suehara
//-----------------------------