//// �萔�o�b�t�@
//cbuffer CB : register(b0)
//{
//    float3 Position;
//};

//// ���̓o�b�t�@(�\�����o�b�t�@�B�ǂݍ��ݐ�p)
//StructuredBuffer<float3> Input : register(t0);
//// �o�̓o�b�t�@(�\�����o�b�t�@�B�ǂݏ����\)
//RWStructuredBuffer<float3> Result : register(u0);

// ���L�������@�@�@/*�@�����Ƀf�[�^��������
//groupshared float shared_data[128];

// �V�F�[�_�֐�
//[numthreads(128, 1, 1)]   // �X���b�h �O���[�v�̃X���b�h��
//void CS(uint3 Gid : SV_GroupID,         // �X���b�h��������̃X���b�h�O���[�v�@�@3�������W
//    uint3 DTid : SV_DispatchThreadID,   // �X���b�h�O���[�v�̃X���b�h            3�������W
//    uint3 GTid : SV_GroupThreadID,      // ���ׂẴX���b�h�̒��ŃX���b�h���ʁ@�@3�������W
//    uint GI : SV_GroupIndex)            // �X���b�h�ԍ�
//{
//     
//    uint hoge = GI * 1;
//    float3 a = Input[GI];
//    Result[Gid.x].x = a.x;
//    Result[Gid.x].x = a.y;
//    Result[Gid.x].x = a.z;
//}

#include	"ComputeShaderHeader.hlsl"



float4 TexColorChanger(float _rate)
{
	float2 _uv = float2(_rate, _rate);

	return g_TexColor.SampleLevel(g_SamplerLinear, _uv.y, .5f, 0);
}

// �F�X�V
void UpdateColor(uint index, uint id)
{
	float _baselife = (float)base[id].life;
	float _buflife = clamp(BufOut[index].life, 0.0f, _baselife);	// �}�C�i�X�ɂȂ�Ȃ��悤�ɂ���
	float _rate = _buflife / _baselife;

	if (base[id].colorFlg&Color_Event_PictureColor) {


		double initLife = 1.0f - (1.0f - _rate);
		double endLife = (1.0f - _rate);
		float4 _col = initLife * base[id].initColor.xyzw + endLife * base[id].endColor.xyzw;

		// �^���I��if��
		BufOut[index].color = lerp(_col, float4(0.0, 0.0, 0.0, 0.0), step(_buflife, 0));
	}
	else {
		float4 _col = TexColorChanger(_rate);
		BufOut[index].color = lerp(_col, float4(0.0, 0.0, 0.0, 0.0), step(_buflife, 0));
	}
}

// �F�ɂ��ړ���
float3 ColorMove(uint index, uint id)
{
	float4 _color = BufOut[index].color;
	return (_color.xyz - 0.5f) * 2 * base[id].speed;
}

// �ړ��X�V
void UpdateVelocity(uint index, uint id)
{
	// �ړ��̌v�Z
	float3 _vel;
	float3 noise = float3(0, 0, 0);

	// �����x�̌v�Z
	_vel.xyz = BufOut[index].vel.xyz + base[id].vel.xyz;

	// �d�͉����x�̌v�Z
	BufOut[index].vel.y -= base[id].gravity * time;

	// �����x�̃v���X
	BufOut[index].vel.xyz += base[id].vel.xyz * time;

	// �g�ړ�
	if (base[id].velocityFlg&Velocity_Event_Sin)
	{
		_vel += curlNoise(BufOut[index].pos) * base[id].speed;
	}

	// �F�ړ�
	if (base[id].velocityFlg&Velocity_Event_ColorMove)
	{
		_vel += ColorMove(index, id);
	}

	// �ړ��ʂ̌v�Z
	if (base[id].velocityFlg&Velocity_Simulation_Space) {
		BufOut[index].pos.xyz = BufOut[index].pos + _vel.xyz * time;
		particleDraw[index].pos = BufOut[index].pos;
	}
	else {
		BufOut[index].pos.xyz += _vel.xyz * time;	
		particleDraw[index].pos = BufOut[index].pos + base[id].spawnPos;
	}

	// �ړ��̏��n
	BufOut[index].vel = BufOut[index].vel;

	// �J��������
	float3 campos = 0;
	campos.x = View._14;
	campos.y = View._24;
	campos.z = View._34;

	// �J��������̋����̊m��
	BufOut[index].depth = Distance(campos, BufOut[index].pos);

}

// �T�C�Y�A�j���[�V����
void ScaleAnimCS(uint index,uint id)
{
	// ���`���
	BufOut[index].scale.xyz = (1.0f - (1.0f - BufOut[index].life / base[id].life)) * base[id].initScale.xyz + (1.0f - BufOut[index].life / base[id].life) * base[id].finishScale.xyz;
}

// Velocity�̏�����
float3 InitVelocityCS(uint index)
{
	// �͈͎擾
	float3 _vel;

	if (base[index].velocityFlg&Velocity_Event_SHAPE_BOX)
	{
		// �l�p
		float3 _volo = GetSpawnVelocityRange(base[index].velRange);

		// �O�����ƈړ���
		_vel.xyz = base[index].forward.xyz * base[index].speed + _volo.xyz;
	}
	else {
		// �~�`
		_vel = GetSpawnVelocityRange(base[index].velRange);
	}
	//�߂�

	return _vel;
}

[numthreads(size_x, size_y, size_z)]
void CS(const uint threadid: SV_DispatchThreadID)
{
	int index = threadid;				// ���s�ԍ�
	int id = BufOut[index].parentId;	// �e�ԍ�
	BufOut[index].parentId = BufOut[index].parentId;

	// �傫��
	ScaleAnimCS(index, id);

	// �F
	UpdateColor(index,id);

	// �ړ�
	UpdateVelocity(index,id);

	particleDraw[index].color = BufOut[index].color;
	particleDraw[index].scale = BufOut[index].scale;
}


// ���������@�����C�ɐ��������@SRV��UAV�Ńf�[�^�̋��L��share�Ńf�[�^����肷��B
[numthreads(1, 1, 1)]
void SpawnCS(const CSInput input, uint index : SV_GroupIndex)
{
	//int index = input.dispatch.x;

	// ���݂̐��̍X�V�i���̌v�Z�j
	//Spawn[0].spawnNum = Spawn[0].spawnNum + nowSpawnMaxNum;

	int basecnt = 0;
	int basespawnnum = 0;

	// ���͍ő吔��
	for (int i = 0; i < Spawn[0].maxParticleNum; i++)
	{
		if (basespawnnum >= base[basecnt].nowSpawnMaxNum)
		{
			// �v���X�̃f�[�^�̏������Ɛi�s
			basecnt++;
			basespawnnum = 0;
		}
			
		if (basespawnnum< base[basecnt].nowSpawnMaxNum)
		{
			if ((int)BufOut[i].Alive == DEAD)
			{
				// ���݂̃t���[���̐������Ɛ����\�ő吔			
				basespawnnum++;
				Spawn[0].nowSpawnNum++;

				BufOut[i].pos = GetSpawnPosRange(basecnt, base[basecnt].spawnPosRange);

				// �����̈ړ���
				BufOut[i].vel = InitVelocityCS(basecnt);
				BufOut[i].life = base[basecnt].life * base[basecnt].Live;
				BufOut[i].Alive = LIVE;

				BufOut[i].movePos = 0;	// 

				BufOut[i].parentId = base[basecnt].parentId;

				Spawn[0].spawnNum = Spawn[0].spawnNum + 1;
			}
		}

		// ��肩���E���g�ŏI��
		if (PlusBufNum <= basecnt)
		{
			// ���[�v�̉�������̔c��
			Spawn[0].loopcnt = i;

			return;
		}
	}

	// parentid�̐؂�ւ���X�V


		// �����܂ŗ����ꍇ�͐��S��"LIVE"�̏��
		// ��������
		//Spawn[0].spawnNum = Spawn[0].spawnNum - (nowSpawnMaxNum - Spawn[0].nowSpawnNum);

		/*GroupMemoryBarrierWithGroupSync();

		if (nowNum[index] < nowSpawnMaxNum)
		{
			if (BufOut[index].life < 0)
			{
				nowNum[index] ++;
				BufOut[index].pos = float3(0, 0, 100 + -0.001f* index);
				BufOut[index].life = life;
			}
		}


		nowNum[index]++; GroupMemoryBarrierWithGroupSync();*/
	
}
// ����X�|�[����S���񂵂Ă��Ďv��

[numthreads(1, 1, 1)]
void DeadCheck()
{
	for (int i = 0; i < Spawn[0].maxParticleNum; i++)
	{
		// �������������̗͂̌����i�{����group shared�ŋ��L���ăA�b�v�f�[�g���Ō�����������j

		// ��������
		BufOut[i].life = BufOut[i].life - time;

		//// ���S����
		if (BufOut[i].life <= 0)
		{
			if (BufOut[i].Alive == LIVE)
			{
				Spawn[0].spawnNum = Spawn[0].spawnNum - 1;
				BufOut[i].life = -10.0f;
				BufOut[i].Alive = DEAD;
			}
		}
	}
}

// ���������@�����C�ɐ��������@SRV��UAV�Ńf�[�^�̋��L��share�Ńf�[�^����肷��B
[numthreads(1, 1, 1)]
void InitCS(const CSInput input)
{
	// ���݂̐�����������
	Spawn[0].nowSpawnNum = 0;
	Spawn[0].loopcnt = 0;
}

[numthreads(1, 1, 1)]
void InitGameCS(const CSInput input, uint index : SV_GroupIndex)
{
	// ���͍ő吔��
	//for (int i = 0; i < Spawn[0].maxParticleNum; i++)
	//{
	//	//BufOut[i].Alive = DEAD;
	//}
}


#define COMPARISON(a,b) (a.depth > b.depth)
#define COMPARISONLIVE(a) (a.Alive==LIVE )

// ���s�ɕς���
[numthreads(size_x, size_y, size_z)]
void BitonicSort(uint threadid : SV_DispatchThreadID)
{
	int t = threadid;			// thread index
	int low = t & ((int)inc - 1);	// low order bits (below INC)
	int i = (t << 1) - low;		// insert 0 at position INC
	bool reverse = (((int)dir & i) == 0); // asc/desc order

	// Load
	ParticleCompute x0 = BufOut[i];
	ParticleCompute x1 = BufOut[inc + i];

	// Sort
	bool swap = reverse ^ COMPARISON(x0, x1);
	bool swapLIVE1 =  COMPARISONLIVE(x0);
	bool swapLIVE2 =  COMPARISONLIVE(x1);

	ParticleCompute auxa = x0;
	ParticleCompute auxb = x1;
	if (swap&&swapLIVE1&&swapLIVE2) { x0 = auxb; x1 = auxa; }

	// Store
	BufOut[i] = x0;
	BufOut[inc + i] = x1;
}

// �����Ă�����̂Ǝ���ł�����̂̃\�[�g
[numthreads(size_x, size_y, size_z)]
void BitonicSortLive(uint threadid : SV_DispatchThreadID)
{
	int t = threadid;			// thread index
	int low = t & ((int)inc - 1);	// low order bits (below INC)
	int i = (t << 1) - low;		// insert 0 at position INC
	bool reverse = (((int)dir & i) == 0); // asc/desc order

	// Load
	ParticleCompute x0 = BufOut[i];
	ParticleCompute x1 = BufOut[inc + i];

	// Sort  (�y�ʉ��̂��ߏ�����)
	//bool swap = reverse ^ COMPARISONLIVE(x0);
	//ParticleCompute auxa = x0;
	//ParticleCompute auxb = x1;
	//if (swap) { x0 = auxb; x1 = auxa; }

	// Store
	BufOut[i] = x0;
	BufOut[inc + i] = x1;
}


// todo ���󂾂�share�łȂ���΂߂�ǂ��������@
// �������֐��̗p�ӂ�����@�ϐ������̌^��`���ō�邩���ׂ�


// �G�~�b�^�[�ɐ������ԂƏI�����Ԃ���������or�I���t���O����
// �~�ƒ����̐؂蕪��
// �p�[�����m�C�Y
// 3d���ł�
// pbr