#pragma once
#include "../../source/PostEffect/BloomPostEffect.h"
#include "../../source/PostEffect/ShimmerPostEffect.h"

namespace PostEffect
{
	class PostEffectManager {
	public:
		PostEffectManager() { }

		static PostEffectManager* Instance() {
			static PostEffectManager singleton;
			return &singleton;
		}
		void	Init();
		void	ManagerDraw();
		void	AfterRenderClear();
	};
}