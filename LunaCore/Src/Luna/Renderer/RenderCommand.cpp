#include "lupch.h"

#include "RenderCommand.h"

namespace Luna
{
	UniquePtr<RendererAPI> RenderCommand::sRendererAPI = RendererAPI::Create();
}