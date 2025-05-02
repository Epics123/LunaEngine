#pragma once

#include "Core.h"
#include "Common.h"

namespace Luna
{
	class Layer
	{
	public:
		Layer(const std::string& Name = "UntitledLayer");
		virtual ~Layer() = default;

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};

		const std::string& GetName() const { return mDebugName; }

	protected:
		std::string mDebugName;
	};
}