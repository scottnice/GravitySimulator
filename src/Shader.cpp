#include "Shader.h"
#include <exception>
#include "glm/glm/gtc/type_ptr.hpp"
namespace GameLib {

	Shader::Shader(const std::string& shaderProgram, const int shaderType) : shader(glCreateShader(shaderType))
	{
		const GLchar* shaderProgramPointer = shaderProgram.c_str();
		glShaderSource(shader, 1, &shaderProgramPointer, NULL);
		glCompileShader(shader);
		int  success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::Shader compilation failed\n" << infoLog << std::endl;
			throw std::exception(infoLog);
		}
	}
	Shader::~Shader()
	{
		if(shader > 0 )
			glDeleteShader(shader);
	}

	Shader::Shader(Shader && other) : shader(other.shader)
	{
		other.shader = -1;
	}

	void ShaderProgram::setVec3(const std::string & name, const glm::vec3& vec) const
	{
		glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &vec[0]);
	}

	void ShaderProgram::setVec3(const std::string & name, const float x, const float y, const float z) const
	{
		glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x,y,z);
	}

	void ShaderProgram::setInt(const std::string & name, const int i) const
	{
		glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), i);
	}

	void ShaderProgram::setMat4(const std::string & name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void ShaderProgram::setFloat(const std::string & name, const float f) const
	{
		glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), f);
	}

	void ShaderProgram::setVec3(const char* name, const glm::vec3& vec) const
	{
		glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, &vec[0]);
	}

	void ShaderProgram::setVec3(const char*  name, const float x, const float y, const float z) const
	{
		glUniform3f(glGetUniformLocation(shaderProgram, name), x, y, z);
	}

	void ShaderProgram::setInt(const char* name, const int i) const
	{
		glUniform1i(glGetUniformLocation(shaderProgram, name), i);
	}

	void ShaderProgram::setMat4(const char* name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void ShaderProgram::setFloat(const char* name, const float f) const
	{
		glUniform1f(glGetUniformLocation(shaderProgram, name), f);
	}

	ShaderProgram::ShaderProgram(std::initializer_list<Shader*> initList):shaderProgram(glCreateProgram())
	{
		for (auto& s : initList) {
			glAttachShader(shaderProgram, s->shader);
		}
		linkShaders();
	}

	void ShaderProgram::unpackVariadic(Shader const& first) {
		glAttachShader(shaderProgram, first.shader);
		linkShaders();
	}
	void ShaderProgram::linkShaders() {
		glLinkProgram(shaderProgram);

		int  success;
		char infoLog[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::Shader Program Link Failed\n" << infoLog << std::endl;
			throw std::exception(infoLog);
		}
	}
	ShaderProgram::~ShaderProgram()
	{
		if(shaderProgram > 0)
			glDeleteProgram(shaderProgram);
	}
	ShaderProgram::ShaderProgram(ShaderProgram && other):shaderProgram(other.shaderProgram)
	{
		other.shaderProgram = -1;
	}
	void ShaderProgram::useProgram() const
	{
		glUseProgram(shaderProgram);
	}

	Shader make_vertex_shader(const std::string & shader)
	{
		return Shader{ shader, GL_VERTEX_SHADER };
	}
	Shader make_fragment_shader(const std::string & shader)
	{
		return Shader{ shader, GL_FRAGMENT_SHADER };
	}
}