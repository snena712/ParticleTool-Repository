#include "QuadRenderComponent.h"
#include "../DX11System/CDirectxGraphics.h"
#include "../DX11System/ImageResourceManager.h"
#include "AnimationComponent.h"
#include "ImGuiComponent.h"
#include "SceneManager.h"

using namespace Component;

Quad QuadRender::quad;
unsigned int QuadRender::counter;

QuadRender::QuadRender(GameObject* _owner, const char* _filename) :
	IRenderBase(_owner, RenderingType::Transparent),
	texFileName(_filename),
	width(1.0f),
	height(1.0f),
	uv{ {0.0f, 0.0f},{ 1.0f,0.0f }, { 0.0f,1.0f }, { 1.0f,1.0f } },
	color(1.0f, 1.0f, 1.0f, 1.0f),
	renderMode(Quad::RenderMode::DEFAULT),
	uvState(UV_STATE::DEFAULT),
	uvswitchCall(true)
{
	if (counter == 0) {
		quad.Init(width, height, color, uv, 0.0f);
	}

	counter++;

	bool sts = ImageResourceManager::GetInstance()->Load(_filename);
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", _filename);
		MessageBox(nullptr, str, "error", MB_OK);
		return;
	}
}

QuadRender::~QuadRender() {

	counter--;

	if (counter == 0) {
		quad.Exit();
	}
}

void QuadRender::Start()
{
	SceneManager::AddDrawFunction(mRenderingType, owner);
}

void QuadRender::DrawObject() {

	//texFileNameが空なら描画しない
	if (texFileName != "") {

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

		// テクスチャをセット
		ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
		ID3D11ShaderResourceView* srv[] = { ImageResourceManager::GetInstance()->GetResource(texFileName)->GetSRV() };
		devcontext->PSSetShaderResources(0, 1, srv);

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
		quad.updateVertex(width, height, color, uv, 0.0f);
		quad.updateVbuffer();

		// 位置と角度を設定
		if (tra != nullptr) {
			quad.SetMtx(make);
		}

		//描画モードの変更
		quad.SetRenderMode(renderMode);

		// 矩形を描画
		quad.Draw();
	}
}

#ifdef DEBUG

void QuadRender::ImGuiDebug()
{
	//プラスしてｃ＿ｓｔｒ
	std::string name = owner->GetName() + "R";
	auto* name2 = name.c_str();

	auto srv = ImageResourceManager::GetInstance()->GetResource(texFileName)->GetSRV();

	if (srv != nullptr) {

		ImGui::BeginChild(name2, ImVec2(0, 220), true);
	}
	else {
		ImGui::BeginChild(name2, ImVec2(0, 110), true);
	}
	ImGui::Text(u8"*Render");
	ImGui::Text(u8"現在の画像情報");

	auto* _str = texFileName.c_str();

	ImGui::Text(u8"画像名 : %s", _str);

	if (srv != nullptr) {
		//ImGui::Begin(u8"画像", &imageflg);
		ImGuiIO& io = ImGui::GetIO();

		float my_tex_w = 100;
		float my_tex_h = 100;

		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
		ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
		ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
		ImGui::Image((void*)srv, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
	}

	ImGui::EndChild();
}

#endif //DEBUG

void QuadRender::ChangeTexture(const char* _fileName) {

	bool sts;

	sts = ImageResourceManager::GetInstance()->Load(_fileName);
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", _fileName);
		MessageBox(nullptr, str, "error", MB_OK);
		return;
	}

	texFileName = _fileName;
}

//=================================================================//
//	頂点座標セット関数
//=================================================================//
void QuadRender::SetUVPos(float left, float right, float top, float bottom) {
	uv[0] = { left,top };
	uv[1] = { right,top };
	uv[2] = { left,bottom };
	uv[3] = { right,bottom };

	uvswitchCall = true;
}

void QuadRender::SetUVPos(DirectX::XMFLOAT2 left_top, DirectX::XMFLOAT2 right_top, DirectX::XMFLOAT2 left_bottom, DirectX::XMFLOAT2 right_bottom) {
	uv[0] = left_top;
	uv[1] = right_top;
	uv[2] = left_bottom;
	uv[3] = right_bottom;

	uvswitchCall = true;
}

void QuadRender::SetUVPos(DirectX::XMFLOAT2 split, DirectX::XMFLOAT2 pos) {

	DirectX::XMFLOAT2 uv_size = {
		100.0f / split.x / 100.0f,
		100.0f / split.y / 100.0f
	};

	uv[0] = { uv_size.x * pos.x,uv_size.y * pos.y };
	uv[1] = { uv_size.x * (pos.x + 1),uv_size.y * pos.y };
	uv[2] = { uv_size.x * pos.x,uv_size.y * (pos.y + 1) };
	uv[3] = { uv_size.x * (pos.x + 1),uv_size.y * (pos.y + 1) };

	uvswitchCall = true;
}

