#ifndef TOOL_H__
#define TOOL_H__

#include <string>

// TODO ne pas oublier de ne pas definir DEBUGMODE en release
#ifndef DEBUGMODE
#define DEBUGMODE
#endif

#ifdef DEBUGMODE
#	define CHECK_GL_ERROR() Tool::CheckGLError(__FILE__, __LINE__);
#else
#	define CHECK_GL_ERROR()
#endif


class Tool
{
public:
	static void CheckTypes();
	static bool LoadTextFile(const std::string& filename, std::string& buffer);
	static void CheckGLError(const char* file, int line);
};

#endif // TOOL_H__
