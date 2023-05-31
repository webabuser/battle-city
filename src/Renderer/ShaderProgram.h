#pragma once
#include <glad/glad.h>
#include <string>


namespace Renderer {


class ShaderProgram {
public:
	ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
	~ShaderProgram();

	bool isCompiled() const { return m_isCompiled; }
	void use() const;
		
	ShaderProgram() = delete; //Запрещаем конструктор без параметров
	ShaderProgram(ShaderProgram&) = delete;  // запретим конструктор копирования вроде, чтобы нельзя было создать шейдер из другой программы шейдера
	ShaderProgram& operator=(const ShaderProgram&) = delete;  // запрещаем оператор присваивания
	ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
	ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

private:
	bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);

private:
	bool m_isCompiled = false;
	GLuint m_ID = 0;
};



}//namespace Renderer
