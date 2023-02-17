#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>

export module graphics:Shader;

using std::cout;
using std::istreambuf_iterator;
using std::ifstream;
using std::string;

using glm::vec4;
using glm::mat4;
using glm::value_ptr;

namespace graphics {
	class Shader {
	private:
		unsigned int program;
	public:
		Shader(const char* vertexShaderSourceLocation, const char* fragmentShaderSourceLocation) {
			auto checkCompileErrrors = [](unsigned int shader, bool program) {
				GLenum status = program ? GL_LINK_STATUS : GL_COMPILE_STATUS;
				auto logFunc = program ? glGetProgramInfoLog : glGetShaderInfoLog;
				auto succFunc = program ? glGetProgramiv : glGetShaderiv;
				int success;
				succFunc(shader, status, &success);
				if (!success) {
					char* log = new char[1024];
					logFunc(shader, 1024, nullptr, log);
					cout << log;
					delete[] log;
				}
			};

			auto compileShader = [](unsigned int shader, const char* sourceLocation) {
				ifstream sourceFileStream(sourceLocation);
				string source = string(istreambuf_iterator<char>(sourceFileStream), istreambuf_iterator<char>());
				const char* c_source = source.c_str();

				glShaderSource(shader, 1, &c_source, nullptr);
				glCompileShader(shader);
			};

			unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
			compileShader(vertex, vertexShaderSourceLocation);
			checkCompileErrrors(vertex, false);

			unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
			compileShader(fragment, fragmentShaderSourceLocation);
			checkCompileErrrors(fragment, false);

			program = glCreateProgram();
			glAttachShader(program, vertex);
			glAttachShader(program, fragment);
			glLinkProgram(program);
			checkCompileErrrors(program, true);

			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}
		~Shader() {
			glDeleteProgram(program);
		}

		void use() const {
			glUseProgram(program);
		}

		void setUniform(const char* name, int value) const {
			glUniform1i(glGetUniformLocation(program, name), value);
		}

		void setUniform(const char* name, vec4 value) const {
			glUniform4fv(glGetUniformLocation(program, name), 1, value_ptr(value));
		}

		void setTransform(mat4* transform) const {
			glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_FALSE, value_ptr(*transform));
		}
	};
}