#pragma once
#include <string>
#include <iostream>
#include "glm/glm/glm.hpp"
#include "Gl/glew.h"
namespace GameLib{
	const std::string BASIC_FRAGMENT_SHADER = "#version 330 core\r\nout vec4 FragColor;\r\n\r\nvoid main()\r\n{\r\n    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\r\n}";
	const std::string AMBIENT_LIGHTING_FRAGMENT_SHADER = "#version 330 core\r\nout vec4 FragColor;\r\n\r\nin vec3 Normal;  \r\nin vec3 FragPos;  \r\n  \r\nuniform vec3 lightPos; \r\nuniform vec3 viewPos; \r\nuniform vec3 lightColor;\r\nuniform vec3 objectColor;\r\n\r\nvoid main()\r\n{\r\n    // ambient\r\n    float ambientStrength = 0.1;\r\n    vec3 ambient = ambientStrength * lightColor;\r\n  	\r\n    // diffuse \r\n    vec3 norm = normalize(Normal);\r\n    vec3 lightDir = normalize(lightPos - FragPos);\r\n    float diff = max(dot(norm, lightDir), 0.0);\r\n    vec3 diffuse = diff * lightColor;\r\n    \r\n    // specular\r\n    float specularStrength = 999.5;\r\n    vec3 viewDir = normalize(viewPos - FragPos);\r\n    vec3 reflectDir = reflect(-lightDir, norm);  \r\n    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);\r\n    vec3 specular = specularStrength * spec * lightColor;  \r\n        \r\n    vec3 result = (ambient + diffuse + specular) * objectColor;\r\n    FragColor = vec4(result, 1.0);\r\n} \r\n\r\n";
	const std::string COLORING_FRAGMENT_SHADER = "#version 330 core\r\nout vec4 FragColor;\r\n  \r\nuniform vec3 objectColor;\r\nuniform vec3 lightColor;\r\n\r\nvoid main()\r\n{\r\n    FragColor = vec4(lightColor * objectColor, 1.0);\r\n}";
	const std::string BASIC_VERTEX_SHADER = "#version 330 core\r\nlayout (location = 0) in vec3 aPos;\r\nuniform vec3 offset;\r\nvoid main()\r\n{\r\n    gl_Position = vec4(aPos+offset, 1.0);\r\n}";
	const std::string MODEL_VIEW_VERTEX_SHADER = "#version 330 core\r\nlayout (location = 0) in vec3 aPos;\r\nuniform mat4 view;\r\nuniform mat4 projection;\r\nuniform mat4 model;\r\nvoid main()\r\n{\r\n    gl_Position = projection * view * model * vec4(aPos, 1.0);\r\n}";
	const std::string AMBIENT_LIGHTING_VERTEX_SHADER = "#version 330 core\r\nlayout (location = 0) in vec3 aPos;\r\nlayout (location = 1) in vec3 aNormal;\r\n\r\nout vec3 FragPos;\r\nout vec3 Normal;\r\n\r\nuniform mat4 model;\r\nuniform mat4 view;\r\nuniform mat4 projection;\r\n\r\nvoid main()\r\n{\r\n    FragPos = vec3(model * vec4(aPos, 1.0));\r\n    Normal = mat3(transpose(inverse(model))) * aNormal;  \r\n    \r\n    gl_Position = projection * view * vec4(FragPos, 1.0);\r\n}\r\n\r\n";
	const std::string POINT_LIGHT_FRAGMENT_SHADER = "#version 330 core\r\nout vec4 FragColor;\r\n\r\nin vec3 Normal;  \r\nin vec3 FragPos;  \r\nuniform int numberLights;\r\nuniform vec3 lightPos[20]; \r\nuniform vec3 viewPos; \r\nuniform vec3 lightColor;\r\nuniform vec3 objectColor;\r\nuniform float constant;\r\nuniform float linear;\r\nuniform float quadratic;\r\nvec3 CalcPointLight(vec3 light, vec3 normal, vec3 fragPos, vec3 viewDir);\r\nvoid main()\r\n{\r\n	vec3 viewDir = normalize(viewPos - FragPos);\r\n	vec3 norm = normalize(Normal);\r\n	vec3 result;\r\n	if(constant > 0){\r\n		for(int i = 0; i < numberLights; i++){\r\n			result += CalcPointLight(lightPos[i], norm, FragPos, viewDir);\r\n		}\r\n	}else{\r\n		result = objectColor;\r\n	}\r\n    FragColor = vec4(result, 1.0);\r\n} \r\n\r\n\r\n// calculates the color when using a point light.\r\nvec3 CalcPointLight(vec3 light, vec3 normal, vec3 fragPos, vec3 viewDir)\r\n{\r\n    vec3 lightDir = normalize(light - fragPos);\r\n    // diffuse shading\r\n    float diff = max(dot(normal, lightDir), 0.0);\r\n    // specular shading\r\n    vec3 reflectDir = reflect(-lightDir, normal);\r\n    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);\r\n    // attenuation\r\n    float distance = length(light - fragPos);\r\n    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));    \r\n    // combine results\r\n    vec3 l_ambient = objectColor * 0.05;\r\n    vec3 l_diffuse = lightColor * diff * 0.5;\r\n    vec3 l_specular = lightColor * spec;\r\n	l_diffuse *= attenuation;\r\n    return (l_ambient + l_diffuse + l_specular);\r\n}\r\n\r\n";
	const std::string POINT_LIGHT_VERTEX_SHADER = "\r\n#version 330 core\r\nlayout (location = 0) in vec3 aPos;\r\n\r\n\r\nout vec3 FragPos;\r\nout vec3 Normal;\r\n\r\nuniform mat4 projection;\r\nuniform mat4 view;\r\nuniform mat4 model;\r\n\r\nvoid main()\r\n{\r\n    FragPos = aPos;\r\n    Normal = aPos;\r\n   gl_Position = projection * view * model * vec4(aPos, 1.0);\r\n}\r\n\r\n";

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
		ShaderProgram(shaderlist const&... shaders):shaderProgram(glCreateProgram()) {
			unpackVariadic(shaders...);
		}
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

		template<typename head>
		void unpackVariadic(head const& first) {

			glAttachShader(shaderProgram, first.shader);
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
		template<typename head, typename... shaderlist>
		void unpackVariadic(head const& first, shaderlist const&... therest) {
			glAttachShader(shaderProgram, first.shader);
			unpackVariadic(therest...);
		}
	};

	extern Shader make_vertex_shader(const std::string & shader);
	extern Shader make_fragment_shader(const std::string & shader);
}

