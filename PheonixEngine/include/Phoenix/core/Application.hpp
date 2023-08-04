#pragma once
#include <Phoenix/Core.hpp>
#include <Phoenix/core/Window.hpp>

namespace phnx {

	struct AppSpec {
		int width, height;
		const char* title;
	};

	class Application {
	public:
		virtual void OnUpdate(float timestep) = 0;
		virtual void OnRender() = 0;
		virtual void OnCreate();
		virtual void OnDestroy();
		virtual void OnImGui();

	public:
		void Start(const AppSpec& spec);

	protected:
		void Clear(float r, float g, float b, float a);
		double DeltaTime() const { return deltaTime; }

	protected:
		std::unique_ptr<Window> m_window;
	private:
		double deltaTime;
	};
}