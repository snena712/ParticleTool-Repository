#pragma once
#include "CDirectxGraphics.h"
#include <string>

class ImageResource {

private:
	ID3D11ShaderResourceView* srv;
	ID3D11Resource* res;

public:
	ImageResource() {}
	~ImageResource();
	ID3D11ShaderResourceView* GetSRV() const { return srv; }
	ID3D11Resource* GetRES() const { return res; }

	bool Load(std::string _fileName);
	void Unload();
};
