#include "AnimationComponent.h"
#include "ImGuiComponent.h"

using namespace Component;

//�t�@�C���̓ǂݍ���
void Animation::fileread(string const fname) {

	string str_buf;
	int n = 0;
	XMFLOAT2 sam[2][2];							//�ۑ�1���I�z��
	int autcnt = 0, mojicnt = 0;				//�����̏ꏊ�c��
	string stock;								//���O�ۑ�
	unsigned int animidx, animcnt;				//�A�j���[�V�����𓮂����J�E���g
	int str[2];
	bool autflg = false;						//�����擾���������ĂȂ���

	m_filename = fname;

	// �ǂݍ���csv�t�@�C�����J��(std::ifstream�̃R���X�g���N�^�ŊJ��)
	ifstream ifs(fname);

	// getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e��str_buf�Ɋi�[)
	while (getline(ifs, str_buf)) {
		//�P�ڂ������̎��f�[�^����

		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		istringstream i_stream(str_buf);


		//�t�@�C�����̔��o
		for (const auto subStr : split(str_buf, ',')) {
			//���߂̃f�[�^�̊m�F�i�����̎��̓A�j���[�V�����Ƃ��ĕۑ��j
			string a = subStr;
			string copy = a;

			//char�^�ɕϊ�
			stock = a;

			// �X�y�[�X�̏���
			if (a[0] == ' ')
			{
				a.erase(0, 1);
			}

			if (a[0] != '\0')
			{
				a.erase(1);// 1�ȍ~�폜

			}

			//�O�Ԃ���W�Ԃ܂ōs���A0�ɂ���B�O���󗓂̎��͂��̂܂ܓǂݍ��݁A�����̎��͈قȂ�vector�ɕۑ�����
			// string �̍\�������񂪑S�Đ����̏ꍇ�� true
			if (std::all_of(a.cbegin(), a.cend(), isdigit))
			{

				switch (autcnt)
				{
				case 1:m_uv.up = stod(subStr);
					break;
				case 2:m_uv.down = stod(subStr);
					break;
				case 3:m_uv.left = stod(subStr);
					break;
				case 4:m_uv.right = stod(subStr);
					break;
				default:break;
				}

			}
			else
			{
				//vector�z��ɕۑ�
				m_animlist.emplace_back();

				//���O�ۑ�
				m_animlist[n].NameSet(stock);
				//autflg = true;

				//�ԍ��̕ۑ�
				m_animlist[n].NumSet(n);

				//�z��̐i�s
				n++;
			}
			autcnt++;
		}


		//uv�̑��M
		m_animlist[n - 1].Uvsend(m_uv);

		//�J�E���g�̏�����
		autcnt = 0;
		autflg = false;
	}
}

// �A�j���[�V�����̍X�V�i��A���A���A�E�̏��ɕۑ��j
void Animation::AnimUpdate()
{
	if (m_filename != "")
	{
		// �O��ƍ���̃A�j���[�V�������r���A�����������Ƃ�
		if (AnimCheck())
		{
			// �X�g�b�v�ȊO�̎��͐i��
			if (StatusGet() != Status::STOP) {
				if (m_animidx ==0)
				{
					m_endflg = false;
				}

				// �A�j���[�V�����J�E���g���A�j���[�V�����̐ݒ�X�s�[�h���J�E���g���������H								
				if (m_animcnt < m_animspd)
				{
					m_animcnt++;
				}
				else
				{
					m_animidx++;
					m_animcnt = 0;
					m_uvchanflg = true;
				}
			}
			// �C���f�b�N�X���[�v��߂�
			if (m_end < m_animidx)
			{
				m_animidx = m_begin;
				m_endflg = true;
			}
		}
		else//�قȂ�Ƃ�				
		{
			m_animidx = 0;				// �p���A�j���[�V�����̏�����
			m_animcnt = m_animspd;		// �J�E���g������ɂ��A���̃A�j���[�V������
			m_oldanim = m_nowanim;		// �V�����A�j���[�V�����̏��ۑ�
		}
	}
}

// �������O�̃A�j���[�V�����擾
unsigned int Animation::GetAnim(string const _name)
{
	for (auto& obj : m_animlist) {
		if (obj.NameGet() == _name)
		{
			return obj.NumGet();
		}
	}
	MessageBox(nullptr, "UV SET error ", "error", MB_OK);
	//�G���[����
	return -1;
}

// �w�蕶���܂ŕ�����̓ǂݍ���
vector<string> Animation::split(string str, char del) {
	int first = 0;
	int last = str.find_first_of(del);		//�����̕����܂�

	std::vector<std::string> result;

	while (first < str.size()) {
		string subStr(str, first, last - first);

		result.push_back(subStr);

		first = last + 1;
		last = str.find_first_of(del, first);

		if (last == string::npos) {
			last = str.size();
		}
	}
	return result;
}


// �A�j���[�V�����̍ŏ��ƏI���̐ݒ�
void Animation::SetSpan(int const _begin, int const _end)
{
	m_begin >= 0 ? m_begin = _begin : m_begin = 0;													// ���l�ȉ��̎��͋����I��0�ɂ���
	m_animlist[m_nowanim].CntGet() >= _end ? m_end = _end : m_end = m_animlist[m_nowanim].CntGet();	// ���l�ȏ�̎��͋����I�ɍő�l�ɂ���
}

#ifdef DEBUG

void Animation::ImGuiDebug()
{
	string aaa = GetAnimList();
	const char* _str = aaa.c_str();

	std::string name = owner->GetName() + "A";
	auto* name2 = name.c_str();

	ImGui::BeginChild(name2, ImVec2(0, 60), true);
	ImGui::Text(u8"*Animation");
	ImGui::Text(u8"���݂̃A�j���[�V���� :%s", _str);
	ImGui::EndChild();
}

#endif