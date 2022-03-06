#include "QuadMultiRenderComponent.h"
#include "../DX11System/CDirectxGraphics.h"
#include "../DX11System/ImageResourceManager.h"
#include "AnimationComponent.h"
#include "ImGuiComponent.h"
#include "SceneManager.h"

using namespace Component;

QuadMultiRender::QuadMultiRender(GameObject* Owner, const char* _filename, const char* _subfilename) :
	QuadRender(Owner, _filename),
	texSubFileName(_subfilename)
{
	bool sts = ImageResourceManager::GetInstance()->Load(_subfilename);
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", _subfilename);
		MessageBox(nullptr, str, "error", MB_OK);
		return;
	}
	colorVolume.volume = 1;
	ConstBufferInit();
}

QuadMultiRender::~QuadMultiRender() {

	//継承元でカウンタをマイナスするため、使用しなくなる時のカウンタの値は「１」
	if (counter == 1) {
		m_ColorVolumeBuffer->Release();
		m_ColorVolumeBuffer = nullptr;
	}
}

void QuadMultiRender::Start()
{
	SceneManager::AddDrawFunction(mRenderingType, owner);
}

void QuadMultiRender::DrawObject() {

	//texFileNameが空なら描画しない
	if (texFileName != "" && texSubFileName != "") {

		Transform* tra = owner->transform;
		DirectX::XMFLOAT4X4 make, mtx, scale;

		DX11MtxScale(tra->scale.x, tra->scale.y, tra->scale.z, scale);
		DX11MtxFromQt(mtx, tra->qt);
		DX11MtxMultiply(make, scale, mtx);

		make._41 = tra->position.x;
		make._42 = tra->position.y;
		make._43 = tra->position.z;

		if (tra->GetParent() != nullptr) {
			Transform::GetMultiplParentTransform(make, tra);
		}

		// テクスチャを各種セット 
		ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
		std::vector<ID3D11ShaderResourceView*> srv = { {
		 ImageResourceManager::GetInstance()->GetResource(texFileName)->GetSRV() ,
		 ImageResourceManager::GetInstance()->GetResource(texSubFileName)->GetSRV() ,
		 } };
		devcontext->PSSetShaderResources(0, static_cast<UINT>(srv.size()), srv.data());

		Component::Animation* ani = this->owner->GetComponent<Component::Animation>();

		if (ani != nullptr)
		{
			// UVの座標が変わった時
			if (ani->UvCheck())
			{	// 現在の親アニメーションのuv取得、UVのセット
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

		//uvStateの内容に応じてUV座標を反転させる
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

		//頂点データ、頂点バッファ更新
		quad.updateVertex(width, height, color, uv);
		quad.updateVbuffer();

		// 合成色の割合変更
		devcontext->UpdateSubresource(m_ColorVolumeBuffer, 0, nullptr, &colorVolume, 0, 0);
		devcontext->PSSetConstantBuffers(6, 1, &m_ColorVolumeBuffer);

		// 位置と角度を設定
		if (tra != nullptr) {
			quad.SetMtx(make);
		}

		//描画モードの変更
		quad.SetRenderMode(renderMode);

		// 矩形を描画
		quad.MultiDraw();
	}
}

void QuadMultiRender::ChangeSubTexture(const char* _fileName) {

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


void QuadMultiRender::ConstBufferInit()
{
	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// コンスタントバッファ作成
	bool sts = CreateConstantBuffer(
		device,			// デバイス
		sizeof(ColorVolume),	// サイズ
		&m_ColorVolumeBuffer);			// コンスタントバッファ２
	if (!sts) {
		MessageBox(NULL, "CreateBuffer(constant buffer world) error", "Error", MB_OK);
	}

}