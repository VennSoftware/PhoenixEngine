# Building

## Prerequisites (Windows)
- Visual Studio 2022

First, Clone the repository from GitHub.
```bash
git clone https://github.com/VennSoftware/PhoenixEngine
```

After the repo has been cloned, open the Phoenix.sln Solution and Build.


## Basic Setup

```c++
#include <Phoenix/core/Application.hpp>

class PhoenixApp : public phnx::Application {
	virtual void OnUpdate(float timestep) override {
		// Runs Before OnRender();
	}

	virtual void OnRender() override {
	}
};


int main() {
	phnx::AppSpec spec{};

	spec.width = 1280;
	spec.height = 720;
	spec.title = "Phoenix App";

	PhoenixApp app;

	app.Start(spec);

	return 0;
}
```
