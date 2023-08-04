#pragma once
#include <Phoenix/Core.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
namespace phnx {
	namespace gfx {

		class Shader {
		public:
			void Use();

			void UploadInt(const std::string& name, int v);
			void UploadInts(const std::string& name, const std::vector<int>& ints);

			void UploadFloat3(const std::string& name, const glm::vec3& v);

			void UploadFloat2(const std::string& name, const glm::vec2& v);

			void UploadFloat(const std::string& name, float v);

			void UploadFloat4(const std::string& name, const glm::vec4& v);

			void UploadMatrix4f(const std::string& name, const glm::mat4& mat);


			void CompileSrc(const std::string& vertexPath, const std::string& fragmentPath);

		public:
			Shader(uint32_t program) : m_program(program) {}
			Shader() : m_program(0) {}

		protected:
			

		private:
			uint32_t m_program;
		};

		std::shared_ptr<Shader> LoadShader(const std::string& vertexPath, const std::string& fragmentPath);
	}
}