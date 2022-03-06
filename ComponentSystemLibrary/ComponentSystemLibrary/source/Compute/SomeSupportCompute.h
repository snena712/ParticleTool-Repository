#pragma once
#include "../../imGui/imgui.h"
#include <DirectXTex.h>
#include <string>
#include "../../DX11System/CDirectxGraphics.h"
#include "../../DX11System/shaderhashmap.h"
#include "../../Application.h"

namespace Helper {
	// ヘルパー作成
	static void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	// VS
	// シェダーの引数変かも
	static bool InitVertexShader(ID3D11Device* device,
		const char* szFileName,
		LPCSTR szEntryPoint,
		LPCSTR szShaderModel,
		ID3D11VertexShader** _vs)
	{
		HRESULT hr;

		ID3DBlob* pBlob = nullptr;

		void* ShaderObject;
		size_t	ShaderObjectSize;

		// ファイルの拡張子に合わせてコンパイル
		hr = CompileShader(szFileName, szEntryPoint, szShaderModel, &ShaderObject, ShaderObjectSize, &pBlob);
		if (FAILED(hr))
		{
			if (pBlob)pBlob->Release();
			return false;
		}

		// 頂点シェーダーを生成
		hr = device->CreateVertexShader(ShaderObject, ShaderObjectSize, nullptr, _vs);
		if (FAILED(hr))
		{
			if (pBlob)pBlob->Release();
			return false;
		}

		return true;
	}


	// PS
	static bool InitPixelShader(ID3D11Device* device,
		const char* szFileName,
		LPCSTR szEntryPoint,
		LPCSTR szShaderModel,
		ID3D11PixelShader** _ps)
	{
		bool sts = CreatePixelShader(CDirectXGraphics::GetInstance()->GetDXDevice(),		// デバイスオブジェクト
			szFileName,
			szEntryPoint,
			szShaderModel,
			_ps);

		return sts;
	}

	// GS
	static bool InitGeometryShader(ID3D11Device* device,
		const char* szFileName,
		LPCSTR szEntryPoint,
		LPCSTR szShaderModel,
		ID3D11GeometryShader** _gs)
	{
		bool sts = CreateGeometryShader(CDirectXGraphics::GetInstance()->GetDXDevice(),	// デバイスオブジェクト
			szFileName,
			szEntryPoint,
			szShaderModel,
			_gs);

		return sts;
	}

	// Shaderの初期化
	static bool InitComputeShader(ID3D11Device* device,
		const char* szFileName,
		LPCSTR szEntryPoint,
		LPCSTR szShaderModel,
		ID3D11ComputeShader** _CSShader)
	{
		HRESULT hr;

		ID3DBlob* pBlob = nullptr;

		void* ShaderObject;
		size_t	ShaderObjectSize;

		// ファイルの拡張子に合わせてコンパイル
		hr = CompileShader(szFileName, szEntryPoint, szShaderModel, &ShaderObject, ShaderObjectSize, &pBlob);
		if (FAILED(hr))
		{
			if (pBlob)pBlob->Release();
			return false;
		}

		// コンピュート・シェーダの作成
		hr = device->CreateComputeShader(
			pBlob->GetBufferPointer(),// バイト・コードへのポインタ
			pBlob->GetBufferSize(),	// バイト・コードの長さ
			NULL,
			_CSShader);

		if (FAILED(hr))
		{
			if (pBlob)pBlob->Release();
			return false;
		}

		return true;
	}

	// 文字列の分割
	static std::vector<std::string>splitString(std::string str, char del) {
		int first = 0;
		int last = str.find_first_of(del);

		std::vector<std::string> result;

		while (first < str.size()) {
			std::string subStr(str, first, last - first);

			result.push_back(subStr);

			first = last + 1;
			last = str.find_first_of(del, first);

			if (last == std::string::npos) {
				last = str.size();
			}
		}

		return result;
	}

	// VSの設定
	static  bool SetVS(std::string _name) {
		// 頂点データの定義
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// エレメント数
		unsigned int numElements = ARRAYSIZE(layout);

		// 頂点シェーダーを生成
		auto sts = ShaderHashmap::GetInstance()->SetVertexShader(
			_name,			// 頂点シェーダーファイル名
			layout,					// 頂点レイアウト
			numElements);			// エレメント数

		if (!sts) {
			MessageBox(nullptr, "SetVertexShader error(monotoneps.hlsl)", "error", MB_OK);
			return false;
		}
		return true;
	}

	static  bool SetVS(std::string _name, D3D11_INPUT_ELEMENT_DESC *layout, unsigned int _numElements) {
		

		// 頂点シェーダーを生成
		auto sts = ShaderHashmap::GetInstance()->SetVertexShader(
			_name,			// 頂点シェーダーファイル名
			layout,					// 頂点レイアウト
			_numElements);			// エレメント数

		if (!sts) {
			MessageBox(nullptr, "SetVertexShader error(monotoneps.hlsl)", "error", MB_OK);
			return false;
		}
		return true;
	}

	// VSの設定
	static  bool SetVS(std::string _name, std::string _entryname) {
		// 頂点データの定義
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// エレメント数
		unsigned int numElements = ARRAYSIZE(layout);

		// 頂点シェーダーを生成
		auto sts = ShaderHashmap::GetInstance()->SetVertexShader(
			_name,			// 頂点シェーダーファイル名
			_entryname,		// 関数名
			layout,					// 頂点レイアウト
			numElements);			// エレメント数

		if (!sts) {
			MessageBox(nullptr, "SetVertexShader error(monotoneps.hlsl)", "error", MB_OK);
			return false;
		}
		return true;
	}


	// PSの設定
	static  bool SetPS(const std::string _name) {

		// ピクセルシェーダーを生成
		auto sts = ShaderHashmap::GetInstance()->SetPixelShader(_name);
		if (!sts) {
			MessageBox(nullptr, "SetPixelShader error(monotoneps.hlsl)", "error", MB_OK);
			return false;
		}
		return true;
	}

	// サンプラーの初期化（descの切換えできるようにする）
	static void InitSampler(ID3D11SamplerState** _sam)
	{
		auto _device = CDirectXGraphics::GetInstance()->GetDXDevice();

		// 初期化
		// サンプラステートの設定
		D3D11_SAMPLER_DESC smpDesc;
		memset(&smpDesc, 0, sizeof(smpDesc));
		smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		smpDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		smpDesc.MinLOD = 0;
		smpDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// サンプラステート生成
		auto hr = _device->CreateSamplerState(&smpDesc, _sam);
		if (FAILED(hr))
		{
			MessageBox(nullptr, "Create RenderTarget error", "error", MB_OK);
		}
	}


};

//-----------------------------
// create by Suehara
//-----------------------------