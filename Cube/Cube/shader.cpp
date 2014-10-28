#include "shader.h"
#include "define.h"
#include "tool.h"
#include <iostream>
#include <cassert>

#ifndef WINDOWS

bool Shader::Load(const std::string& vertFile, const std::string& fragFile, bool verbose)
{
    std::string fragmentShader;
    std::string vertexShader;

    if(!Tool::LoadTextFile(vertFile, vertexShader))
    {
        if(verbose)
            std::cout << "Failed to load " << vertFile << std::endl;
        return false;
    }

    if(!Tool::LoadTextFile(fragFile, fragmentShader))
    {
        if(verbose)
            std::cout << "Failed to load " << fragFile << std::endl;
        return false;
    }

    const char * my_fragment_shader_source = fragmentShader.c_str();
    const char * my_vertex_shader_source = vertexShader.c_str();

    //std::cout << fragmentShader << std::endl;
    //std::cout << vertexShader << std::endl;

    m_program = glCreateProgram();
    CHECK_GL_ERROR();
    assert(glIsProgram(m_program));

    m_vertexShader = glCreateShader(GL_VERTEX_SHADER_ARB);
    CHECK_GL_ERROR();
    assert(glIsShader(m_vertexShader));

    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER_ARB);
    CHECK_GL_ERROR();
    assert(glIsShader(m_fragmentShader));

    // Load Shader Sources
    glShaderSource(m_vertexShader, 1, (const GLchar**)&my_vertex_shader_source, NULL);
    CHECK_GL_ERROR();
    glShaderSource(m_fragmentShader, 1, (const GLchar**)&my_fragment_shader_source, NULL);
    CHECK_GL_ERROR();

    // Compile The Shaders
    if(verbose)
        std::cout << "Compiling vertex shader (" << vertFile << ")..." << std::endl;
    glCompileShader(m_vertexShader);
    if(!CheckShaderError(m_vertexShader, verbose))
        return false;

    if(verbose)
        std::cout << "Compiling fragment shader (" << fragFile << ")..." << std::endl;
    glCompileShader(m_fragmentShader);
    if(!CheckShaderError(m_fragmentShader, verbose))
        return false;

    // Attach The Shader Objects To The Program Object
    glAttachShader(m_program, m_vertexShader);
    CHECK_GL_ERROR();
    glAttachShader(m_program, m_fragmentShader);
    CHECK_GL_ERROR();

    // Link The Program Object
    glLinkProgram(m_program);
    //if(!CheckProgramError(m_program, verbose))
    //    return false;
    CheckProgramError(m_program, true, verbose);
    CHECK_GL_ERROR();

    return true;

}

void Shader::Use() const
{
    glUseProgram(m_program);
}

GLint Shader::BindIntUniform(const std::string& name) const
{
    return glGetUniformLocation(m_program, name.c_str());
}

void Shader::UpdateIntUniform(GLint name, GLint value) const
{
    glUniform1i(name, value);
}

void Shader::UpdateFloatUniform(GLint name, GLfloat value) const
{
    glUniform1f(name, value);
}

void Shader::Disable()
{
    glUseProgram(0);
}

bool Shader::CheckShaderError(GLenum shader, bool verbose)
{
    GLint compileOk;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileOk);
    if(verbose && !compileOk)
    {
        int maxLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        char* infoLog = new char[maxLength];

        glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog);

        std::cout << infoLog << std::endl;
        delete [] infoLog;
        return false;
    }

    return compileOk;
}

bool Shader::CheckProgramError(GLenum program, bool showWarning, bool verbose)
{
    GLint compileOk;

    glGetProgramiv(program, GL_LINK_STATUS, &compileOk);
    CHECK_GL_ERROR();
    if(verbose && (showWarning || !compileOk))
    {
        int maxLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        CHECK_GL_ERROR();

        char* infoLog = new char[maxLength];

        glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);
        CHECK_GL_ERROR();

        std::cout << infoLog << std::endl;
        delete [] infoLog;
    }

    return compileOk;
}

#endif
