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

class  AnimationIndex		// �����\���̈׎g�p�s��
{
private:
	//Microsoft::WRL::ComPtr<> a;

	vector<XMFLOAT2> m_animlist_xm;				// �A�j���[�V�������X�g
	vector<UV> m_animlist;						// �A�j���[�V�������X�g
	XMFLOAT2 stock[4];							// uv�ꎞ�ۑ�
	string m_animname;							// �A�j���[�V�����̖��O
	int m_cnt;									// uv�̕ۑ��ꏊ
	unsigned int m_animnum;						// �A�j���[�V�����̕ۑ��ꏊ�ԍ�

	//�A�j���[�V�����̖��O��uv�̑��(���ݖ��g�p)
	//void SetAnimation(XMFLOAT2 _uv) {stock[m_cnt] = _uv;m_cnt++;if (m_cnt == 4) {m_animlist.emplace_back(_uv);}}

public:
	AnimationIndex() { m_cnt = 0; m_animnum = 0; }

	~AnimationIndex() {}
	//�A�j���[�V�����m��
	XMFLOAT2 GetAnimationXM(unsigned int const& _idx) { return m_animlist_xm[_idx]; }

	UV GetAnimation(unsigned int const& _idx) { return m_animlist[_idx]; }

	//�A�j���[�V�����ۑ�
	void NameSet(string const& _name) { m_animname = _name; }
	string NameGet() {
		return m_animname;
	}

	//uv�̑��M
	void Uvsend(UV const& _uv) { m_animlist.emplace_back(); m_animlist[m_cnt] = _uv;	m_cnt++; };

	// �e�A�j���[�V�����C���f�b�N�X�ԍ��̕ۑ�
	void NumSet(unsigned int _num) { m_animnum = _num; }

	// ���̃A�j���[�V�����̃C���f�b�N�X�̑��M
	unsigned int NumGet() { return m_animnum; }

	// ���̃A�j���[�V�����̃J�E���g�̏���擾
	unsigned int CntGet() { return m_cnt - 1; }

};

namespace Component//�e�R���|�[�l���g�𖼑O��Ԃň͂��Ă�����΁A�ǉ�����Ƃ���Component::���đł�����Component�̈ꗗ���o�Ă��邩��֗�
{
	class Animation : public ComponentBase
	{
		//�A�j���[�V�����̍Đ����
		enum class Status
		{
			UPDATE,
			STOP
		};

	private:

		Status m_status;							// �Đ����
		UV m_uv;									// UV�̍\����
		vector < AnimationIndex> m_animlist;		// �A�j���[�V�����C���f�b�N�X�N���X���X�g

		unsigned int m_animcnt, m_animidx;			// �A�j���[�V�����J�E���g,�C���f�b�N�X
		unsigned int m_animspd;						// �A�j���[�V�����X�s�[�h
		unsigned int m_begin, m_end;				// �A�j���[�V�����̏��߂̔ԍ��ƏI���̔ԍ�
		unsigned int m_nowanim, m_oldanim;			// �i�s���̃A�j���[�V�����Ɖߋ��̃A�j���[�V����
		string		 m_filename;					// �t�@�C����
		bool		 m_uvchanflg;					// uv�̒l���ς�����t���O
		bool		 m_endflg = false;				// �I���̃t���O

		bool check_int(string const& str) { return ((all_of(str.cbegin(), str.cend(), isdigit)) ? true : false); }	// �����������̔���

		vector<string> split(string const str, char const del);						// �w�蕶���܂ŕ�����̓ǂݍ���

		bool AnimCheck() { return ((m_nowanim == m_oldanim) ? true : false); }		// �A�j���[�V�����̌��݂Ɖߋ��̔�r

		void AnimUpdate();															// �A�j���[�V�����̍X�V�B�A�j���[�V�������������S����

		//==============================================
		// FLOAT�Q��uv�擾�i�����ݎg�p�s�j
		XMFLOAT2  UvGetXM() { return m_animlist[m_nowanim].GetAnimationXM(m_animidx); }
		// �A�j���[�V�������̎擾�i���ݎg�p�s�j
		void AnimSend(int _id, XMFLOAT2* _uv) { m_animlist.emplace_back(); }
		//==============================================

	public:

		Animation(GameObject* Owner) : ComponentBase(Owner), m_animcnt(0), m_animidx(0), m_animspd(0), m_nowanim(0), m_oldanim(0), m_uvchanflg(true) {}//�A�j���[�V�����ԍ�(0)
		Animation(GameObject* Owner, unsigned int speed) :ComponentBase(Owner), m_animcnt(0), m_animidx(0), m_animspd(speed), m_nowanim(0), m_oldanim(0), m_uvchanflg(true) {}

		// �x�N�^�[�̓f�X�g���N�^���Ă΂��Ə�����B��������swap����ƃf�X�g���N�^���N���������Bdeleat�͎g�����A�N���A���������������Ȃ�
		~Animation() {}

		UV  UvGet() { 
			return m_animlist[m_nowanim].GetAnimation(m_animidx); }			// �����݂̃A�j���[�V����UV�擾(�����_�[���Ɏg�p)  		
		bool UvCheck() { return ((m_uvchanflg) ? m_uvchanflg = false, true : false); }	// uv���ω����Ă��邩�̔���

		void fileread(string const fname);												// �t�@�C���̓ǂݍ��݁i��A���A���A�E�̏��ɕۑ��j
		string GetFileName() { return m_filename; }										// �t�@�C�����̎擾

		void SetSpeed(unsigned int const _spd) { m_animspd = _spd; }					// �A�j���[�V�����̃X�s�[�h�ύX
		unsigned int GetSpeed() { return m_animspd; }									// �A�j���[�V�����̃X�s�[�h�擾

		// �A�j���[�V�����̏�ԕύX
		Status StatusGet() { return m_status; }											// �A�j���[�V�����̏�Ԃ̎擾
		void Stop() { m_status = Status::STOP; }										// �A�j���[�V�������~�߂�
		void Start() { m_status = Status::UPDATE; }										// �A�j���[�V������i�߂�

		void SetAnim(int const animnum) {
			m_nowanim = animnum; m_begin = 0; m_end = m_animlist[m_nowanim].CntGet();
		}				// �A�j���[�V�����ԍ��̃Z�b�g�i�Z�b�g�����A�j���[�V����������j		
		unsigned int GetAnim(string const _name);												// ���O�Ɠ����A�j���[�V�����ԍ��T�����Ė߂�

		void SetSpan(int const _begin, int const _end);											// �A�j���[�V�����̍ŏ��ƏI���̐ݒ�

		void ResetSpan() { m_begin = 0; m_end = m_animlist[m_nowanim].CntGet(); }				// �A�j���[�V�����̍ŏ��ƏI���̏������i�ݒ蒆�̃A�j���[�V�����̕��ɂ���j


		string GetAnimList() {
			return ((m_filename == "") ? "NONE" : m_animlist[m_nowanim].NameGet()); // ���݂̃A�j���[�V�������擾(ImGui�Ŏg�p��)
		}

		bool GetEndFlg() { return m_endflg; }		// �A�j���[�V����������������Ƃ̃t���O

		void Update() { AnimUpdate(); }

#ifdef DEBUG
		void ImGuiDebug();
#endif
	};
}

// todo compointer���g��

