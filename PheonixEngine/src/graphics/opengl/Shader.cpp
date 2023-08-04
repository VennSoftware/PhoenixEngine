#include <Phoenix/graphics/opengl/Shader.hpp>
#include <glad/glad.h>
#include <fstream>
#include <iostream>

namespace phnx {
	namespace gfx {
		void Shader::Use()
		{
			glUseProgram(m_program);
		}

		void Shader::UploadInt(const std::string& name, int v)
		{
			int loc = glGetUniformLocation(m_program, name.c_str());
			glUniform1i(loc, v);
		}

		void Shader::UploadInts(const std::string& name, const std::vector<int>& ints)
		{
			int loc = glGetUniformLocation(m_program, name.c_str());
			glUniform1iv(loc, ints.size(), ints.data());
		}

		void Shader::UploadFloat3(const std::string& name, const glm::vec3& v)
		{
			glUniform3f(glGetUniformLocation(m_program, name.c_str()), v.x, v.y, v.z);
		}

		void Shader::UploadFloat2(const std::string& name, const glm::vec2& v)
		{
			glUniform2f(glGetUniformLocation(m_program, name.c_str()), v.x, v.y);
		}

		void Shader::UploadFloat(const std::string& name, float v)
		{
			glUniform1f(glGetUniformLocation(m_program, name.c_str()), v);
		}

		void Shader::UploadFloat4(const std::string& name, const glm::vec4& v)
		{
			glUniform4f(glGetUniformLocation(m_program, name.c_str()), v.x, v.y, v.z, v.w);
		}

		void Shader::UploadMatrix4f(const std::string& name, const glm::mat4& mat)
		{
			int loc = glGetUniformLocation(m_program, name.c_str());
			glProgramUniformMatrix4fv(m_program, loc, 1, false, &mat[0][0]);
		}

		std::string ReadText(const std::string& path) {
			std::ifstream file(path, std::ios::ate);

			if (file.is_open()) {
				size_t size = file.tellg();
				std::string buffer(size, '\0');
				file.seekg(0);

				file.read(&buffer[0], size);

				return buffer;
			}

			PHNX_FATAL("Failed To Load Text File @ '%s'", path.c_str());
		}

		void Shader::CompileSrc(const std::string& vertexPath, const std::string& fragmentPath)
		{
			uint32_t program = glCreateProgram();
			uint32_t vertID, fragID;
			vertID = glCreateShader(GL_VERTEX_SHADER);
			fragID = glCreateShader(GL_FRAGMENT_SHADER);

			auto vs = ReadText(vertexPath);
			auto fs = ReadText(fragmentPath);

			const char* vertSource = vs.c_str();
			const char* fragSource = fs.c_str();

			char info[1024] = { 0 };
			int status = 0;
			std::memset(info, 0, 1024);

			glShaderSource(vertID, 1, &vertSource, NULL);
			glShaderSource(fragID, 1, &fragSource, NULL);

			glCompileShader(vertID);
			glCompileShader(fragID);


			glGetShaderiv(vertID, GL_COMPILE_STATUS, &status);
			if (!status) {
				int logSize = 0;
				glGetShaderInfoLog(vertID, 1024, NULL, info);
				PHNX_WARN("Vertex Shader Failed To Compile: \n\t%s", info);
			}

			glGetShaderiv(fragID, GL_COMPILE_STATUS, &status);
			if (!status) {
				int logSize = 0;
				glGetShaderInfoLog(fragID, 1024, NULL, info);
				PHNX_WARN("Fragment shader Failed To Compile: \n\t%s", info);
			}


			glAttachShader(program, vertID);
			glAttachShader(program, fragID);

			glLinkProgram(program);

			glDetachShader(program, vertID);
			glDetachShader(program, fragID);

			glDeleteShader(vertID);
			glDeleteShader(fragID);

			m_program = program;
		}

		

		std::shared_ptr<Shader> LoadShader(const std::string& vertexPath, const std::string& fragmentPath)
		{
			uint32_t program = glCreateProgram();
			uint32_t vertID, fragID;
			vertID = glCreateShader(GL_VERTEX_SHADER);
			fragID = glCreateShader(GL_FRAGMENT_SHADER);

			auto vs = ReadText(vertexPath);
			auto fs = ReadText(fragmentPath);

			const char* vertSource = vs.c_str();
			const char* fragSource = fs.c_str();

			char info[1024] = { 0 };
			int status = 0;
			std::memset(info, 0, 1024);

			glShaderSource(vertID, 1, &vertSource, NULL);
			glShaderSource(fragID, 1, &fragSource, NULL);
			
			glCompileShader(vertID);
			glCompileShader(fragID);


			glGetShaderiv(vertID, GL_COMPILE_STATUS, &status);
			if (!status){
				int logSize = 0;
				glGetShaderInfoLog(vertID, 1024, NULL, info);
				PHNX_WARN("Vertex Shader Failed To Compile: \n\t%s", info);
			}

			glGetShaderiv(fragID, GL_COMPILE_STATUS, &status);
			if (!status) {
				int logSize = 0;
				glGetShaderInfoLog(fragID, 1024, NULL, info);
				PHNX_WARN("Fragment shader Failed To Compile: \n\t%s", info);
			}


			glAttachShader(program, vertID);
			glAttachShader(program, fragID);

			glLinkProgram(program);

			glDetachShader(program, vertID);
			glDetachShader(program, fragID);

			glDeleteShader(vertID);
			glDeleteShader(fragID);

			return std::make_shared<Shader>(program);
		}
	}
}