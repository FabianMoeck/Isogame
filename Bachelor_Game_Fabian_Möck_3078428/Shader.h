#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
class Shader
{
public:
	unsigned int ID;

	Shader() {
		//vertexShader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		//fragmentShader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		//Shaderprogram
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		glValidateProgram(ID);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	~Shader() {

	}

	void use() {
		glUseProgram(ID);
	}
	void deleteShader() {
		glDeleteProgram(ID);
	}

private:
	//basic Shadercode
	const char* vertexShaderSource = "#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
		"}\n\0";

	//basic Shader for color
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	unsigned int vertexShader;
	unsigned int fragmentShader;
};
#endif // !SHADER_H


