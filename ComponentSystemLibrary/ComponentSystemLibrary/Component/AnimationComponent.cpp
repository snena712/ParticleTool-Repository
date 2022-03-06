#include "AnimationComponent.h"
#include "ImGuiComponent.h"

using namespace Component;

//ファイルの読み込み
void Animation::fileread(string const fname) {

	string str_buf;
	int n = 0;
	XMFLOAT2 sam[2][2];							//保存1次的配列
	int autcnt = 0, mojicnt = 0;				//文字の場所把握
	string stock;								//名前保存
	unsigned int animidx, animcnt;				//アニメーションを動かすカウント
	int str[2];
	bool autflg = false;						//文字取得したかしてないか

	m_filename = fname;

	// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
	ifstream ifs(fname);

	// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
	while (getline(ifs, str_buf)) {
		//１つ目が文字の時データ送る

		// 「,」区切りごとにデータを読み込むためにistringstream型にする
		istringstream i_stream(str_buf);


		//ファイル情報の抜出
		for (const auto subStr : split(str_buf, ',')) {
			//初めのデータの確認（文字の時はアニメーションとして保存）
			string a = subStr;
			string copy = a;

			//char型に変換
			stock = a;

			// スペースの除去
			if (a[0] == ' ')
			{
				a.erase(0, 1);
			}

			if (a[0] != '\0')
			{
				a.erase(1);// 1以降削除

			}

			//０番から８番まで行い、0にする。０が空欄の時はそのまま読み込み、文字の時は異なるvectorに保存する
			// string の構成文字列が全て数字の場合に true
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
				//vector配列に保存
				m_animlist.emplace_back();

				//名前保存
				m_animlist[n].NameSet(stock);
				//autflg = true;

				//番号の保存
				m_animlist[n].NumSet(n);

				//配列の進行
				n++;
			}
			autcnt++;
		}


		//uvの送信
		m_animlist[n - 1].Uvsend(m_uv);

		//カウントの初期化
		autcnt = 0;
		autflg = false;
	}
}

// アニメーションの更新（上、下、左、右の順に保存）
void Animation::AnimUpdate()
{
	if (m_filename != "")
	{
		// 前回と今回のアニメーションを比較し、正しかったとき
		if (AnimCheck())
		{
			// ストップ以外の時は進む
			if (StatusGet() != Status::STOP) {
				if (m_animidx ==0)
				{
					m_endflg = false;
				}

				// アニメーションカウントがアニメーションの設定スピードよりカウントが多いか？								
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
			// インデックスループを戻す
			if (m_end < m_animidx)
			{
				m_animidx = m_begin;
				m_endflg = true;
			}
		}
		else//異なるとき				
		{
			m_animidx = 0;				// 姿勢アニメーションの初期化
			m_animcnt = m_animspd;		// カウントを上限にし、次のアニメーションへ
			m_oldanim = m_nowanim;		// 新しいアニメーションの情報保存
		}
	}
}

// 同じ名前のアニメーション取得
unsigned int Animation::GetAnim(string const _name)
{
	for (auto& obj : m_animlist) {
		if (obj.NameGet() == _name)
		{
			return obj.NumGet();
		}
	}
	MessageBox(nullptr, "UV SET error ", "error", MB_OK);
	//エラー処理
	return -1;
}

// 指定文字まで文字列の読み込み
vector<string> Animation::split(string str, char del) {
	int first = 0;
	int last = str.find_first_of(del);		//引数の文字まで

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


// アニメーションの最初と終わりの設定
void Animation::SetSpan(int const _begin, int const _end)
{
	m_begin >= 0 ? m_begin = _begin : m_begin = 0;													// 一定値以下の時は強制的に0にする
	m_animlist[m_nowanim].CntGet() >= _end ? m_end = _end : m_end = m_animlist[m_nowanim].CntGet();	// 一定値以上の時は強制的に最大値にする
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
	ImGui::Text(u8"現在のアニメーション :%s", _str);
	ImGui::EndChild();
}

#endif