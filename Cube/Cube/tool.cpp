#include "tool.h"
#include "define.h"
#include <cassert>
#include <iostream>
#include <fstream>

void Tool::CheckTypes()
{
	// Vérifier que les types définis dans define.h ont vraiment la bonne taille
	// Cette vérification se révèlera utile si on change de compilateur/architecture
	// et qu'on oublie de vérifier la taille des types
	assert(sizeof(uint8) == 1);
	assert(sizeof(uint16) == 2);
	assert(sizeof(uint32) == 4);
	assert(sizeof(uint64) == 8);
	assert(sizeof(int8) == 1);
	assert(sizeof(int16) == 2);
	assert(sizeof(int32) == 4);
	assert(sizeof(int64) == 8);
}

bool Tool::LoadTextFile(const std::string& filename, std::string& buffer)
{
	std::ifstream f(filename.c_str(), std::ios::binary);
	if (!f.is_open())
		return false;

	f.seekg(0, std::ios::end);
	unsigned int len = f.tellg();
	f.seekg(0, std::ios::beg);

	char* tmp = new char[len + 1];

	f.read(tmp, len);
	f.close();

	tmp[len] = 0;
	buffer = tmp;
	delete[] tmp;

	return true;
}

void Tool::CheckGLError(const char* file, int line)
{
	GLuint err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cerr << "Opengl error before " << file << "[" << line << "]:" << std::hex << err << "(";

		switch (err)
		{
		case GL_INVALID_ENUM:
			std::cerr << "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			std::cerr << "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			std::cerr << "GL_INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			std::cerr << "GL_STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			std::cerr << "GL_STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			std::cerr << "GL_OUT_OF_MEMORY";
			break;
		case GL_TABLE_TOO_LARGE:
			std::cerr << "GL_TABLE_TOO_LARGE";
			break;
		default:
			std::cerr << "unknown";
		}
		std::cerr << ")" << std::endl;
		std::cerr << "ATTENTION: this error might come from anywhere in the code since the previous call to CHECK_GL_ERROR" << std::endl;
		exit(1);
	}
}
