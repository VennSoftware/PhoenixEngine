#pragma once

#include <Phoenix/vk/Context.hpp>
#include <Phoenix/vk/RenderQueue.hpp>
int vk_start() {
	using namespace phoenix::vk;
	ContextSpec spec{};

	spec.appName = "PhoenixSandbox";
	spec.engineName = "Phoenix";
	spec.height = 720;
	spec.width = 720;
	spec.title = "Phoenix Sandbox";

	Context ctx(spec);

	std::shared_ptr<RenderQueue> rq = ctx.GetRenderQueue();

	while (ctx.IsActive()) {
		ctx.PollEvents();
		ctx.StartFrame();
		rq->SetViewPort(0, 0, spec.width, spec.height);
		rq->DrawArrays(3);
		ctx.EndFrame();
	}

	ctx.Destroy();
	exit(EXIT_SUCCESS);

}