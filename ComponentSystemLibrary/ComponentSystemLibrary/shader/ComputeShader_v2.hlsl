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

#include	"ComputeShaderHeader.hlsl"



float4 TexColorChanger(float _rate)
{
	float2 _uv = float2(_rate, _rate);

	return g_TexColor.SampleLevel(g_SamplerLinear, _uv.y, .5f, 0);
}

// 色更新
void UpdateColor(uint index, uint id)
{
	float _baselife = (float)base[id].life;
	float _buflife = clamp(BufOut[index].life, 0.0f, _baselife);	// マイナスにならないようにする
	float _rate = _buflife / _baselife;

	if (base[id].colorFlg&Color_Event_PictureColor) {


		double initLife = 1.0f - (1.0f - _rate);
		double endLife = (1.0f - _rate);
		float4 _col = initLife * base[id].initColor.xyzw + endLife * base[id].endColor.xyzw;

		// 疑似的なif分
		BufOut[index].color = lerp(_col, float4(0.0, 0.0, 0.0, 0.0), step(_buflife, 0));
	}
	else {
		float4 _col = TexColorChanger(_rate);
		BufOut[index].color = lerp(_col, float4(0.0, 0.0, 0.0, 0.0), step(_buflife, 0));
	}
}

// 色による移動量
float3 ColorMove(uint index, uint id)
{
	float4 _color = BufOut[index].color;
	return (_color.xyz - 0.5f) * 2 * base[id].speed;
}

// 移動更新
void UpdateVelocity(uint index, uint id)
{
	// 移動の計算
	float3 _vel;
	float3 noise = float3(0, 0, 0);

	// 加速度の計算
	_vel.xyz = BufOut[index].vel.xyz + base[id].vel.xyz;

	// 重力加速度の計算
	BufOut[index].vel.y -= base[id].gravity * time;

	// 加速度のプラス
	BufOut[index].vel.xyz += base[id].vel.xyz * time;

	// 波移動
	if (base[id].velocityFlg&Velocity_Event_Sin)
	{
		_vel += curlNoise(BufOut[index].pos) * base[id].speed;
	}

	// 色移動
	if (base[id].velocityFlg&Velocity_Event_ColorMove)
	{
		_vel += ColorMove(index, id);
	}

	// 移動量の計算
	if (base[id].velocityFlg&Velocity_Simulation_Space) {
		BufOut[index].pos.xyz = BufOut[index].pos + _vel.xyz * time;
		particleDraw[index].pos = BufOut[index].pos;
	}
	else {
		BufOut[index].pos.xyz += _vel.xyz * time;	
		particleDraw[index].pos = BufOut[index].pos + base[id].spawnPos;
	}

	// 移動の譲渡
	BufOut[index].vel = BufOut[index].vel;

	// カメラ距離
	float3 campos = 0;
	campos.x = View._14;
	campos.y = View._24;
	campos.z = View._34;

	// カメラからの距離の確定
	BufOut[index].depth = Distance(campos, BufOut[index].pos);

}

// サイズアニメーション
void ScaleAnimCS(uint index,uint id)
{
	// 線形補間
	BufOut[index].scale.xyz = (1.0f - (1.0f - BufOut[index].life / base[id].life)) * base[id].initScale.xyz + (1.0f - BufOut[index].life / base[id].life) * base[id].finishScale.xyz;
}

// Velocityの初期化
float3 InitVelocityCS(uint index)
{
	// 範囲取得
	float3 _vel;

	if (base[index].velocityFlg&Velocity_Event_SHAPE_BOX)
	{
		// 四角
		float3 _volo = GetSpawnVelocityRange(base[index].velRange);

		// 前方向と移動量
		_vel.xyz = base[index].forward.xyz * base[index].speed + _volo.xyz;
	}
	else {
		// 円形
		_vel = GetSpawnVelocityRange(base[index].velRange);
	}
	//戻す

	return _vel;
}

[numthreads(size_x, size_y, size_z)]
void CS(const uint threadid: SV_DispatchThreadID)
{
	int index = threadid;				// 実行番号
	int id = BufOut[index].parentId;	// 親番号
	BufOut[index].parentId = BufOut[index].parentId;

	// 大きさ
	ScaleAnimCS(index, id);

	// 色
	UpdateColor(index,id);

	// 移動
	UpdateVelocity(index,id);

	particleDraw[index].color = BufOut[index].color;
	particleDraw[index].scale = BufOut[index].scale;
}


// 生成処理　現状一気に生成される　SRVかUAVでデータの共有かshareでデータやり取りする。
[numthreads(1, 1, 1)]
void SpawnCS(const CSInput input, uint index : SV_GroupIndex)
{
	//int index = input.dispatch.x;

	// 現在の数の更新（仮の計算）
	//Spawn[0].spawnNum = Spawn[0].spawnNum + nowSpawnMaxNum;

	int basecnt = 0;
	int basespawnnum = 0;

	// 数は最大数回す
	for (int i = 0; i < Spawn[0].maxParticleNum; i++)
	{
		if (basespawnnum >= base[basecnt].nowSpawnMaxNum)
		{
			// プラスのデータの初期化と進行
			basecnt++;
			basespawnnum = 0;
		}
			
		if (basespawnnum< base[basecnt].nowSpawnMaxNum)
		{
			if ((int)BufOut[i].Alive == DEAD)
			{
				// 現在のフレームの生成数と生成可能最大数			
				basespawnnum++;
				Spawn[0].nowSpawnNum++;

				BufOut[i].pos = GetSpawnPosRange(basecnt, base[basecnt].spawnPosRange);

				// 初期の移動量
				BufOut[i].vel = InitVelocityCS(basecnt);
				BufOut[i].life = base[basecnt].life * base[basecnt].Live;
				BufOut[i].Alive = LIVE;

				BufOut[i].movePos = 0;	// 

				BufOut[i].parentId = base[basecnt].parentId;

				Spawn[0].spawnNum = Spawn[0].spawnNum + 1;
			}
		}

		// 一定かうウントで終了
		if (PlusBufNum <= basecnt)
		{
			// ループの回った数の把握
			Spawn[0].loopcnt = i;

			return;
		}
	}

	// parentidの切り替えや更新


		// ここまで来た場合は数全て"LIVE"の状態
		// 減少処理
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
// 現状スポーンを全部回していて思い

[numthreads(1, 1, 1)]
void DeadCheck()
{
	for (int i = 0; i < Spawn[0].maxParticleNum; i++)
	{
		// ＊＊＊＊＊＊体力の減少（本来はgroup sharedで共有してアップデート内で減少処理する）

		// 減少処理
		BufOut[i].life = BufOut[i].life - time;

		//// 死亡処理
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

// 生成処理　現状一気に生成される　SRVかUAVでデータの共有かshareでデータやり取りする。
[numthreads(1, 1, 1)]
void InitCS(const CSInput input)
{
	// 現在の生成数初期化
	Spawn[0].nowSpawnNum = 0;
	Spawn[0].loopcnt = 0;
}

[numthreads(1, 1, 1)]
void InitGameCS(const CSInput input, uint index : SV_GroupIndex)
{
	// 数は最大数回す
	//for (int i = 0; i < Spawn[0].maxParticleNum; i++)
	//{
	//	//BufOut[i].Alive = DEAD;
	//}
}


#define COMPARISON(a,b) (a.depth > b.depth)
#define COMPARISONLIVE(a) (a.Alive==LIVE )

// 奥行に変える
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

// 生きているものと死んでいるもののソート
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

	// Sort  (軽量化のため消した)
	//bool swap = reverse ^ COMPARISONLIVE(x0);
	//ParticleCompute auxa = x0;
	//ParticleCompute auxb = x1;
	//if (swap) { x0 = auxb; x1 = auxa; }

	// Store
	BufOut[i] = x0;
	BufOut[inc + i] = x1;
}


// todo 現状だとshareでなければめんどくさそう　
// 初期化関数の用意をする　変数を何の型や形式で作るか調べる


// エミッターに生成時間と終了時間を持たせるor終了フラグいる
// 円と直線の切り分け
// パー離村ノイズ
// 3dもでる
// pbr