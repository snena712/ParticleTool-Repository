#pragma once

#include	"ComponentBaseClass.h"
#include	"RenderUtility.h"
#include	"SceneManager.h"
#include	<d3d11.h>

namespace Component
{

	class IRenderBase :public ComponentBase
	{
	public:
		IRenderBase(GameObject* _owner, RenderingType _renderingType) :ComponentBase(_owner), mRenderingType(_renderingType), mSortOrder(0) {}
		virtual ~IRenderBase() { SceneManager::DeleteDrawFunction(mRenderingType, owner); }

		virtual void Start() {}
		virtual void DrawObject() {}
		virtual void DrawUI() {}

		RenderingType GetRenderingType() { return mRenderingType; }

		void SetSortOrder(uint16_t _orderNum) { mSortOrder = _orderNum; }
		uint16_t GetSortOrder() { return mSortOrder; }

#ifdef DEBUG
		virtual void ImGuiDebug() {}
#endif

	protected:
		RenderingType mRenderingType;
		uint16_t mSortOrder;

	};

}