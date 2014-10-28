#ifndef SHADER_H
#define SHADER_H
#include <string>
#include "define.h"

class Shader
{
    public:    
        bool Load(const std::string& vertFile, const std::string& fragFile, bool verbose = false);
        void Use() const;

        GLint BindIntUniform(const std::string& name) const;
        void UpdateIntUniform(GLint name, GLint value) const;
        void UpdateFloatUniform(GLint name, GLfloat value) const;

        static void Disable();

    public: // TODO remettre ca private...
        GLenum m_program;
        GLenum m_vertexShader;
        GLenum m_fragmentShader;

    private:
        bool CheckShaderError(GLenum shader, bool verbose);
        bool CheckProgramError(GLenum program, bool showWarning, bool verbose);
};


#endif // SHADER_H
