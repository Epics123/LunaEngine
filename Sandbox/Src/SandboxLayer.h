#pragma once

#include "Luna/Core/Layer.h"

class SandboxLayer : public Luna::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
};
