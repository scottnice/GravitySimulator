#pragma once
#include <string>
#include <iostream>
#include "glm/glm/glm.hpp"
#include "Gl/glew.h"
namespace GameLib{

	class Shader
	{
	public:
		Shader(const std::string& shaderProgram, const int shaderType);
		~Shader();
		Shader(const Shader&) = delete;
		Shader(Shader&&);
	private:
		unsigned int shader;
		friend class ShaderProgram;
	};
	class ShaderProgram
	{
	public:
		template<typename... shaderlist>
		ShaderProgram(const Shader& first, shaderlist const&... shaders);
		ShaderProgram(std::initializer_list<Shader*> initList);
		~ShaderProgram();
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&&);
		void useProgram() const;
		void setFloat(const std::string& name, const float f) const;
		void setVec3(const std::string& name, const glm::vec3& vec) const;
		void setVec3(const std::string& name, const float x, const float y, const float z) const;
		void setInt(const std::string & name, const int i) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;
		void setFloat(const char* name, const float f) const;
		void setVec3(const char* name, const glm::vec3& vec) const;
		void setVec3(const char* name, const float x, const float y, const float z) const;
		void setInt(const char* name, const int i) const;
		void setMat4(const char* name, const glm::mat4& mat) const;
	private:
		unsigned int shaderProgram;
		void unpackVariadic(Shader const& first);
		template<typename... shaderlist>
		void unpackVariadic(Shader const& first, shaderlist const&... therest);
		void linkShaders();
	};

	template<typename... shaderlist>
	ShaderProgram::ShaderProgram(const Shader& first, shaderlist const&... shaders):shaderProgram(glCreateProgram()) {
		unpackVariadic(first, shaders...);
	}

	template<typename... shaderlist>
	void ShaderProgram::unpackVariadic(Shader const& first, shaderlist const&... therest) {
		glAttachShader(shaderProgram, first.shader);
		unpackVariadic(therest...);
	}

	Shader make_vertex_shader(const std::string & shader);
	Shader make_fragment_shader(const std::string & shader);
}

