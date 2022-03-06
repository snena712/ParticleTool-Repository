#include "ComputeComponent.h"
#include "../source/Compute/ComputeManager.h"
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")

using namespace Component;

ID3D11ShaderResourceView* const     g_pNullSRV = NULL;       // Helper to Clear SRVs
ID3D11UnorderedAccessView* const    g_pNullUAV = NULL;       // Helper to Clear UAVs

ComputeComponent::ComputeComponent(GameObject* _owner, const char* _filename, const char* _efeName) :IRenderBase(_owner, RenderingType::Transparent)
{
	// ファイル名の設定
	filename = _efeName;

	// パーティクル用の生成
	bool sts = ImageResourceManager::GetInstance()->Load(_filename);
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", _filename);
		MessageBox(nullptr, str, "error", MB_OK);
		return;
	}

	// 値の初期化
	Init();

	NormalLoad(filename);

	auto hoge = this;
	// このコンポーネントから自身送れる？
	ComputeManager::Instance()->SetComputeList(this);

	m_colorPictureName = "";

#if defined( TOOLMODE)
	// 矢印
	arrow = owner->AddComponent< Component::ArrowUI>();

	m_boxFrame = new GameObject();
	m_boxFrame->AddComponent<Component::BoxFrame>();
	m_boxFrame->SetExistState(false);
#endif
}



ComputeComponent::~ComputeComponent()
{
}

void ComputeComponent::Start()
{
	SceneManager::AddDrawFunction(mRenderingType, owner);

	// シェーダーや定数バッファ設定
	ID3D11DeviceContext* m_context = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// 定数バッファの更新
	m_context->Dispatch(1, 1, 1);
}

void ComputeComponent::Update()
{
	// ループの可否による更新
	NoLoopTimerUpdate();
	
	// パーティクルの更新
	ComputeUpdate();

	// 状態変化
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F1)) {
		Save();
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F2)) {	
	}
}

// シェーダーの設定＋実行の流れ
void ComputeComponent::ComputeUpdate()
{
}

// 初期化シェーダーに送る各種データ初期化
void ComputeComponent::Init()
{
	m_emitterName = "sample";

	//---------------
	// プラスの値の初期化
	//----------------
	plusData->rand.x = 1;
	plusData->rand.y = 0;
	plusData->rand.z = 0;
	plusData->speed = (float)5;
	plusData->vel.x = (float)0;
	plusData->vel.y = (float) - 1;
	plusData->vel.z = (float)0;
	plusData->life = (float)2;
	plusData->nowSpawnMaxNum = (float)1;
	plusData->spawnPos = XMFLOAT3(0,0,0);
	plusData->spawnPosRange = XMFLOAT3(0, 0, 0);
	plusData->gravity = 0;

	// サイズの初期化
	plusData->scaleAnim.initScale = XMFLOAT3(1,1,1);
	plusData->scaleAnim.finishScale = XMFLOAT3(1, 1, 1);

	plusData->velRange= XMFLOAT3(0, 0, 0);
	plusData->forward = XMFLOAT3(0, 0, 0);
	
	plusData->velocityFlg = 0;
	plusData->color.initColor = XMFLOAT4(1, 1, 1, 1);
	plusData->color.endColor = XMFLOAT4(1, 1, 1, 1);

	plusData->Live = (int)StatusCompute::Dead;

	//-------------
	// 粒子の初期化
	//-------------
	
	m_loopFlg = true;

	// 初期回転
	m_nowRot.x = -90;

	// 形の決定
	veloShape = Shape::Box;

	// 進行方向の計算
	UpdateVelocity();
}

// IMGuiの表示
void ComputeComponent::DebugImGui()
{
	ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(800, 0), ImGuiCond_Once);   //画面位置を外部から取得できるようにする

	// オブジェクトの数を表示
	ID3D11Device* m_device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* m_context = CDirectXGraphics::GetInstance()->GetImmediateContext();

	ImGui::Begin(m_emitterName.c_str(), &m_debugOpen);

	// 名前変更
	ImGui::Text(u8"エフェクト名 : %s", m_emitterName.c_str());	
	char* cstr = new char[m_emitterName.size() + 1]; // メモリ確保

	std::char_traits<char>::copy(cstr, m_emitterName.c_str(), m_emitterName.size() + 1);

	////static char buf[32] = u8"NIHONGO"; // <- this is how you would write it with C++11, using real kanjis
	ImGui::InputText("UTF-8 input", cstr, IM_ARRAYSIZE(cstr));
	
	m_emitterName = cstr;

	//ImGui::Text(u8"生成可能総パーティクル数 : %d", (int)ComputeManager::Instance()->GetParticleNum()->maxParticleNum);
	//ImGui::Text(u8"現在の生成総パーティクル数 : %d", (int)ComputeManager::Instance()->GetParticleNum()->spawnNum);

	std::string ownname = "パラメーター設定";
	const char* bbb = ownname.c_str();

	//if (ImGui::TreeNode(bbb)) {

	//ImGui::BeginChild(bbb, ImVec2(0, 60), false);
	ImGui::Text("%s", bbb);


	//ImGui::SameLine();
	//ImGui::Checkbox("open?", _objList.obj->GetDebugAdress());	対応するフラグのオンオフができる

	ImGui::Checkbox(u8"ループ", &m_loopFlg);
	
	ImGui::InputFloat(u8"ライフ", (float*)&plusData->life);
	ImGui::InputFloat(u8"生成時間", &m_liveTime);

	ImGui::InputInt(u8"Rate over Time", &m_stackSpawnNum);
	
	//ImGui::EndChild();

	// 位置
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::InputFloat3(u8"生成場所", (float*)&plusData->spawnPos);
		ImGui::InputFloat3(u8"生成場所範囲", (float*)&plusData->spawnPosRange);

		// 生成の方法と移動方法
		const char* items[] = { "Local", "World" };
		ImGui::Combo("Simulation Space", &m_simulationCnt, items, IM_ARRAYSIZE(items));
		ImGui::SameLine(); Helper::HelpMarker(
			u8"Localはエミッターに追従する。Worldはエミッターに追従しない");

		if (ComputeManager::Instance()->GetDrawStatus() == DrawStatus::Second) {
			ImGui::InputFloat2(u8"初期サイズ", (float*)& plusData->scaleAnim.initScale);
			ImGui::InputFloat2(u8"終了サイズ", (float*)& plusData->scaleAnim.finishScale);
		}
		else {
			ImGui::InputFloat3(u8"初期サイズ", (float*)& plusData->scaleAnim.initScale);
			ImGui::InputFloat3(u8"終了サイズ", (float*)& plusData->scaleAnim.finishScale);
		}

		ImGui::InputFloat3(u8"角度", (float*)&m_nowRot);	

		// ビットの更新
		BitFlgUpdate();
	}

	// 力
	if (ImGui::CollapsingHeader("Velocity"))
	{
		//	ImGui::BeginChild(bbb, ImVec2(0, 60), false);
		ImGui::InputFloat(u8"力", (float*)&plusData->speed);
		ImGui::InputFloat3(u8"加速度", (float*)&plusData->vel);
		ImGui::InputFloat3(u8"移動量のランダム", (float*)&plusData->velRange);
		ImGui::InputFloat(u8"重力", (float*)&plusData->gravity);

		const char* items[] = { "Box", "Sphere" };

		ImGui::Combo("Velocity Shape", &item_current, items, IM_ARRAYSIZE(items));
		ImGui::SameLine(); Helper::HelpMarker(
			u8"Boxは回転可能。Sphereは回転不可");

		ImGui::Checkbox(u8"波", (bool*)&waveFlg);

		ImGui::Checkbox(u8"色による移動", (bool*)&colorMoveFlg);
		ImGui::SameLine(); Helper::HelpMarker(
			u8"パーティクルの色に応じた移動");
		
		// ビットの更新
		BitFlgUpdate();
	}

	if (ImGui::CollapsingHeader("Color"))
	{
		const char* items[] = { "Normal", "Picture" };
		ImGui::Combo("ColorType", &m_colorPictureCnt, items, IM_ARRAYSIZE(items));
		
		// ビットの更新
		BitFlgUpdate();

		switch (colorControl)
		{
		case ColorControl::Normal:
			ImGuiColorEditFlags flags;
			flags = ImGuiColorEditFlags_AlphaBar;
			ImGui::ColorEdit4("SpawnColor", (float*)&plusData->color.initColor, flags); 
			ImGui::ColorEdit4("EndColor", (float*)&plusData->color.endColor, flags);

			break;
		case ColorControl::Picture:
			DrawPictureColor();
			break;
		default:
			break;
		}			
	}
	
	// 終了	
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(500, 200), ImGuiCond_Once);   //画面位置を外部から取得できるようにする
	ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(800, 0), ImGuiCond_Once);   //画面位置を外部から取得できるようにする
	ImGui::Begin("再生", 0);
	
	if (ImGui::SmallButton(u8"再生開始")) {
		ComputeManager::Instance()->SetStopFlg(false);
		m_startFlg = !m_startFlg;
	};
	if (ImGui::SmallButton(u8"経過時間リセット"))m_elapsedTime = 0;
	
	auto _flg = ComputeManager::Instance()->GetStopFlg();
	if(ImGui::Checkbox(u8"再生停止", _flg))
	{
		ComputeManager::Instance()->SetStopFlg(*_flg);
		m_startFlg = false;
	}
	ImGui::Text(u8"経過時間　:%f", m_elapsedTime);	
	ImGui::InputFloat(u8"経過時間速度", (float*)&ComputeManager::Instance()->GetTimeSpeed());

	ImGui::End();

	// 動きの更新
	UpdateVelocity();							
}

void ComputeComponent::BitFlgUpdate()
{
	// 形の管理
	switch (item_current)
	{
	case 0:
		veloShape = Shape::Box;
		BitFlgChanger(plusData->velocityFlg, Velocity_Event_SHAPE_BOX, BitControl::ON);
		break;
	case 1:
		veloShape = Shape::Sphere;
		BitFlgChanger(plusData->velocityFlg, Velocity_Event_SHAPE_BOX, BitControl::OFF);
		break;
	default:
		break;
	}

	// 色変更
	switch (m_colorPictureCnt)
	{
	case 0:
		colorControl = ColorControl::Normal;
		BitFlgChanger(plusData->colorFlg, Color_Event_PictureColor, BitControl::ON);
		break;
	case 1:
		colorControl = ColorControl::Picture;
		BitFlgChanger(plusData->colorFlg, Color_Event_PictureColor, BitControl::OFF);
		break;
	default:
		break;
	}

	// シミュレーションの方法
	switch (m_simulationCnt)
	{
		// ローカル
	case 0:
		BitFlgChanger(plusData->velocityFlg, Velocity_Simulation_Space, BitControl::OFF);
		break;
		// ワールド
	case 1:
		BitFlgChanger(plusData->velocityFlg, Velocity_Simulation_Space, BitControl::ON);
		break;
	default:
		break;
	}

	// 波
	if (waveFlg)
	{
		BitFlgChanger(plusData->velocityFlg, Velocity_Event_Sin, BitControl::ON);
	}
	else {
		BitFlgChanger(plusData->velocityFlg, Velocity_Event_Sin, BitControl::OFF);
	}

	// 色による移動
	if (colorMoveFlg)
	{
		BitFlgChanger(plusData->velocityFlg, Velocity_Event_ColorMove, BitControl::ON);
	}
	else {
		BitFlgChanger(plusData->velocityFlg, Velocity_Event_ColorMove, BitControl::OFF);
	}
}

// 保存
void ComputeComponent::Save()
{
	const char* name = m_emitterName.c_str();
	if (ImGui::MenuItem(name))
	{
		TCHAR strFile[MAX_PATH * 256];
		if (WindowFileDealer::WindowsFileSave(0, strFile, sizeof(strFile) / sizeof(TCHAR)))
		{
			MessageBox(0, strFile, _TEXT("選択されたファイル名"), MB_OK);
			//m_emitterName = "testname";

			std::string filename= strFile;

			// ファイル名のディレクトリ

			/*if (m_emitterName != "") {
				filename = "assets/EfeFile/" + m_emitterName + "." + fileFormat;
			}
			else {
				m_emitterName = "NO_NAME";
				filename = "assets/EfeFile/" + m_emitterName + "." + fileFormat;
			}*/
			// ファイルの開く。なければ作成。
			std::ofstream writing_file;
			writing_file.open(filename, std::ios::out);

			std::cout << "writing " << filename << "..." << std::endl;


			writing_file << nameStr << ":" << m_emitterName << std::endl;
			writing_file << spawnNumStr << ":" << plusData->nowSpawnMaxNum << std::endl;
			writing_file << lifeStr << ":" << plusData->life << std::endl;
			writing_file << liveTimeSrt << ":" << m_liveTime << std::endl;
			writing_file << loopFlgSrt << ":" << m_loopFlg << std::endl;
			writing_file << velStr << ":" << plusData->vel.x << "," << plusData->vel.y << "," << plusData->vel.z << "," << std::endl;

			writing_file << scaleAnim_initScaleStr << ":" << plusData->scaleAnim.initScale.x << "," << plusData->scaleAnim.initScale.y << "," << plusData->scaleAnim.initScale.z << "," << std::endl;
			writing_file << scaleAnim_finishScaleStr << ":" << plusData->scaleAnim.finishScale.x << "," << plusData->scaleAnim.finishScale.y << "," << plusData->scaleAnim.finishScale.z << "," << std::endl;

			writing_file << spawnPosStr << ":" << plusData->spawnPos.x << "," << plusData->spawnPos.y << "," << plusData->spawnPos.z << std::endl;
			
			writing_file << spawnPosRangeStr << ":" << plusData->spawnPosRange.x << "," << plusData->spawnPosRange.y << "," << plusData->spawnPosRange.z << std::endl;			
			writing_file << velRangeStr << ":"<<plusData->velRange.x << "," << plusData->velRange.y << "," << plusData->velRange.z << std::endl;
			writing_file << speedStr << ":" << plusData->speed<< std::endl;
			writing_file << forwardStr << ":" << plusData->forward.x << "," << plusData->forward.y << "," << plusData->forward.z << std::endl;
			writing_file << gravityStr << ":" << plusData->gravity << std::endl;
			writing_file << velFlgStr << ":" << plusData->velocityFlg << std::endl;
			writing_file << InitColorStr << ":" << plusData->color.initColor.x << "," << plusData->color.initColor.y << "," << plusData->color.initColor.z << "," << plusData->color.initColor.w << std::endl;
			writing_file << EndColorStr << ":" << plusData->color.endColor.x << "," << plusData->color.endColor.y << "," << plusData->color.endColor.z << "," << plusData->color.endColor.w << std::endl;

			//writing_file << "vel:" << plusData. << "," << plusData->vel.y << "," << plusData->vel.z << "," << std::endl;

		}
		// ○○の部分はユーザー名
	}

	return;
}

// 読み込み
void ComputeComponent::WindowsLoad()
{
	TCHAR szFile[MAX_PATH * 256];
	if (WindowFileDealer::WindowsFileLoad(0, szFile, sizeof(szFile) / sizeof(TCHAR)))
	{
		MessageBox(0, szFile, _TEXT("選択されたファイル名"), MB_OK);

		// ファイル名のディレクトリ
		//std::string filename = "assets/EfeFile/" + m_emitterName + "." + fileFormat;
		std::string filename = szFile;

		LoadDeal(filename);
	}
	// 進行方向の更新
	UpdateVelocity();

}

void ComputeComponent::NormalLoad(std::string _str)
{
	// 
	LoadDeal(_str);
	
	BitFlgUpdate();

	// 進行方向の更新
	UpdateVelocity();
}

void ComputeComponent::LoadDeal(std::string _str)
{
	std::ifstream reading_file;
	reading_file.open(_str, std::ios::in);

	std::string reading_line_buffer;

	std::cout << "reading " << _str << "..." << std::endl;

	while (!reading_file.eof())
	{
		// read by line
		std::getline(reading_file, reading_line_buffer);

		if (reading_line_buffer == "")break;

		std::cout << reading_line_buffer << std::endl;


		//std::cout << separated_string_buffer << std::endl;
		auto str = Helper::splitString(reading_line_buffer, ':');

		// 名前の設定
		if (str[0] == nameStr)
		{
			// 名前が被る場合は異なるものを出す
			m_emitterName = ComputeManager::Instance()->ListNameCheck(str[1]);
		}

		if (str[0] == spawnNumStr)
		{
			// 名前が被る場合は異なるものを出す
			plusData->nowSpawnMaxNum = std::stof(str[1]);
			m_stackSpawnNum = plusData->nowSpawnMaxNum;
		}

		// 体力
		if (str[0] == lifeStr)
		{
			plusData->life = std::stof(str[1]);
		}

		// エミッターの生成時間
		if (str[0] == liveTimeSrt)
		{
			m_liveTime = std::stof(str[1]);
		}

		// エミッターのループ
		if (str[0] == loopFlgSrt)
		{
			m_loopFlg = std::stof(str[1]);
		}

		// 力
		if (str[0] == velStr)
		{
			auto vel = Helper::splitString(str[1], ',');
			plusData->vel.x = std::stof(vel[0]);
			plusData->vel.y = std::stof(vel[1]);
			plusData->vel.z = std::stof(vel[2]);
		}

		// 場所
		if (str[0] == spawnPosStr)
		{
			auto hoge = Helper::splitString(str[1], ',');
			plusData->spawnPos.x = std::stof(hoge[0]);
			plusData->spawnPos.y = std::stof(hoge[1]);
			plusData->spawnPos.z = std::stof(hoge[2]);
		}

		// 生成範囲
		if (str[0] == spawnPosRangeStr)
		{
			auto hoge = Helper::splitString(str[1], ',');
			plusData->spawnPosRange.x = std::stof(hoge[0]);
			plusData->spawnPosRange.y = std::stof(hoge[1]);
			plusData->spawnPosRange.z = std::stof(hoge[2]);
		}

		// サイズ
		if (str[0] == scaleAnim_initScaleStr)
		{
			auto hoge = Helper::splitString(str[1], ',');
			plusData->scaleAnim.initScale.x = std::stof(hoge[0]);
			plusData->scaleAnim.initScale.y = std::stof(hoge[1]);
			plusData->scaleAnim.initScale.z = std::stof(hoge[2]);
		}

		if (str[0] == scaleAnim_finishScaleStr)
		{
			auto hoge = Helper::splitString(str[1], ',');
			plusData->scaleAnim.finishScale.x = std::stof(hoge[0]);
			plusData->scaleAnim.finishScale.y = std::stof(hoge[1]);
			plusData->scaleAnim.finishScale.z = std::stof(hoge[2]);
		}

		if (str[0] == speedStr)
		{
			plusData->speed = std::stof(str[1]);
		}

		if (str[0] == velRangeStr)
		{
			auto hoge = Helper::splitString(str[1], ',');
			plusData->velRange.x = std::stof(hoge[0]);
			plusData->velRange.y = std::stof(hoge[1]);
			plusData->velRange.z = std::stof(hoge[2]);
		}

		if (str[0] == forwardStr)
		{
			auto hoge = Helper::splitString(str[1], ',');
			plusData->forward.x = std::stof(hoge[0]);
			plusData->forward.y = std::stof(hoge[1]);
			plusData->forward.z = std::stof(hoge[2]);
		}

		if (str[0] == gravityStr)
		{
			plusData->gravity = std::stof(str[1]);
		}

		if (str[0] == velFlgStr)
		{
			plusData->velocityFlg = std::stof(str[1]);
			
			// 状態
			if (GetBitFlg(plusData->velocityFlg, Velocity_Event_SHAPE_BOX))
			{
				item_current = 0;
			}
			else {
				item_current = 1;
			}

	/*		if (GetBitFlg(plusData->colorFlg, Velocity_Event_ColorMove))
			{
				m_colorPictureCnt = 0;
			}
			else {
				m_colorPictureCnt = 1;
			}*/

			if (GetBitFlg(plusData->velocityFlg, Velocity_Simulation_Space))
			{
				m_simulationCnt = 1;
			}
			else {
				m_simulationCnt = 0;
			}
		}

		if (str[0] == InitColorStr)
		{
			auto hoge = Helper::splitString(str[1], ',');
			plusData->color.initColor.x = std::stof(hoge[0]);
			plusData->color.initColor.y = std::stof(hoge[1]);
			plusData->color.initColor.z = std::stof(hoge[2]);
			plusData->color.initColor.w = std::stof(hoge[3]);
		}

		if (str[0] == EndColorStr)
		{
			auto hoge = Helper::splitString(str[1], ',');
			plusData->color.endColor.x = std::stof(hoge[0]);
			plusData->color.endColor.y = std::stof(hoge[1]);
			plusData->color.endColor.z = std::stof(hoge[2]);
			plusData->color.endColor.w = std::stof(hoge[3]);
		}
	}
}


void ComputeComponent::NoLoopTimerUpdate()
{
	// ループ選択時
	auto comMana = ComputeManager::Instance();
	// 再生フラグ
	if (m_startFlg)
	{
		auto _f = ComputeManager::Instance()->GetStopFlg();
		// 停止フラグ
		if (!*_f)
		{		
			m_elapsedTime += Time::GetInstance()->GetDeltaTime() * ComputeManager::Instance()->GetTimeSpeed();
			plusData->nowSpawnMaxNum = m_stackSpawnNum;

			if (m_elapsedTime < m_liveTime)
			{
				plusData->Live = (int)StatusCompute::Live;
			}
			else {
				if (!m_loopFlg)
				{					
					m_startFlg = false;
					m_elapsedTime = 0;
					plusData->Live = (int)StatusCompute::Dead;
				}
				else {
					plusData->Live = (int)StatusCompute::Live;
				}
			}
		}
		else {
			plusData->nowSpawnMaxNum = 0;
		}
	}
	else {
		plusData->Live = (int)StatusCompute::Dead;
	}

}

// このIMGuiアイコンを触った時の対応
bool ComputeComponent::DebugImGuiOpen()
{
	const char* name = m_emitterName.c_str();
		
	// ダブルクリック時に出てきてしまう
	if (ImGui::Selectable(name, m_debugOpen, ImGuiSelectableFlags_AllowDoubleClick))
	{
		if (ImGui::IsMouseDoubleClicked(0))
		{
			Vector3 _pos;
			_pos.x = plusData->spawnPos.x;
			_pos.y = plusData->spawnPos.y;
			_pos.z = plusData->spawnPos.z;
			ComputeManager::Instance()->SetLookEmitter(_pos);
			return false;
		}
		else {
			m_debugOpen = !m_debugOpen;
			return m_debugOpen;
		}
	}
	return false;
}

// 移動量の更新
void ComputeComponent::UpdateVelocity()
{
	auto _qt = XMFLOAT4(0, 0, 0, 1);

	// x回転
	DirectX::XMFLOAT4 q, axis;
	axis = { 1,0,0,0 };
	DX11QtRotationAxis(q, axis, m_nowRot.x);
	DX11QtMul(_qt, _qt, q);

	// y回転
	axis = { 0,1,0,0 };
	DX11QtRotationAxis(q, axis, m_nowRot.y);
	DX11QtMul(_qt, _qt, q);

	// z回転
	axis = { 0,0,1,0 };
	DX11QtRotationAxis(q, axis, m_nowRot.z);
	DX11QtMul(_qt, _qt, q);

	// オブジェクトの姿勢と合成
	//_qt.x = owner->transform->qt.x*_qt.x;
	//_qt.y = owner->transform->qt.y*_qt.y;
	//_qt.z = owner->transform->qt.z*_qt.z;
	//_qt.w = owner->transform->qt.w*_qt.w;

	//////////////////////////////////////////////
	DirectX::XMFLOAT4X4 make, mat;

	DX11MtxIdentity(make);

	// 子供
	DX11MtxFromQt(mat, _qt);
	DX11MtxMultiply(make, make, mat);

	// 親
	DX11MtxFromQt(mat, owner->transform->qt);
	DX11MtxMultiply(make, make, mat);

	DirectX::XMFLOAT4 t;
	DX11GetQtfromMatrix(make, t);



	///////////////////////////////////////////////////////

	//printf(" X:%f Y:%f Z:%f W:%f\n", _qt.x, _qt.y,_qt.z, _qt.w);
	printf(" X:%f Y:%f Z:%f \n", make._31, make._32, make._33);
	//owner->transform->RotateXaxis(1);

	// 前方向の計算
	plusData->forward = Vector3{ make._31,make._32,make._33 };

	Vector3 _pos;
	_pos.x = plusData->spawnPos.x;
	_pos.y = plusData->spawnPos.y;
	_pos.z = plusData->spawnPos.z;
	owner->transform->position = _pos;
}

// ImGuiで色の画像描画
void ComputeComponent::DrawPictureColor()
{
	if (ImGui::SmallButton(u8"画像"))
	{
		LoadColorPicture();

		colorPictureSRV = ImageResourceManager::GetInstance()->GetResource(m_colorPictureName)->GetSRV();
		ComputeManager::Instance()->SetColorPictureSRV(colorPictureSRV.Get());
	}


	if (colorPictureSRV != nullptr) {
		//ImGui::Begin(u8"画像", &imageflg);
		ImGuiIO& io = ImGui::GetIO();

		float my_tex_w = 100;
		float my_tex_h = 5;

		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
		ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
		ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
		ImGui::Image((void*)colorPictureSRV.Get(), ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
	}
}

// 画像取得
void ComputeComponent::LoadColorPicture()
{
	TCHAR szFile[MAX_PATH * 256];
	if (WindowFileDealer::WindowsPNGFileLoad(0, szFile, sizeof(szFile) / sizeof(TCHAR)))
	{
		MessageBox(0, szFile, _TEXT("選択されたファイル名"), MB_OK);

		// ファイル名のディレクトリ
		//std::string filename = "assets/EfeFile/" + m_emitterName + "." + fileFormat;
		
		m_colorPictureName = szFile;
		ImageResourceManager::GetInstance()->Load(m_colorPictureName);
		GetPictureSize(szFile);
	}
}


// 画像サイズの取得
XMFLOAT2 ComputeComponent::GetPictureSize(std::string _str)
{
	auto pSrc = _str.c_str();
	const unsigned int dataSize = _str.size();
	std::vector< wchar_t> wlocal;
	wlocal.resize(dataSize+1);

	int ret = MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED,
		pSrc,
		strlen(pSrc),
		wlocal.data(),
		strlen(pSrc) + 1);

	// WIC使用のための呼び出し
	CoInitialize(NULL);

	// ファクトリを作成
	IWICImagingFactory* factory;
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)(&factory));
	
	// テクスチャの読み込み
	IWICBitmapDecoder* decoder;
	factory->CreateDecoderFromFilename(wlocal.data(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
	

	// デコードフレームを取得
	IWICBitmapFrameDecode* frame;
	decoder->GetFrame(0, &frame);

	// フォーマットコンバータを作成
	IWICFormatConverter* formatConv;
	factory->CreateFormatConverter(&formatConv);

	// フォーマット初期化
	formatConv->Initialize(frame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, NULL, 1.0f, WICBitmapPaletteTypeMedianCut);

	// 画像サイズを取得
	UINT imageWidth;
	UINT imageHeight;
	formatConv->GetSize(&imageWidth, &imageHeight);

	XMFLOAT2 _size;
	// 画像サイズを格納
	_size.x= static_cast<FLOAT>(imageWidth);
	_size.y = static_cast<FLOAT>(imageHeight);

	return _size;
}

//-----------------------------
// create by Suehara
//-----------------------------