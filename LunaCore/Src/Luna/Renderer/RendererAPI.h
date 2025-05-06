#pragma once

#include "Luna/Core/Common.h"

#include "glm/glm.hpp"

namespace Luna
{
	// Generic abstraction for rendering api functionality
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			D3D12 = 1,
			Vulkan = 2
		};

	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void Clear() = 0;

		virtual void SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height) = 0;

		virtual void SetClearColor(const glm::vec4& Color) { mClearColor = Color; }

#ifdef LU_DEBUG
		virtual void PollDebugMessages() = 0;
#endif // LU_DEBUG


		// Get the currently active graphics api
		static API GetAPI() { return sActiveAPI; }
		static UniquePtr<RendererAPI> Create();

	protected:
		// Number of back buffers to use, defaults to 3 (triple buffering)
		static const uint32_t sBufferCount = 3;

		// Viewport dimensions  TODO: Might want to move these elsewhere eventually (or get from window?)
		uint32_t mWidth;
		uint32_t mHeight;

		uint32_t mFrameIndex;

		glm::vec4 mClearColor = { 0.0f, 0.2f, 0.4f, 1.0f};

	private:
		static API sActiveAPI;
	};
}