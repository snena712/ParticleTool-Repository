#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include "../DX11System/dx11mathutil.h"
#include "../DX11System/CDirectxGraphics.h"
#include "../Component/GameObject.h"

using namespace std;

typedef struct
{
	float up;
	float down;
	float left;
	float right;
}UV;

class  AnimationIndex		// 内部構造の為使用不可
{
private:
	//Microsoft::WRL::ComPtr<> a;

	vector<XMFLOAT2> m_animlist_xm;				// アニメーションリスト
	vector<UV> m_animlist;						// アニメーションリスト
	XMFLOAT2 stock[4];							// uv一時保存
	string m_animname;							// アニメーションの名前
	int m_cnt;									// uvの保存場所
	unsigned int m_animnum;						// アニメーションの保存場所番号

	//アニメーションの名前とuvの代入(現在未使用)
	//void SetAnimation(XMFLOAT2 _uv) {stock[m_cnt] = _uv;m_cnt++;if (m_cnt == 4) {m_animlist.emplace_back(_uv);}}

public:
	AnimationIndex() { m_cnt = 0; m_animnum = 0; }

	~AnimationIndex() {}
	//アニメーション確保
	XMFLOAT2 GetAnimationXM(unsigned int const& _idx) { return m_animlist_xm[_idx]; }

	UV GetAnimation(unsigned int const& _idx) { return m_animlist[_idx]; }

	//アニメーション保存
	void NameSet(string const& _name) { m_animname = _name; }
	string NameGet() {
		return m_animname;
	}

	//uvの送信
	void Uvsend(UV const& _uv) { m_animlist.emplace_back(); m_animlist[m_cnt] = _uv;	m_cnt++; };

	// 各アニメーションインデックス番号の保存
	void NumSet(unsigned int _num) { m_animnum = _num; }

	// このアニメーションのインデックスの送信
	unsigned int NumGet() { return m_animnum; }

	// このアニメーションのカウントの上限取得
	unsigned int CntGet() { return m_cnt - 1; }

};

namespace Component//各コンポーネントを名前空間で囲ってあげれば、追加するときにComponent::って打つだけでComponentの一覧が出てくるから便利
{
	class Animation : public ComponentBase
	{
		//アニメーションの再生状態
		enum class Status
		{
			UPDATE,
			STOP
		};

	private:

		Status m_status;							// 再生状態
		UV m_uv;									// UVの構造体
		vector < AnimationIndex> m_animlist;		// アニメーションインデックスクラスリスト

		unsigned int m_animcnt, m_animidx;			// アニメーションカウント,インデックス
		unsigned int m_animspd;						// アニメーションスピード
		unsigned int m_begin, m_end;				// アニメーションの初めの番号と終わりの番号
		unsigned int m_nowanim, m_oldanim;			// 進行中のアニメーションと過去のアニメーション
		string		 m_filename;					// ファイル名
		bool		 m_uvchanflg;					// uvの値が変わったフラグ
		bool		 m_endflg = false;				// 終了のフラグ

		bool check_int(string const& str) { return ((all_of(str.cbegin(), str.cend(), isdigit)) ? true : false); }	// 数字か文字の判別

		vector<string> split(string const str, char const del);						// 指定文字まで文字列の読み込み

		bool AnimCheck() { return ((m_nowanim == m_oldanim) ? true : false); }		// アニメーションの現在と過去の比較

		void AnimUpdate();															// アニメーションの更新。アニメーション動かす中心部分

		//==============================================
		// FLOAT２のuv取得（今現在使用不可）
		XMFLOAT2  UvGetXM() { return m_animlist[m_nowanim].GetAnimationXM(m_animidx); }
		// アニメーション情報の取得（現在使用不可）
		void AnimSend(int _id, XMFLOAT2* _uv) { m_animlist.emplace_back(); }
		//==============================================

	public:

		Animation(GameObject* Owner) : ComponentBase(Owner), m_animcnt(0), m_animidx(0), m_animspd(0), m_nowanim(0), m_oldanim(0), m_uvchanflg(true) {}//アニメーション番号(0)
		Animation(GameObject* Owner, unsigned int speed) :ComponentBase(Owner), m_animcnt(0), m_animidx(0), m_animspd(speed), m_nowanim(0), m_oldanim(0), m_uvchanflg(true) {}

		// ベクターはデストラクタが呼ばれると消える。処理中はswapするとデストラクタが起こり消える。deleatは使えず、クリアもメモリ解放されない
		~Animation() {}

		UV  UvGet() { 
			return m_animlist[m_nowanim].GetAnimation(m_animidx); }			// 今現在のアニメーションUV取得(レンダー時に使用)  		
		bool UvCheck() { return ((m_uvchanflg) ? m_uvchanflg = false, true : false); }	// uvが変化しているかの判定

		void fileread(string const fname);												// ファイルの読み込み（上、下、左、右の順に保存）
		string GetFileName() { return m_filename; }										// ファイル名の取得

		void SetSpeed(unsigned int const _spd) { m_animspd = _spd; }					// アニメーションのスピード変更
		unsigned int GetSpeed() { return m_animspd; }									// アニメーションのスピード取得

		// アニメーションの状態変更
		Status StatusGet() { return m_status; }											// アニメーションの状態の取得
		void Stop() { m_status = Status::STOP; }										// アニメーションを止める
		void Start() { m_status = Status::UPDATE; }										// アニメーションを進める

		void SetAnim(int const animnum) {
			m_nowanim = animnum; m_begin = 0; m_end = m_animlist[m_nowanim].CntGet();
		}				// アニメーション番号のセット（セットしたアニメーションをする）		
		unsigned int GetAnim(string const _name);												// 名前と同じアニメーション番号探索して戻す

		void SetSpan(int const _begin, int const _end);											// アニメーションの最初と終わりの設定

		void ResetSpan() { m_begin = 0; m_end = m_animlist[m_nowanim].CntGet(); }				// アニメーションの最初と終わりの初期化（設定中のアニメーションの物にする）


		string GetAnimList() {
			return ((m_filename == "") ? "NONE" : m_animlist[m_nowanim].NameGet()); // 現在のアニメーション名取得(ImGuiで使用中)
		}

		bool GetEndFlg() { return m_endflg; }		// アニメーションが一周したことのフラグ

		void Update() { AnimUpdate(); }

#ifdef DEBUG
		void ImGuiDebug();
#endif
	};
}

// todo compointerを使う

