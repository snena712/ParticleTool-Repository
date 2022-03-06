//// 定数バッファ
//cbuffer CB : register(b0)
//{
//    float3 Position;
//};

//// 入力バッファ(構造化バッファ。読み込み専用)
//StructuredBuffer<float3> Input : register(t0);
//// 出力バッファ(構造化バッファ。読み書き可能)
//RWStructuredBuffer<float3> Result : register(u0);

// 共有メモリ　　　/*　ここにデータ書き込み
//groupshared float shared_data[128];

// シェーダ関数
//[numthreads(128, 1, 1)]   // スレッド グループのスレッド数
//void CS(uint3 Gid : SV_GroupID,         // スレッドが属するのスレッドグループ　　3次元座標
//    uint3 DTid : SV_DispatchThreadID,   // スレッドグループのスレッド            3次元座標
//    uint3 GTid : SV_GroupThreadID,      // すべてのスレッドの中でスレッド識別　　3次元座標
//    uint GI : SV_GroupIndex)            // スレッド番号
//{
//     
//    uint hoge = GI * 1;
//    float3 a = Input[GI];
//    Result[Gid.x].x = a.x;
//    Result[Gid.x].x = a.y;
//    Result[Gid.x].x = a.z;
//}


// パーティクル構造体
struct ParticleCompute
{
    float3 pos;
    float life;
    float3 vel;
    float dummy;
};

cbuffer ConstantBuffer : register(b0)
{
    float3 pos;
    float life;
    float3 vel;
    float dummy;
}

// CS設定
struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};

// In
StructuredBuffer<ParticleCompute> particle : register(t0);
// Out
RWStructuredBuffer<ParticleCompute> BufOut : register(u0);

#define size_x    256
#define size_y      1
#define size_z      1

[numthreads(size_x, size_y, size_z)]
void CS(const CSInput input)
{
	int index = input.dispatch.x;

	float3 result = particle[index].pos + particle[index].vel;

	float3 a = (0, 0, 0);
	//a.x = 0.1f;*/

	BufOut[index].pos.x = particle[index].pos.x + particle[index].vel.x + vel.x;
	BufOut[index].pos.y = particle[index].pos.y + particle[index].vel.y + vel.y;
	BufOut[index].pos.z = particle[index].pos.z + particle[index].vel.z + vel.z;

	BufOut[index].vel = particle[index].vel;

	/*  if (particle[index].life < 0)
	  {
		  BufOut[index].pos = float3(0, 0, -0.001f* index);
		  BufOut[index].life = 50;
	  }*/

	BufOut[index].life = BufOut[index].life - 1;

	BufOut[index].dummy = dummy;
}