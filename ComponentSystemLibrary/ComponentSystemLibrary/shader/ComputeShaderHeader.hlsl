#include    "RandomCS.hlsl"
//#include    "MacroHeader.hlsl"
#include	"../source/Compute/ComputeDetail.h"

#define size_x    256
#define size_y      1
#define size_z      1

#define ViewportWidth 1280
#define ViewportHeight 720

#define DEAD      0
#define LIVE      1

//#define ParticleBase 3;

#define EPSILON 1e-3
//---------------------------------
// �t���O���r�b�g�P�ʂŌv�Z����
//---------------------------------

// �p�[�e�B�N���\����  IN�@OUT
struct ParticleCompute
{
	float3 pos;		// �ʒu
	float life;		// ���C�t

	float3 vel;		// �p�[�e�B�N���������Ă����
	int Alive;		// ������

	float3 scale;	// �傫��
	float depth;	// �[�x

	float3 dummy;	// �_�~�[
	float parentId;	// �eID

	float4 color;	// �F

	float3 movePos;	// �p�[�e�B�N���̈ړ���
};

struct ParticleDraw
{
	float3 pos;		// �ʒu
	float dummyParticleDraw1;

	float3 scale;	// �傫��
	float dummyParticleDraw2;

	float4 color;	// �F
};


// �p�[�e�B�N���\����  IN�@OUT
struct ParticleNum
{
	float spawnNum;			// ���݂̐�
	float nowSpawnNum;		// ���t���[���̐������ꂽ��
	float maxParticleNum;	// ����������Ă��ė��p�\�Ȑ�
	float seedcnt;			// �V�[�h�l
	float loopcnt;			// for�̃��[�v��
	float drawcnt;			// 3d�p�̕`��J�E���g
	float dummynum2;		// �_�~�[
	float dummynum3;		// �_�~�[
};

// �萔�o�b�t�@�@�S�̂̊Ǘ��f�[�^

// LIVE��bool(2byte)����@����ID�i�e�j�̔ԍ��ێ� ��������depth���g���\�[�g����

cbuffer ConstantBufferWorld : register(b0)		// ���[���h�ϊ��s��
{
	matrix World;
}

cbuffer ConstantBufferView : register(b1)		// ���[���h�ϊ��s��
{
	matrix View;
}

cbuffer ConstantBufferProjection : register(b2) // ���[���h�ϊ��s��
{
	matrix Projection;
}

// �萔�o�b�t�@�@�T�C�Y�Ǘ�
struct ScaleAnim 
{
	//float3 initScale;	// �����̃T�C�Y
	//float dummyscale;
	//float3 finishScale;	// �ŏI�̑傫��
	//float dummyanim;
};


// �\�[�g�p�ϐ�
cbuffer Sort:register(b5)
{
	float inc;
	float dir ;
	float time;
	float seedkey;
}

struct Base {
	matrix Mtx;				// �����]�̂�

	float3 pos;				// �ꏊ
	float life;				// �������̃��C�t

	float3 vel;				// �ړ���
	int nowSpawnMaxNum;		// ���t���[���̐����ő吔

	float3 spawnPos;		// �����|�W�V����
	float speed;			// �����͈͂̕�

	float3 spawnPosRange;	// �����͈͂̕�
	int parentId;			// �������̐eID

	// �T�C�Y�Ǘ�
	float3 initScale;		// �����̃T�C�Y
	float dummyscale;		// �_�~�[
	float3 finishScale;		// �ŏI�̑傫��
	float dummyanim;		// �_�~�[

	float4 initColor;		// �F
	float4 endColor;		// �F

	float3 velRange;		// �ړ��͈�
	float gravity;			// �d��

	float3 forward;			// �O����
	float forwardDummy;		// �_�~�[

	int velocityFlg;		// �ړ��Ɋւ���t���O
	int colorFlg;			// �F�Ɋւ���t���O
	int Live;				// ������		
	int dummyFlg;			// �_�~�[
};

cbuffer ConstantBufferWorld : register(b6)
{
	Base base[PlusBufNum];
}
	
cbuffer ConstantBufferLight : register(b4)
{
	float4 LightDirection;			// ���̕���
	float4 EyePos;					// ���_�ʒu
	float4 Ambient;
}

// CS�ݒ�
struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};

// In
StructuredBuffer<ParticleDraw> ParticlesRO : register(t0);		// �e�p�[�e�B�N���̏��
Texture2D		g_Tex	   : register(t2);							// �e�N�X�`��
Texture2D		g_TexColor : register(t3);							// �e�N�X�`��

SamplerState	g_SamplerLinear : register(s0);			// �T���v���[

// Out
RWStructuredBuffer<ParticleCompute> BufOut : register(u0);
RWStructuredBuffer<ParticleNum> Spawn : register(u1);
RWStructuredBuffer<ParticleDraw> particleDraw : register(u2);


static groupshared int nowNum[size_x];
groupshared bool check[size_x];


float rand_1_05(in float2 uv, float seed)
{
	float2 noise = (frac(sin(dot(uv, float2(12.9898 + seed, 78.233 + seed)*2.0)) * 43758.5453));
	return abs(noise.x + noise.y) * 0.5;
}

float2 rand_2_10(in float2 uv) {
	float noiseX = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
	float noiseY = sqrt(1 - noiseX * noiseX);
	return float2(noiseX, noiseY);
}

float2 rand_2_0004(in float2 uv)
{
	float noiseX = (frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453));
	float noiseY = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
	return float2(noiseX, noiseY) * 0.004;
}

float rand(float2 co) {
	return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}


float3 mod289(float3 x)
{
	return x - floor(x / 289.0) * 289.0;
}

float4 mod289(float4 x)
{
	return x - floor(x / 289.0) * 289.0;
}

float4 permute(float4 x)
{
	return mod289((x * 34.0 + 1.0) * x);
}

float4 taylorInvSqrt(float4 r)
{
	return 1.79284291400159 - r * 0.85373472095314;
}

float snoise(float3 v)
{
	const float2 C = float2(1.0 / 6.0, 1.0 / 3.0);

	// First corner
	float3 i = floor(v + dot(v, C.yyy));
	float3 x0 = v - i + dot(i, C.xxx);

	// Other corners
	float3 g = step(x0.yzx, x0.xyz);
	float3 l = 1.0 - g;
	float3 i1 = min(g.xyz, l.zxy);
	float3 i2 = max(g.xyz, l.zxy);

	// x1 = x0 - i1  + 1.0 * C.xxx;
	// x2 = x0 - i2  + 2.0 * C.xxx;
	// x3 = x0 - 1.0 + 3.0 * C.xxx;
	float3 x1 = x0 - i1 + C.xxx;
	float3 x2 = x0 - i2 + C.yyy;
	float3 x3 = x0 - 0.5;

	// Permutations
	i = mod289(i); // Avoid truncation effects in permutation
	float4 p =
		permute(permute(permute(i.z + float4(0.0, i1.z, i2.z, 1.0))
			+ i.y + float4(0.0, i1.y, i2.y, 1.0))
			+ i.x + float4(0.0, i1.x, i2.x, 1.0));

	// Gradients: 7x7 points over a square, mapped onto an octahedron.
	// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
	float4 j = p - 49.0 * floor(p / 49.0);  // mod(p,7*7)

	float4 x_ = floor(j / 7.0);
	float4 y_ = floor(j - 7.0 * x_);  // mod(j,N)

	float4 x = (x_ * 2.0 + 0.5) / 7.0 - 1.0;
	float4 y = (y_ * 2.0 + 0.5) / 7.0 - 1.0;

	float4 h = 1.0 - abs(x) - abs(y);

	float4 b0 = float4(x.xy, y.xy);
	float4 b1 = float4(x.zw, y.zw);

	//float4 s0 = float4(lessThan(b0, 0.0)) * 2.0 - 1.0;
	//float4 s1 = float4(lessThan(b1, 0.0)) * 2.0 - 1.0;
	float4 s0 = floor(b0) * 2.0 + 1.0;
	float4 s1 = floor(b1) * 2.0 + 1.0;
	float4 sh = -step(h, 0.0);

	float4 a0 = b0.xzyw + s0.xzyw * sh.xxyy;
	float4 a1 = b1.xzyw + s1.xzyw * sh.zzww;

	float3 g0 = float3(a0.xy, h.x);
	float3 g1 = float3(a0.zw, h.y);
	float3 g2 = float3(a1.xy, h.z);
	float3 g3 = float3(a1.zw, h.w);

	// Normalise gradients
	float4 norm = taylorInvSqrt(float4(dot(g0, g0), dot(g1, g1), dot(g2, g2), dot(g3, g3)));
	g0 *= norm.x;
	g1 *= norm.y;
	g2 *= norm.z;
	g3 *= norm.w;

	// Mix final noise value
	float4 m = max(0.6 - float4(dot(x0, x0), dot(x1, x1), dot(x2, x2), dot(x3, x3)), 0.0);
	m = m * m;
	m = m * m;

	float4 px = float4(dot(x0, g0), dot(x1, g1), dot(x2, g2), dot(x3, g3));
	return 42.0 * dot(m, px);
}


// �Q�_�ԋ����擾
float Distance(float3 _a, float3 _b) {
	return sqrt(pow(_a.x - _b.x, 2.0f) + pow(_a.y - _b.y, 2.0f) + pow(_a.z - _b.z, 2.0f));
}

int CreateSeed(float3 pos)
{
	return pos.x + pos.y + pos.z;
}


float3 snoiseVec3(float3 x) {

	float s = snoise(float3(x));
	float s1 = snoise(float3(x.y - 19.1, x.z + 33.4, x.x + 47.2));
	float s2 = snoise(float3(x.z + 74.2, x.x - 124.5, x.y + 99.4));
	float3 c = float3(s, s1, s2);
	return c;

}



float3 curlNoise(float3 p) {
	const float e = .1;
	float3 dx = float3(e, 0.0, 0.0);
	float3 dy = float3(0.0, e, 0.0);
	float3 dz = float3(0.0, 0.0, e);
	
	float3 p_x0 = snoiseVec3(p - dx);
	float3 p_x1 = snoiseVec3(p + dx);
	float3 p_y0 = snoiseVec3(p - dy);
	float3 p_y1 = snoiseVec3(p + dy);
	float3 p_z0 = snoiseVec3(p - dz);
	float3 p_z1 = snoiseVec3(p + dz);

	float x = p_y1.z - p_y0.z - p_z1.y + p_z0.y;
	float y = p_z1.x - p_z0.x - p_x1.z + p_x0.z;
	float z = p_x1.y - p_x0.y - p_y1.x + p_y0.x;

	const float divisor = 1.0 / (2.0 * e);
	return normalize(float3(x, y, z) * divisor);
}

void SeedChange(float seed)
{
	Spawn[0].seedcnt = frac(rand(float2(seed, seed)));
}

float3 GetSpawnVelocityRange(float3 range)
{
	float3 vel = float3(0, 0, 0);

	float seed = frac(time + Spawn[0].seedcnt + Spawn[0].nowSpawnNum);
	vel.x = rand(float2(Spawn[0].seedcnt, seed + 0.1f)); SeedChange(seed);
	vel.y = rand(float2(Spawn[0].seedcnt, seed + 0.2f)); SeedChange(seed);
	vel.z = rand(float2(Spawn[0].seedcnt, seed + 0.3f)); SeedChange(seed);

	Spawn[0].seedcnt += seed;

	vel.x = (vel.x * 2 - 1.0f) * range.x;
	vel.y = (vel.y * 2 - 1.0f) * range.y;
	vel.z = (vel.z * 2 - 1.0f) * range.z;

	return vel;
}

float3 GetSpawnPosRange(int id, float3 range)
{
	float3 pos = float3(0, 0, 0);
	float seed = frac(time + Spawn[0].seedcnt + Spawn[0].nowSpawnNum);
	pos.x = rand(float2(Spawn[0].seedcnt, seed)); SeedChange(seed * 2);
	pos.y = rand(float2(Spawn[0].seedcnt, seed)); SeedChange(seed);
	pos.z = rand(float2(Spawn[0].seedcnt, seed)); SeedChange(seed);

	Spawn[0].dummynum2 = Spawn[0].seedcnt;

	// �e�Ǐ]
	// ���[���h
	if (base[id].velocityFlg&Velocity_Simulation_Space) {
		pos.x = base[id].spawnPos.x + ((pos.x - 0.5f) * 2) * range.x;
		pos.y = base[id].spawnPos.y + ((pos.y - 0.5f) * 2) * range.y;
		pos.z = base[id].spawnPos.z + ((pos.z - 0.5f) * 2) * range.z;
	}
	else {
		// ���[�J��
		pos.x = ((pos.x - 0.5f) * 2) * range.x;
		pos.y = ((pos.y - 0.5f) * 2) * range.y;
		pos.z = ((pos.z - 0.5f) * 2) * range.z;
	}
	Spawn[0].seedcnt += seed;
	
	return pos;
}


// �X�N���[�����W�̎擾 
//float4 ScreenPos(float3 _pos)
//{
//	float4 Pos = float4(_pos.x, _pos.y, _pos.z, 1.0f);
//	// �X�N���[�����W�v�Z�p
//	float4 ScreenPos;
//	float4 outPos;
//
//
//
//	// ���W�ϊ�
//	//ScreenPos = mul(Pos, wmul); // ���W�ϊ�
//
//	// �X�N���[�����W�ɕϊ�
//	outPos = ScreenPos;
//	outPos.x = (Pos.x / ViewportWidth) * 2 - 1.0f;
//	outPos.y = 1.0f - (Pos.y / ViewportHeight) * 2;
//
//	outPos.z = Pos.z;
//	outPos.w = 1.0f;
//
//	return outPos;
//}

//
//
//// todo ���󂾂�share�łȂ���΂߂�ǂ��������@
//// �������֐��̗p�ӂ�����@�ϐ������̌^��`���ō�邩���ׂ�


// �N�I�[�^�j�I���̂Q��̃f�[�^��n���B���߂�ǂ��Ƃ��R��
// �O�ς��狁�܂�
// ���������]�ʋ��߂Ċ|����ƈړ���