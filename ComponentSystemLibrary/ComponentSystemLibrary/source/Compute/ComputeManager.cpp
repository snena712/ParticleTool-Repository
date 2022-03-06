#include "ComputeManager.h"

ID3D11Buffer* const                 g_pNullBuffer = NULL;    // Helper to Clear Buffers
UINT                                g_iNullUINT = 0;         // Helper to Clear Buffers

void ComputeManager::Init()
{
	// �摜�����ꍞ��
	m_filename = "assets/Image/Particle/ParticleCircle.png";
	ImageResourceManager::GetInstance()->Load(m_filename);

	m_noiseTex = "assets/Image/Noise/noise.jpg";
	// �e�N�X�`�����Z�b�g(�����p)
	ImageResourceManager::GetInstance()->Load(m_noiseTex);

	// ���[���h�ϊ��s��Z�b�g
	DX11MtxIdentity(worldmtx);	//���[���h�ϊ��s���P�ʍs��ɂ���

	// �f�[�^�̏�����
	InitDate();

	// Buffer
	InitBuffer();

	// �V�F�_�[�̏�����
	InitShader();

	// �u�����h
	CreateBlendState();
	CreateSampleState();

	// 3d�p�̃��f��
	GameObject*sphere = new GameObject;
	sphere->AddComponent<Component::SphereModel>();
	model = sphere->GetComponent<Component::ModelRenderInstance>();

	initEnd = true;
}

void ComputeManager::InitDate()
{
	// ���̏�����
	m_particleNum = new ParticleNum;
	m_particleNum->nowSpawnNum = 0;
	m_particleNum->spawnNum = 0;
	m_particleNum->maxParticleNum = NUM_ELEMENTS;
	m_particleNum->seed = 1;

	// �\�[�g�p�ϐ�
	sortStruct = new SortStruct;

	// �e�p�[�e�B�N���̏�����
	m_emitter = new ParticleBase[NUM_ELEMENTS];

	m_particleDraw = new ParticleDraw[NUM_ELEMENTS];

	// �J�n����
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

		// ���S
		m_emitter[i].Live = 0;

		// �T�C�Y
		m_emitter[i].scale = Vector3(0, 0, 0);

		// �e�ݒ�
		m_emitter[i].parentId = 1000;
		m_emitter[i].dummy.x = -1000;
		m_emitter[i].dummy.y = -1000;
		m_emitter[i].dummy.z = -1000;
	}

#if defined( TOOLMODE)
	//����
	tile = new GameObject();
	tile->AddComponent<Component::TileLine>();
	SceneManager::Instance()->AddObject(tile, true);
#endif
}

void ComputeManager::InitBuffer()
{
	// �o�[�e�B�N���̊�b
	m_particleBuffer = new StructuredBuffer();
	m_particleBuffer->CreateStructuredBuffer(NUM_ELEMENTS, m_emitter);

	// �o�[�e�B�N���̊�b�̌v�Z���ʎ擾
	m_resultBuffer = new StructuredBuffer();
	m_resultBuffer->CreateStructuredBuffer(NUM_ELEMENTS, m_particleDraw);

	m_resultTestBuffer = new StructuredBuffer();
	m_resultTestBuffer->CreateStructuredBuffer(NUM_ELEMENTS, m_particleDraw);

	// �\�[�g�Ǘ�
	m_SortBuffer = new ConstBuffer();
	m_SortBuffer->CreateConStructuredBuffer<SortStruct>(ConstBuffer::Type::Multi,1);

	// ���Ǘ��o�b�t�@
	m_particleNumBuffer = new StructuredBuffer();
	m_particleNumBuffer->CreateStructuredBuffer(1, m_particleNum);

	// �萔�o�b�t�@�̍X�V
	// �v�Z�p�̃o�b�t�@�̏������i�z��j
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

	// �\�[�g
	Helper::InitComputeShader(_device,
		"shader/ComputeShader_v2.hlsl",
		"BitonicSortLive",
		"cs_5_0",
		&m_pSortCS);

	// ������
	Helper::InitComputeShader(_device,
		"shader/ComputeShader_v2.hlsl",
		"InitCS",
		"cs_5_0",
		&m_pInitCS);

	// �R���s���[�g�V�F�[�_�[�̐����@�i�x�[�X�̂��́j
	Helper::InitComputeShader(_device,
		"shader/ComputeShader_v2.hlsl",
		"CS",
		"cs_5_0",
		&m_pParticleCS);

	// �R���s���[�g�V�F�[�_�[�̐����@�i����������́j
	Helper::InitComputeShader(_device,
		"shader/ComputeShader_v2.hlsl",
		"SpawnCS",
		"cs_5_0",
		&m_pSpawnCS);

	// ���S
	Helper::InitComputeShader(_device,
		"shader/ComputeShader_v2.hlsl",
		"DeadCheck",
		"cs_5_0",
		&m_pDeadCS);
}

void ComputeManager::Update()
{
	// CS�̍X�V
	UpdateCS();

	// �s�v�Ȑݒ�̏�����
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

	// SRV��UAV�̐ݒ�
	m_noiseTex = "assets/Image/Noise/noise.jpg";
	// �e�N�X�`�����Z�b�g(�摜�A�����A�F)
	ID3D11ShaderResourceView* sRViews[4] = { GetStructuredBuffer()->GetSRV(), 
		ImageResourceManager::GetInstance()->GetResource(m_noiseTex)->GetSRV(),
		nullptr,
		colorPictureSRV.Get() };

	ComputeManager::Instance()->FirstCS();

	// �T���v���[�̃Z�b�g
	m_context->CSSetSamplers(0, 1, &m_sampler);

	// SRV�̎��s
	m_context->CSSetShaderResources(0, 4, sRViews);

#if defined( TOOLMODE)&&defined(DEBUG)
	// Imgui�̕\��
	ImGui();
#endif

	// �o�b�t�@�̍X�V
	SetBufferPlusDate();

	// ��������
	m_context->CSSetShader(GetCSSpawnShader(), NULL, 0);
	m_context->Dispatch(1, 1, 1);

	// ���S����
	m_context->CSSetShader(GetCSDeadShader(), NULL, 0);
	m_context->Dispatch(1, 1, 1);
	
	// ��{�X�V�̃V�F�[�_�[�Z�b�g
	m_context->CSSetShader(GetCSShader(), NULL, 0);

	// �萔�o�b�t�@�̍X�V
	m_context->Dispatch(NUM_ELEMENTS/Thread, 1, 1);
}

void ComputeManager::Draw()
{	
	if (!initEnd)return;
	DirectX::XMFLOAT4X4 mtx;

	// �eCS�̏���
	BeforeDrawCS();

	// �o�b�t�@��SRV�ɃZ�b�g
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	ID3D11ShaderResourceView* aRViews[2] = { m_resultTestBuffer->GetSRV(), nullptr };

	// �o�b�t�@������
	ID3D11Buffer* ppCBNULL[2] = { NULL,NULL };
	devcontext->VSSetConstantBuffers(0, 2, ppCBNULL);

	// VS��SRV�Z�b�g
	devcontext->VSSetShaderResources(0, 2, aRViews);

	auto _setTrans = DX11SetTransform::GetInstance();
	auto _camera = Component::Camera::GetMainCamera();
	_setTrans->SetTransform(DX11SetTransform::TYPE::WORLD, worldmtx);

	// �r���[�ϊ��s��Z�b�g
	mtx = _camera->GetCameraMatrix();
	_setTrans->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	// �v���W�F�N�V�����ϊ��s��Z�b�g
	mtx = _camera->GetProjectionMatrix();
	_setTrans->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	// ���_�̈ʒu�ݒ�
	DX11LightInit(Component::Camera::GetMainCamera()->GetOwner()->transform->position);

	// �摜��SRV�ݒ�
	ID3D11ShaderResourceView* srv = ImageResourceManager::GetInstance()->GetResource(m_filename)->GetSRV();

	// PS��SRV�̐ݒ�
	devcontext->PSSetShaderResources(2, 1, &srv);

	// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�A���_���C�A�E�g�擾		
	devcontext->VSSetShader(m_pParticleVS.Get(), nullptr, 0);
	devcontext->GSSetShader(m_pParticleGS.Get(), nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(m_pParticlePS.Get(), nullptr, 0);

	devcontext->IASetVertexBuffers(0, 1, &g_pNullBuffer, &g_iNullUINT, &g_iNullUINT);   //NULL
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);             // ���X�g

	 // �u�����h�Z�b�g
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	//�A���t�@�u�����h��ON�ɂ���
	//devcontext->OMSetBlendState(m_blendState, blendFactor, 0xffffffff);

	// �T���v���[�̃Z�b�g
	//devcontext->PSSetSamplers(0, 1, &m_sampler);

	// VS�ɒ萔�̃Z�b�g
	m_plusBuffer->SetVSBuffer(6);




	// �����_�[�^�[�Q�b�g�̐ݒ�
	if (m_bloomFlg) {
		auto _rtv =PostEffect::BloomPostEffect::Instance()->GetSRVDefault().GetRTV();
		devcontext->OMSetRenderTargets(1, &_rtv, nullptr);
	}
	else {
		auto _rtv = DirectGraphicsTexture::GetInstance()->GetRTV();
		devcontext->OMSetRenderTargets(1, &_rtv, nullptr);
	}
	//// �����_�[�^�[�Q�b�g�̐F������
	//m_rt2d->ClearColorRenderTarget();


	// �h���[�R�[�����s�i�`�敨�̎�ށj
	switch (m_drawStatus)
	{
	case DrawStatus::Second:
		devcontext->Draw(
			NUM_ELEMENTS,// �C���f�b�N�X��
			0);				// ����_�C���f�b�N�X
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

	// �����_�[�^�[�Q�b�g�̐ݒ�
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

	// �G�~�b�^�[�̖��`��
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
	// �\�[�g����
	//========================
	ID3D11DeviceContext* m_context = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �֐�
	// �V�F�[�_�[��萔�o�b�t�@�ݒ�
	m_context->CSSetShader(GetCSSortShader(), NULL, 0);
	// �萔�o�b�t�@�̍X�V

	BitonicSort(0);

#ifdef TOOLMODE
	// �o�b�t�@�̃R�s�[
	CopyParticleNumBuffer();
#endif

	// �f�[�^�̃R�s�[
	Copy();
}

void ComputeManager::Reset()
{
	// �X�V�폜
	ID3D11DeviceContext* m_context = CDirectXGraphics::GetInstance()->GetImmediateContext();
	m_context->CSSetShader(NULL, NULL, 0);
	ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
	m_context->CSSetUnorderedAccessViews(0, 1, ppUAViewNULL, NULL);
	ID3D11ShaderResourceView* ppSRVNULL[2] = { NULL, NULL };
	m_context->CSSetShaderResources(0, 2, ppSRVNULL);
	ID3D11Buffer* ppCBNULL[2] = { NULL,NULL };
	m_context->CSSetConstantBuffers(0, 2, ppCBNULL);
}

// �f�[�^�̃R�s�[
void ComputeManager::Copy()
{
	// -------------------------------------------------------------------------------------------------
	ID3D11Device* m_device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* m_context = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �R�s�[�p�̃o�b�t�@�쐬�ƃR�s�[
	ID3D11Buffer* debugbuf = ComputeManager::Instance()->CreateAndCopyToDebugBuf(m_device, m_context, m_resultBuffer->GetBuffer());
	//ID3D11Buffer* debugbuf = CreateAndCopyToDebugBuf(m_device, m_context, m_resultBuffer->GetBuffer());
	D3D11_MAPPED_SUBRESOURCE MappedResource;

	m_context->Map(debugbuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);

	//�o�b�t�@�̒��g���R�s�[�i���󒆐g�m�F�ׂ̈ɒu���Ă���j
	//m_copypos = (ParticleBase*)MappedResource.pData;


	////// �f�[�^�̓��ꍞ�݂Ə�����
	//for (int i = 0; i < NUM_ELEMENTS; ++i)
	//{
	//	g_pos[i] = p[i];	
	////	p[i].depth= RandomClass::GetRandom(0.0f, 100.0f);
	//	//g_pos[i].depth=1;
	//	//break;
	//}
	//// debugBuf�̏�������
	//memcpy_s(MappedResource.pData, MappedResource.RowPitch, g_pos, sizeof(g_pos) * NUM_ELEMENTS);


	// �o�b�t�@�̃f�[�^�̏��������i�o�b�t�@���v�Z���ʃo�b�t�@�j
	m_resultTestBuffer->Copy(m_resultTestBuffer->GetBuffer(), debugbuf);

	// �Œ����
	m_context->Unmap(debugbuf, 0);


	SAFE_RELEASE(debugbuf);
}


double logbase(double a, double base)
{
	return log(a) / log(base);
}

// ���ёւ��̃\�[�g�Ǝ��s
void ComputeManager::BitonicSort(int gpu_data)
{
	ID3D11DeviceContext* m_context = CDirectXGraphics::GetInstance()->GetImmediateContext();
	int nlog = (int)(logbase(NUM_ELEMENTS, 2));		// ���[�v���s��
	const int THREADNUM_X = 256;					// CS�̃X���b�h��

	// �\�[�g�p�̕ϐ��ɒl�����
	sortStruct->inc = 0;
	sortStruct->dir = 0;
	if (!*m_stopFlg) {
		sortStruct->time = Time::GetInstance()->GetDeltaTimeMilli() / 1000.0f* m_timeSpeed;
	}
	else {
		sortStruct->time = 0;
	}
	sortStruct->seedkey = RandomClass::GetRandom(-1, 1);


	// �ϐ����o�b�t�@�ɓ���Z�b�g����
	m_SortBuffer->Update(sortStruct);
	m_SortBuffer->SetBuffer(5);

	for (int i = 0; i < nlog; i++)
	{
		sortStruct->inc = 1 << i;
		for (int j = 0; j < i + 1; j++)
		{
			// CS���s
			m_context->Dispatch(NUM_ELEMENTS / 2 / THREADNUM_X, 1, 1);

			// �l�̍X�V
			sortStruct->dir = 2 << i;

			// SRV�̍X�V			
			m_SortBuffer->Update(sortStruct);

			// �l�̍X�V
			sortStruct->inc /= 2;
		}
	}
}

void ComputeManager::FirstCS()
{
	ID3D11DeviceContext* m_context = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �V�F�[�_�[��萔�o�b�t�@�ݒ�// �v�Z�̒l���Z�b�g
	ID3D11UnorderedAccessView* uav[3] = { m_particleBuffer->GetUAV(),m_particleNumBuffer->GetUAV(),m_resultBuffer->GetUAV() };
	m_context->CSSetShader(GetCSInitShader(), NULL, 0);
	m_context->CSSetUnorderedAccessViews(0, 3, uav, NULL);
	m_context->Dispatch(1, 1, 1);
}


void ComputeManager::CopyParticleNumBuffer()
{
	ID3D11Device* m_device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* m_context = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �R�s�[�p�̃o�b�t�@�쐬�ƃR�s�[
	ID3D11Buffer* debugbuf = CreateAndCopyToDebugBuf(m_device, m_context, m_particleNumBuffer->GetBuffer());
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	// �p�[�e�B�N���̐��̎擾�L��
#if defined( TOOLMODE)	
	if (!*particleNumDrawFlg) {
		// �擾���Ȃ�
		m_context->Map(debugbuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	}
	else {
		// �擾����
		m_context->Map(debugbuf, 0, D3D11_MAP_READ, 0, &MappedResource);
		m_particleNum = (ParticleNum*)MappedResource.pData;
	}
#else
	m_context->Map(debugbuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
#endif
	m_context->Unmap(debugbuf, 0);
}

// �R���s���[�g�̃I�u�W�F�N�g�̃Z�b�g
void ComputeManager::SetComputeList(Component::ComputeComponent* _com)
{	
	_com->SetEmitterName(ListNameCheck(_com->GetEmitterName()));
	computeList.emplace_back(_com);
}

// ������0�̃I�u�W�F�N�g�̎��S
void ComputeManager::RemoveComputeList()
{
}

// �v���X�̃f�[�^���o�b�t�@�ɓ����
void  ComputeManager::SetBufferPlusDate()
{
	// ���T�C�Y
	auto comListSize = computeList.size();
	
	// �e�̔ԍ�������Ȃ��悤�ɂ��Ȃ��Ƃ����Ȃ�
	for (int i = 0; i < PlusBufNum; i++)
	{
		// �͈͓��̒l�ݒ�
		if (comListSize > i)
		{
			Plus[i] = computeList[i]->GetPlusData();
			Plus[i].parentId=i;
			/*if (computeList[i]->GetSatrtEndFlg())
			{
				Plus[i].nowSpawnMaxNum = (float)0;
			}*/
			
		}// �͈͊O�̐ݒ�
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

			// �T�C�Y�̏�����
			Plus[i].scaleAnim.initScale = XMFLOAT3(1, 1, 1);
			Plus[i].scaleAnim.finishScale = XMFLOAT3(1, 1, 1);
			Plus[i].parentId = i;
		}
	}
	// �o�b�t�@�̍X�V
	m_plusBuffer->Update(Plus);
	m_plusBuffer->SetBuffer(6);
}

void  ComputeManager::ImGui()
{
#if defined( TOOLMODE)	

	ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0 / 2, 0), ImGuiCond_Once);   //��ʈʒu���O������擾�ł���悤�ɂ���

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_MenuBar;

	// �J�n
	ImGui::Begin(u8"�f�o�b�O��", &m_debug, window_flags);

	// ���j���[�o�[
	MenuBar();
	DrawMenuBar();

	// �t���[�����[�g��\��
	ImGui::Text(u8"���݂�FPS : %.1f FPS", ImGui::GetIO().Framerate);

	// �S�̂̐�����
	if (*particleNumDrawFlg) {
		ImGui::Text(u8"�S�̂̐����� : %d", (int)m_particleNum->spawnNum);
	}
	else {
		ImGui::Text(u8"�S�̂̐����� : �擾���Ă��܂���");
	}

	// �G�~�b�^�[�̃A�C�R���G�������H
	for (auto list : computeList)
	{
		if (list->DebugImGuiOpen())
		{
			stackCompute = list;
		}
		list->ResetOpenFlg();
	}

	// �G�~�b�^�[����E�B���h�E�\��
	if (stackCompute != nullptr)
	stackCompute->DebugImGui();


	ImGui::End();

	// 2��
	ImGui::SetNextWindowSize(ImVec2(500, 60), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(Application::CLIENT_WIDTH/2- 500/2, 0), ImGuiCond_Once);   //��ʈʒu���O������擾�ł���悤�ɂ���

	// �E�B���h�E�̐ݒ�
	window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoTitleBar;

	// �J�n
	ImGui::Begin(u8"�e��ݒ�", &m_debug, window_flags);
	DrawParticleNumImGui(); ImGui::SameLine();
	DrawArrowImGui(); ImGui::SameLine();
	
	// ����
	if (ImGui::SmallButton(u8"�}�X�ڂ̗L��"))
	{
		if (!*tileDrawFlg) { tile->SetExistState(true); *tileDrawFlg = true; }
		else { tile->SetExistState(false); *tileDrawFlg = false; }
	}
	ImGui::End();

#endif
}

// �Z�[�u
void  ComputeManager::SaveImGui()
{
	if(stackCompute!=nullptr)
	stackCompute->Save();

}

// ���[�h
void  ComputeManager::LoadImGui()
{
	stackCompute->WindowsLoad();
}

// �p�[�e�B�N���̒ǉ�
void  ComputeManager::AddParticleImGui()
{
	if (PlusBufNum > computeList.size()) {
		if (ImGui::MenuItem("New"))
		{
			// �V�K�쐬�ƃ��X�g�ɒǉ�
			GameObject *obj = new GameObject();
			Component::ComputeComponent* com = obj->AddComponent<Component::ComputeComponent>("assets/Image/Particle/ParticleCircle.png", "");
			SceneManager::Instance()->AddObject(obj, false);
		}

		if (ImGui::MenuItem("New Load"))
		{
			// �V�K�쐬�ƃ��X�g�ɒǉ�
			GameObject *obj = new GameObject();
			Component::ComputeComponent* com = obj->AddComponent<Component::ComputeComponent>("assets/Image/Particle/ParticleCircle.png", "");
			SceneManager::Instance()->AddObject(obj, false);
			com->WindowsLoad();
		}
		ImGui::SameLine(); Helper::HelpMarker(
			u8"�V�K�쐬�ƃt�@�C���̃��[�h�B\n"
			"efe�t�@�C�������[�h����Ȃ������ꍇ�A�V�K�f�[�^�쐬�����s����܂��B");
	}
	else {
		// �T�C�Y������̂ō��Ȃ������̃e���b�v
	}
}

// ���O����
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
				u8"�t�@�C���̃Z�[�u�B\n"
				"�Z�[�u����G�~�b�^�[�ƕۑ��ꏊ��I�����Ă��������B");

			if (ImGui::MenuItem("Load?"))
			{
				LoadImGui();
			}
			ImGui::SameLine(); Helper::HelpMarker(
				u8"�t�@�C���̃��[�h�B\n"
				"efe�t�@�C�����I������Ă��Ȃ��ꍇ�́A���[�h�ł��܂���B");
			
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

// ���O����
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
				u8"�`���2D�r���{�[�h�ɂ��܂��i�y�ʁj��3D�ɐ؂�ւ����܂�");
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
			//ImGui::SameLine(); Helper::HelpMarker(u8"�u���[���̃I���I�t�ł��B");

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}


// ���X�g���œ������O�����邩�̊m�F
std::string ComputeManager::ListNameCheck(std::string _name)
{
	int _nameNumber = 0;
	bool _check = false;
	std::string _stackName;
	// �������O�����X�g�ɂ܂Ƃ߂�
	// �ԍ����Ə��ɂ��ċ󂢂Ă��鐔�����Z�o
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

// �������O�̊m�F
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

// ���Z�b�g
void ComputeManager::ResetImGui()
{
	if (ImGui::MenuItem("OK?"))
	{
		computeList.clear();
	}
}

// �X���[�v�ɂ���H
void ComputeManager::SetLookEmitter(Vector3 _vec)
{
	Component::Camera::GetMainCamera()->GetOwner()->transform->LookAt(_vec,Vector3(0,1,0));
}

// �p�[�e�B�N������`�悷�邩
void ComputeManager::DrawParticleNumImGui()
{
#ifdef TOOLMODE


	if (ImGui::SmallButton(u8"�p�[�e�B�N�X�̑����̕`��"))
	{
		*particleNumDrawFlg = !*particleNumDrawFlg;
	}
	
	ImGui::SameLine(); Helper::HelpMarker(
		u8"�p�[�e�B�N�������`��̐؊����ł��B\n"
		"�`��I������FPS���ቺ���܂��B");

#endif
}

// ����`�悷�邩
void ComputeManager::DrawArrowImGui()
{
#ifdef TOOLMODE
	if (ImGui::SmallButton(u8"�G�~�b�^�[�̖��`��"))
	{
		*particleArrowDrawFlg = !*particleArrowDrawFlg;
	}

	ImGui::SameLine(); Helper::HelpMarker(
		u8"�G�~�b�^�[�I�����Ɉʒu�ƕ������������̂�`�悷�邩�H�B");
#endif
}

// �u�����h�X�e�[�g
void ComputeManager::CreateBlendState()
{
	// ���Z�̃u�����h
	HRESULT	hr = S_OK;
	ID3D11Device* m_device = CDirectXGraphics::GetInstance()->GetDXDevice();

	D3D11_BLEND_DESC desc = {};

	//�u�����h�X�e�[�g�ݒ�i�A���t�@�u�����h�j
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

	//�u�����h�X�e�[�g�쐬
	hr = m_device->CreateBlendState(&desc, &m_blendState);
	if (FAILED(hr))
	{
		return ;
	}
}

// �T���v���[�̐ݒ�
void ComputeManager::CreateSampleState()
{
	ID3D11Device* m_device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// �T���v���̐ݒ�
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.0f;		// �z���C�g
	samplerDesc.BorderColor[1] = 1.0f;		// ..
	samplerDesc.BorderColor[2] = 1.0f;		// ..
	samplerDesc.BorderColor[3] = 1.0f;		// ..
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// �T���v���X�e�[�g���쐬
	if (FAILED(m_device->CreateSamplerState(&samplerDesc, &m_sampler)))
	{
		throw std::runtime_error("����Ղ�X�e�[�g�̍쐬�Ɏ��s");
	}
}


//-----------------------------
// create by Suehara
//-----------------------------