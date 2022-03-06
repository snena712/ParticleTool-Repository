#include "ComputeManager.h"

ID3D11Buffer* const                 g_pNullBuffer = NULL;    // Helper to Clear Buffers
UINT                                g_iNullUINT = 0;         // Helper to Clear Buffers

void ComputeManager::Init()
{
	// 画像情報入れ込み
	m_filename = "assets/Image/Particle/ParticleCircle.png";
	ImageResourceManager::GetInstance()->Load(m_filename);

	m_noiseTex = "assets/Image/Noise/noise.jpg";
	// テクスチャをセット(乱数用)
	ImageResourceManager::GetInstance()->Load(m_noiseTex);

	// ワールド変換行列セット
	DX11MtxIdentity(worldmtx);	//ワールド変換行列を単位行列にする

	// データの初期化
	InitDate();

	// Buffer
	InitBuffer();

	// シェダーの初期化
	InitShader();

	// ブレンド
	CreateBlendState();
	CreateSampleState();

	// 3d用のモデル
	GameObject*sphere = new GameObject;
	sphere->AddComponent<Component::SphereModel>();
	model = sphere->GetComponent<Component::ModelRenderInstance>();

	initEnd = true;
}

void ComputeManager::InitDate()
{
	// 数の初期化
	m_particleNum = new ParticleNum;
	m_particleNum->nowSpawnNum = 0;
	m_particleNum->spawnNum = 0;
	m_particleNum->maxParticleNum = NUM_ELEMENTS;
	m_particleNum->seed = 1;

	// ソート用変数
	sortStruct = new SortStruct;

	// 各パーティクルの初期化
	m_emitter = new ParticleBase[NUM_ELEMENTS];

	m_particleDraw = new ParticleDraw[NUM_ELEMENTS];

	// 開始時間
	m_startTime = Time::GetInstance()->GetDeltaTime();

	for (int i = 0; i < NUM_ELEMENTS; ++i)
	{
		m_emitter[i].pos.x = 0;
		m_emitter[i].pos.y = 0;

		m_emitter[i].pos.z = 0;

		m_emitter[i].vel.x = 0;
		m_emitter[i].vel.y = 0;
		m_emitter[i].vel.z = 0;

		m_emitter[i].life = (float)-1;

		// 死亡
		m_emitter[i].Live = 0;

		// サイズ
		m_emitter[i].scale = Vector3(0, 0, 0);

		// 親設定
		m_emitter[i].parentId = 1000;
		m_emitter[i].dummy.x = -1000;
		m_emitter[i].dummy.y = -1000;
		m_emitter[i].dummy.z = -1000;
	}

#if defined( TOOLMODE)
	//升目
	tile = new GameObject();
	tile->AddComponent<Component::TileLine>();
	SceneManager::Instance()->AddObject(tile, true);
#endif
}

void ComputeManager::InitBuffer()
{
	// バーティクルの基礎
	m_particleBuffer = new StructuredBuffer();
	m_particleBuffer->CreateStructuredBuffer(NUM_ELEMENTS, m_emitter);

	// バーティクルの基礎の計算結果取得
	m_resultBuffer = new StructuredBuffer();
	m_resultBuffer->CreateStructuredBuffer(NUM_ELEMENTS, m_particleDraw);

	m_resultTestBuffer = new StructuredBuffer();
	m_resultTestBuffer->CreateStructuredBuffer(NUM_ELEMENTS, m_particleDraw);

	// ソート管理
	m_SortBuffer = new ConstBuffer();
	m_SortBuffer->CreateConStructuredBuffer<SortStruct>(ConstBuffer::Type::Multi,1);

	// 数管理バッファ
	m_particleNumBuffer = new StructuredBuffer();
	m_particleNumBuffer->CreateStructuredBuffer(1, m_particleNum);

	// 定数バッファの更新
	// 計算用のバッファの初期化（配列）
	m_plusBuffer = new ConstBuffer();
	m_plusBuffer->CreateConStructuredBuffer<PlusData>(ConstBuffer::Type::Multi,PlusBufNum);
}

void ComputeManager::InitShader()
{
	auto _device = CDirectXGraphics::GetInstance()->GetDXDevice();

	Helper::InitVertexShader(_device,
		"shader/FluidRender.hlsl",
		"ParticleVS",
		"vs_5_0",
		m_pParticleVS.GetAddressOf());

	Helper::InitPixelShader(_device,
		"shader/FluidRender.hlsl",
		"ParticleBillbord",
		"ps_5_0",
		m_pParticlePS.GetAddressOf());

	Helper::InitGeometryShader(_device,
		"shader/FluidRender.hlsl",
		"ParticleBillbordGS",
		"gs_5_0",
		m_pParticleGS.GetAddressOf());

	// ソート
	Helper::InitComputeShader(_device,
		"shader/ComputeShader_v2.hlsl",
		"BitonicSortLive",
		"cs_5_0",
		&m_pSortCS);

	// 初期化
	Helper::InitComputeShader(_device,
		"shader/ComputeShader_v2.hlsl",
		"InitCS",
		"cs_5_0",
		&m_pInitCS);

	// コンピュートシェーダーの生成　（ベースのもの）
	Helper::InitComputeShader(_device,
		"shader/ComputeShader_v2.hlsl",
		"CS",
		"cs_5_0",
		&m_pParticleCS);

	// コンピュートシェーダーの生成　（生成するもの）
	Helper::InitComputeShader(_device,
		"shader/ComputeShader_v2.hlsl",
		"SpawnCS",
		"cs_5_0",
		&m_pSpawnCS);

	// 死亡
	Helper::InitComputeShader(_device,
		"shader/ComputeShader_v2.hlsl",
		"DeadCheck",
		"cs_5_0",
		&m_pDeadCS);
}

void ComputeManager::Update()
{
	// CSの更新
	UpdateCS();

	// 不要な設定の初期化
	Reset();
}

//void ComputeManager::ResetDate()
//{
//	InitDate();
//	computeList.clear();
//}

void ComputeManager::UpdateCS()
{
	// -------------------------------------------------------------------------------------------------
	ID3D11DeviceContext* m_context = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// SRVとUAVの設定
	m_noiseTex = "assets/Image/Noise/noise.jpg";
	// テクスチャをセット(画像、乱数、色)
	ID3D11ShaderResourceView* sRViews[4] = { GetStructuredBuffer()->GetSRV(), 
		ImageResourceManager::GetInstance()->GetResource(m_noiseTex)->GetSRV(),
		nullptr,
		colorPictureSRV.Get() };

	ComputeManager::Instance()->FirstCS();

	// サンプラーのセット
	m_context->CSSetSamplers(0, 1, &m_sampler);

	// SRVの実行
	m_context->CSSetShaderResources(0, 4, sRViews);

#if defined( TOOLMODE)&&defined(DEBUG)
	// Imguiの表示
	ImGui();
#endif

	// バッファの更新
	SetBufferPlusDate();

	// 生成処理
	m_context->CSSetShader(GetCSSpawnShader(), NULL, 0);
	m_context->Dispatch(1, 1, 1);

	// 死亡処理
	m_context->CSSetShader(GetCSDeadShader(), NULL, 0);
	m_context->Dispatch(1, 1, 1);
	
	// 基本更新のシェーダーセット
	m_context->CSSetShader(GetCSShader(), NULL, 0);

	// 定数バッファの更新
	m_context->Dispatch(NUM_ELEMENTS/Thread, 1, 1);
}

void ComputeManager::Draw()
{	
	if (!initEnd)return;
	DirectX::XMFLOAT4X4 mtx;

	// 各CSの処理
	BeforeDrawCS();

	// バッファをSRVにセット
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	ID3D11ShaderResourceView* aRViews[2] = { m_resultTestBuffer->GetSRV(), nullptr };

	// バッファ初期化
	ID3D11Buffer* ppCBNULL[2] = { NULL,NULL };
	devcontext->VSSetConstantBuffers(0, 2, ppCBNULL);

	// VSにSRVセット
	devcontext->VSSetShaderResources(0, 2, aRViews);

	auto _setTrans = DX11SetTransform::GetInstance();
	auto _camera = Component::Camera::GetMainCamera();
	_setTrans->SetTransform(DX11SetTransform::TYPE::WORLD, worldmtx);

	// ビュー変換行列セット
	mtx = _camera->GetCameraMatrix();
	_setTrans->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	// プロジェクション変換行列セット
	mtx = _camera->GetProjectionMatrix();
	_setTrans->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	// 視点の位置設定
	DX11LightInit(Component::Camera::GetMainCamera()->GetOwner()->transform->position);

	// 画像のSRV設定
	ID3D11ShaderResourceView* srv = ImageResourceManager::GetInstance()->GetResource(m_filename)->GetSRV();

	// PSにSRVの設定
	devcontext->PSSetShaderResources(2, 1, &srv);

	// 頂点シェーダー、ピクセルシェーダー、頂点レイアウト取得		
	devcontext->VSSetShader(m_pParticleVS.Get(), nullptr, 0);
	devcontext->GSSetShader(m_pParticleGS.Get(), nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(m_pParticlePS.Get(), nullptr, 0);

	devcontext->IASetVertexBuffers(0, 1, &g_pNullBuffer, &g_iNullUINT, &g_iNullUINT);   //NULL
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);             // リスト

	 // ブレンドセット
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	//アルファブレンドをONにする
	//devcontext->OMSetBlendState(m_blendState, blendFactor, 0xffffffff);

	// サンプラーのセット
	//devcontext->PSSetSamplers(0, 1, &m_sampler);

	// VSに定数のセット
	m_plusBuffer->SetVSBuffer(6);




	// レンダーターゲットの設定
	if (m_bloomFlg) {
		auto _rtv =PostEffect::BloomPostEffect::Instance()->GetSRVDefault().GetRTV();
		devcontext->OMSetRenderTargets(1, &_rtv, nullptr);
	}
	else {
		auto _rtv = DirectGraphicsTexture::GetInstance()->GetRTV();
		devcontext->OMSetRenderTargets(1, &_rtv, nullptr);
	}
	//// レンダーターゲットの色初期化
	//m_rt2d->ClearColorRenderTarget();


	// ドローコール発行（描画物の種類）
	switch (m_drawStatus)
	{
	case DrawStatus::Second:
		devcontext->Draw(
			NUM_ELEMENTS,// インデックス数
			0);				// 基準頂点インデックス
		devcontext->GSSetShader(nullptr, nullptr, 0);
		break;

	case DrawStatus::Third:
		devcontext->GSSetShader(nullptr, nullptr, 0);

		model->DrawSetting();
		model->Draw(NUM_ELEMENTS);

		break;
	default:
		break;
	}

	// レンダーターゲットの設定
	auto _dafaultRTV = DirectGraphicsTexture::GetInstance()->GetRenderTarget2D().GetRTV();
	auto _dafaultDSV = CDirectXGraphics::GetInstance()->GetDepthStencilView();
	devcontext->OMSetRenderTargets(1, &_dafaultRTV, _dafaultDSV);
}


void ComputeManager::AfterDraw()
{
#if defined( TOOLMODE)
	

	auto _devContext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	auto _rtv = CDirectXGraphics::GetInstance()->GetRenderTargetView();
	_devContext->OMSetRenderTargets(1, &_rtv, nullptr);

	// エミッターの矢印描画
	if (*particleArrowDrawFlg) {
		if (stackCompute != nullptr) {
			stackCompute->ArrowDraw();
		}
	}
	if (*particleRangeDrawFlg) {
		if (stackCompute != nullptr) {
			stackCompute->BoxFrameDraw();
		}
	}
	
	_rtv = DirectGraphicsTexture::GetInstance()->GetRTV();
	_devContext->OMSetRenderTargets(1, &_rtv, CDirectXGraphics::GetInstance()->GetDepthStencilView());
#endif
}

void ComputeManager::BeforeDrawCS()
{
	//========================
	// ソート処理
	//========================
	ID3D11DeviceContext* m_context = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// 関数
	// シェーダーや定数バッファ設定
	m_context->CSSetShader(GetCSSortShader(), NULL, 0);
	// 定数バッファの更新

	BitonicSort(0);

#ifdef TOOLMODE
	// バッファのコピー
	CopyParticleNumBuffer();
#endif

	// データのコピー
	Copy();
}

void ComputeManager::Reset()
{
	// 更新削除
	ID3D11DeviceContext* m_context = CDirectXGraphics::GetInstance()->GetImmediateContext();
	m_context->CSSetShader(NULL, NULL, 0);
	ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
	m_context->CSSetUnorderedAccessViews(0, 1, ppUAViewNULL, NULL);
	ID3D11ShaderResourceView* ppSRVNULL[2] = { NULL, NULL };
	m_context->CSSetShaderResources(0, 2, ppSRVNULL);
	ID3D11Buffer* ppCBNULL[2] = { NULL,NULL };
	m_context->CSSetConstantBuffers(0, 2, ppCBNULL);
}

// データのコピー
void ComputeManager::Copy()
{
	// -------------------------------------------------------------------------------------------------
	ID3D11Device* m_device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* m_context = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// コピー用のバッファ作成とコピー
	ID3D11Buffer* debugbuf = ComputeManager::Instance()->CreateAndCopyToDebugBuf(m_device, m_context, m_resultBuffer->GetBuffer());
	//ID3D11Buffer* debugbuf = CreateAndCopyToDebugBuf(m_device, m_context, m_resultBuffer->GetBuffer());
	D3D11_MAPPED_SUBRESOURCE MappedResource;

	m_context->Map(debugbuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);

	//バッファの中身をコピー（現状中身確認の為に置いている）
	//m_copypos = (ParticleBase*)MappedResource.pData;


	////// データの入れ込みと初期化
	//for (int i = 0; i < NUM_ELEMENTS; ++i)
	//{
	//	g_pos[i] = p[i];	
	////	p[i].depth= RandomClass::GetRandom(0.0f, 100.0f);
	//	//g_pos[i].depth=1;
	//	//break;
	//}
	//// debugBufの書き換え
	//memcpy_s(MappedResource.pData, MappedResource.RowPitch, g_pos, sizeof(g_pos) * NUM_ELEMENTS);


	// バッファのデータの書き換え（バッファ←計算結果バッファ）
	m_resultTestBuffer->Copy(m_resultTestBuffer->GetBuffer(), debugbuf);

	// 固定解除
	m_context->Unmap(debugbuf, 0);


	SAFE_RELEASE(debugbuf);
}


double logbase(double a, double base)
{
	return log(a) / log(base);
}

// 並び替えのソートと実行
void ComputeManager::BitonicSort(int gpu_data)
{
	ID3D11DeviceContext* m_context = CDirectXGraphics::GetInstance()->GetImmediateContext();
	int nlog = (int)(logbase(NUM_ELEMENTS, 2));		// ループ実行数
	const int THREADNUM_X = 256;					// CSのスレッド数

	// ソート用の変数に値入れる
	sortStruct->inc = 0;
	sortStruct->dir = 0;
	if (!*m_stopFlg) {
		sortStruct->time = Time::GetInstance()->GetDeltaTimeMilli() / 1000.0f* m_timeSpeed;
	}
	else {
		sortStruct->time = 0;
	}
	sortStruct->seedkey = RandomClass::GetRandom(-1, 1);


	// 変数をバッファに入れセットする
	m_SortBuffer->Update(sortStruct);
	m_SortBuffer->SetBuffer(5);

	for (int i = 0; i < nlog; i++)
	{
		sortStruct->inc = 1 << i;
		for (int j = 0; j < i + 1; j++)
		{
			// CS実行
			m_context->Dispatch(NUM_ELEMENTS / 2 / THREADNUM_X, 1, 1);

			// 値の更新
			sortStruct->dir = 2 << i;

			// SRVの更新			
			m_SortBuffer->Update(sortStruct);

			// 値の更新
			sortStruct->inc /= 2;
		}
	}
}

void ComputeManager::FirstCS()
{
	ID3D11DeviceContext* m_context = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// シェーダーや定数バッファ設定// 計算の値をセット
	ID3D11UnorderedAccessView* uav[3] = { m_particleBuffer->GetUAV(),m_particleNumBuffer->GetUAV(),m_resultBuffer->GetUAV() };
	m_context->CSSetShader(GetCSInitShader(), NULL, 0);
	m_context->CSSetUnorderedAccessViews(0, 3, uav, NULL);
	m_context->Dispatch(1, 1, 1);
}


void ComputeManager::CopyParticleNumBuffer()
{
	ID3D11Device* m_device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* m_context = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// コピー用のバッファ作成とコピー
	ID3D11Buffer* debugbuf = CreateAndCopyToDebugBuf(m_device, m_context, m_particleNumBuffer->GetBuffer());
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	// パーティクルの数の取得有無
#if defined( TOOLMODE)	
	if (!*particleNumDrawFlg) {
		// 取得しない
		m_context->Map(debugbuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	}
	else {
		// 取得する
		m_context->Map(debugbuf, 0, D3D11_MAP_READ, 0, &MappedResource);
		m_particleNum = (ParticleNum*)MappedResource.pData;
	}
#else
	m_context->Map(debugbuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
#endif
	m_context->Unmap(debugbuf, 0);
}

// コンピュートのオブジェクトのセット
void ComputeManager::SetComputeList(Component::ComputeComponent* _com)
{	
	_com->SetEmitterName(ListNameCheck(_com->GetEmitterName()));
	computeList.emplace_back(_com);
}

// 生成数0のオブジェクトの死亡
void ComputeManager::RemoveComputeList()
{
}

// プラスのデータをバッファに入れる
void  ComputeManager::SetBufferPlusDate()
{
	// 仮サイズ
	auto comListSize = computeList.size();
	
	// 親の番号がずれないようにしないといけない
	for (int i = 0; i < PlusBufNum; i++)
	{
		// 範囲内の値設定
		if (comListSize > i)
		{
			Plus[i] = computeList[i]->GetPlusData();
			Plus[i].parentId=i;
			/*if (computeList[i]->GetSatrtEndFlg())
			{
				Plus[i].nowSpawnMaxNum = (float)0;
			}*/
			
		}// 範囲外の設定
		else {
			Plus[i].rand.x = 1;
			Plus[i].rand.y = 0;
			Plus[i].rand.z = 0;
			Plus[i].vel.x = (float)0;
			Plus[i].vel.y = (float)- 1;
			Plus[i].vel.z = (float)0;
			Plus[i].life = (float)1;
			Plus[i].nowSpawnMaxNum = (float)0;
			Plus[i].spawnPos.x = (float)20*i;
			Plus[i].spawnPos.y = (float)10;
			Plus[i].spawnPos.z = (float)20;
			Plus[i].spawnPosRange.x = (float)1;
			Plus[i].spawnPosRange.y = (float)1;
			Plus[i].spawnPosRange.z = (float)1;

			// サイズの初期化
			Plus[i].scaleAnim.initScale = XMFLOAT3(1, 1, 1);
			Plus[i].scaleAnim.finishScale = XMFLOAT3(1, 1, 1);
			Plus[i].parentId = i;
		}
	}
	// バッファの更新
	m_plusBuffer->Update(Plus);
	m_plusBuffer->SetBuffer(6);
}

void  ComputeManager::ImGui()
{
#if defined( TOOLMODE)	

	ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0 / 2, 0), ImGuiCond_Once);   //画面位置を外部から取得できるようにする

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_MenuBar;

	// 開始
	ImGui::Begin(u8"デバッグ中", &m_debug, window_flags);

	// メニューバー
	MenuBar();
	DrawMenuBar();

	// フレームレートを表示
	ImGui::Text(u8"現在のFPS : %.1f FPS", ImGui::GetIO().Framerate);

	// 全体の生成数
	if (*particleNumDrawFlg) {
		ImGui::Text(u8"全体の生成数 : %d", (int)m_particleNum->spawnNum);
	}
	else {
		ImGui::Text(u8"全体の生成数 : 取得していません");
	}

	// エミッターのアイコン触ったか？
	for (auto list : computeList)
	{
		if (list->DebugImGuiOpen())
		{
			stackCompute = list;
		}
		list->ResetOpenFlg();
	}

	// エミッター操作ウィンドウ表示
	if (stackCompute != nullptr)
	stackCompute->DebugImGui();


	ImGui::End();

	// 2つ目
	ImGui::SetNextWindowSize(ImVec2(500, 60), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(Application::CLIENT_WIDTH/2- 500/2, 0), ImGuiCond_Once);   //画面位置を外部から取得できるようにする

	// ウィンドウの設定
	window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoTitleBar;

	// 開始
	ImGui::Begin(u8"各種設定", &m_debug, window_flags);
	DrawParticleNumImGui(); ImGui::SameLine();
	DrawArrowImGui(); ImGui::SameLine();
	
	// 升目
	if (ImGui::SmallButton(u8"マス目の有無"))
	{
		if (!*tileDrawFlg) { tile->SetExistState(true); *tileDrawFlg = true; }
		else { tile->SetExistState(false); *tileDrawFlg = false; }
	}
	ImGui::End();

#endif
}

// セーブ
void  ComputeManager::SaveImGui()
{
	if(stackCompute!=nullptr)
	stackCompute->Save();

}

// ロード
void  ComputeManager::LoadImGui()
{
	stackCompute->WindowsLoad();
}

// パーティクルの追加
void  ComputeManager::AddParticleImGui()
{
	if (PlusBufNum > computeList.size()) {
		if (ImGui::MenuItem("New"))
		{
			// 新規作成とリストに追加
			GameObject *obj = new GameObject();
			Component::ComputeComponent* com = obj->AddComponent<Component::ComputeComponent>("assets/Image/Particle/ParticleCircle.png", "");
			SceneManager::Instance()->AddObject(obj, false);
		}

		if (ImGui::MenuItem("New Load"))
		{
			// 新規作成とリストに追加
			GameObject *obj = new GameObject();
			Component::ComputeComponent* com = obj->AddComponent<Component::ComputeComponent>("assets/Image/Particle/ParticleCircle.png", "");
			SceneManager::Instance()->AddObject(obj, false);
			com->WindowsLoad();
		}
		ImGui::SameLine(); Helper::HelpMarker(
			u8"新規作成とファイルのロード。\n"
			"efeファイルがロードされなかった場合、新規データ作成が実行されます。");
	}
	else {
		// サイズ超えるので作れなかったのテロップ
	}
}

// 名前部分
void ComputeManager::MenuBar()
{	
	// Menu Bar
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::BeginMenu("Save"))
			{
				SaveImGui();
				ImGui::EndMenu();
			}
			ImGui::SameLine(); Helper::HelpMarker(
				u8"ファイルのセーブ。\n"
				"セーブするエミッターと保存場所を選択してください。");

			if (ImGui::MenuItem("Load?"))
			{
				LoadImGui();
			}
			ImGui::SameLine(); Helper::HelpMarker(
				u8"ファイルのロード。\n"
				"efeファイルが選択されていない場合は、ロードできません。");
			
			if (ImGui::BeginMenu("AddParticle"))
			{
				AddParticleImGui();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("ReSet"))
			{
				ResetImGui();
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}	
		ImGui::EndMenuBar();
	}

	ImGui::Spacing();
}

// 名前部分
void ComputeManager::DrawMenuBar()
{
	// Menu Bar
	if (ImGui::BeginMenuBar())
	{

		if (ImGui::BeginMenu("DrawSetting"))
		{
			const float width = ImGui::GetWindowWidth();
			const float combo_width = width * 0.5f;
			ImGui::SetNextItemWidth(combo_width);

			const char* items[] = { "2D", "3D" };
			static int _itemDrawSetting = 0;
			ImGui::Combo("Draw Setting", &_itemDrawSetting, items, IM_ARRAYSIZE(items));
			if (_itemDrawSetting==0)
			{
				m_drawStatus = DrawStatus::Second;
			}
			else {
				m_drawStatus = DrawStatus::Third;
			}

			ImGui::SameLine(); Helper::HelpMarker(
				u8"描画を2Dビルボードにします（軽量）か3Dに切り替えられます");
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Spacing();

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("PostEffect"))
		{
			if (ImGui::MenuItem("Bloom", NULL, m_bloomFlg))  m_bloomFlg = !m_bloomFlg;
			//ImGui::SameLine(); Helper::HelpMarker(u8"ブルームのオンオフです。");

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}


// リスト内で同じ名前があるかの確認
std::string ComputeManager::ListNameCheck(std::string _name)
{
	int _nameNumber = 0;
	bool _check = false;
	std::string _stackName;
	// 同じ名前をリストにまとめる
	// 番号を照準にして空いている数字を算出
	while (!_check)
	{
		if (_nameNumber == 0)
		{
			_stackName = _name;
		}
		else {
			std::string num_str1(std::to_string(_nameNumber));
			_stackName = _name + "_" + num_str1;
		}

		if (SameNameCheck(_stackName))
		{
			_nameNumber++;
		}
		else {
			_check = true;
		}
	}
	return _stackName;
}

// 同じ名前の確認
bool ComputeManager::SameNameCheck(std::string _name)
{
	for (auto list : computeList)
	{
		if (_name == list->GetEmitterName())
		{
			
			return true;
		}
	}
	return false;
}

// リセット
void ComputeManager::ResetImGui()
{
	if (ImGui::MenuItem("OK?"))
	{
		computeList.clear();
	}
}

// スラープにする？
void ComputeManager::SetLookEmitter(Vector3 _vec)
{
	Component::Camera::GetMainCamera()->GetOwner()->transform->LookAt(_vec,Vector3(0,1,0));
}

// パーティクル数を描画するか
void ComputeManager::DrawParticleNumImGui()
{
#ifdef TOOLMODE


	if (ImGui::SmallButton(u8"パーティクスの総数の描画"))
	{
		*particleNumDrawFlg = !*particleNumDrawFlg;
	}
	
	ImGui::SameLine(); Helper::HelpMarker(
		u8"パーティクル総数描画の切換えです。\n"
		"描画選択時はFPSが低下します。");

#endif
}

// 矢印を描画するか
void ComputeManager::DrawArrowImGui()
{
#ifdef TOOLMODE
	if (ImGui::SmallButton(u8"エミッターの矢印描画"))
	{
		*particleArrowDrawFlg = !*particleArrowDrawFlg;
	}

	ImGui::SameLine(); Helper::HelpMarker(
		u8"エミッター選択時に位置と方向を示すものを描画するか？。");
#endif
}

// ブレンドステート
void ComputeManager::CreateBlendState()
{
	// 加算のブレンド
	HRESULT	hr = S_OK;
	ID3D11Device* m_device = CDirectXGraphics::GetInstance()->GetDXDevice();

	D3D11_BLEND_DESC desc = {};

	//ブレンドステート設定（アルファブレンド可）
	desc.RenderTarget[0].BlendEnable = TRUE;
	//blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	//desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	//ブレンドステート作成
	hr = m_device->CreateBlendState(&desc, &m_blendState);
	if (FAILED(hr))
	{
		return ;
	}
}

// サンプラーの設定
void ComputeManager::CreateSampleState()
{
	ID3D11Device* m_device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// サンプラの設定
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.0f;		// ホワイト
	samplerDesc.BorderColor[1] = 1.0f;		// ..
	samplerDesc.BorderColor[2] = 1.0f;		// ..
	samplerDesc.BorderColor[3] = 1.0f;		// ..
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// サンプラステートを作成
	if (FAILED(m_device->CreateSamplerState(&samplerDesc, &m_sampler)))
	{
		throw std::runtime_error("さんぷるステートの作成に失敗");
	}
}


//-----------------------------
// create by Suehara
//-----------------------------