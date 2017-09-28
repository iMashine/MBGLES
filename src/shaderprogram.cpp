#include "shaderprogram.h"

ShaderProgram::ShaderProgram()
    : QOpenGLShaderProgram(Q_NULLPTR)
{

}

void ShaderProgram::Create()
{
    m_vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    m_fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);

    m_vertexShader->compileSourceCode(m_vertexShaderCode);
    m_fragmentShader->compileSourceCode(m_fragmentShaderCode);

    this->addShader(m_vertexShader);
    this->addShader(m_fragmentShader);

    this->bindAttributeLocation("position", QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeVertex);
    this->bindAttributeLocation("color", QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeColor);
    this->bindAttributeLocation("inputTextureCoordinate0", QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeTexturePosition);
    this->link();

    uniform_screenscale = this->uniformLocation("screenScale");
    uniform_sampler = this->uniformLocation("texture0");
}

bool ShaderProgram::SetVertexShader(const char *code)
{
    return m_vertexShader->compileSourceCode(code);
}

bool ShaderProgram::SetFragmentShader(const char *code)
{
    return m_fragmentShader->compileSourceCode(code);
}

GLuint ShaderProgram::GetVertexShader()
{
    return m_vertexShader->shaderId();
}

GLuint ShaderProgram::GetFragmentShader()
{
    return m_fragmentShader->shaderId();
}
