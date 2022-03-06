#include "ImageResource.h"
#include "Shader.h"

ImageResource::~ImageResource() {
	Unload();
}

bool ImageResource::Load(std::string _fileName) {

	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	bool sts = CreateSRVfromFile(_fileName.c_str(), device, devicecontext, &res, &srv);

	return sts;
}

void ImageResource::Unload() {
	if (srv != nullptr) {
		srv->Release();
	}
	if (res != nullptr) {
		res->Release();
	}
}