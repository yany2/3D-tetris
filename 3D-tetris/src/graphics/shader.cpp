#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/gl.h>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>

module graphics;

graphics::Shader::Shader
(
	const char* vertexShaderSourceLocation,
	const char* fragmentShaderSourceLocation
)
{
	auto checkCompileErrrors = [](unsigned int shader, bool program)
	{
		GLenum status = program ? GL_LINK_STATUS : GL_COMPILE_STATUS;
		auto logFunc = program ? glGetProgramInfoLog : glGetShaderInfoLog;
		auto succFunc = program ? glGetProgramiv : glGetShaderiv;
		int success;
		succFunc(shader, status, &success);
		if (!success)
		{
			char* log = new char[1024];
			logFunc(shader, 1024, nullptr, log);
			std::cout << log;
			delete[] log;
		}
	};

	auto compileShader = [](unsigned int shader, const char* sourceLocation)
	{
		using std::istreambuf_iterator;
		std::ifstream sourceFileStream(sourceLocation);
		std::string source = std::string(istreambuf_iterator<char>(sourceFileStream), istreambuf_iterator<char>());
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

graphics::Shader::~Shader()
{
	glDeleteProgram(program);
}

void graphics::Shader::use() const
{
	glUseProgram(program);
}

void graphics::Shader::setUniform(const char* name, int value) const
{
	glUniform1i(glGetUniformLocation(program, name), value);
}

void graphics::Shader::setUniform(const char* name, glm::vec4 value) const
{
	glUniform4fv(glGetUniformLocation(program, name), 1, glm::value_ptr(value));
}

void graphics::Shader::setTransform(glm::mat4* transform) const
{
	glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_FALSE, glm::value_ptr(*transform));
}