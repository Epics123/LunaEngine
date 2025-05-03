#include "lupch.h"

#include "RenderCommand.h"

namespace Luna
{
	UniquePtr<RendererAPI> RenderCommand::sRendererAPI = RendererAPI::Create();

#ifdef LU_DEBUG
	void RenderCommand::PollDebugMessages()
	{
		sRendererAPI->PollDebugMessages();
	}
#endif
}