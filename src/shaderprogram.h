#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QOpenGLShaderProgram>

enum QOpenGLShaderProgramAttribute {
    QOpenGLShaderProgramAttributeVertex,
    QOpenGLShaderProgramAttributeColor,
    QOpenGLShaderProgramAttributeTexturePosition,
    QOpenGLShaderProgramAttributeNUM
};

class ShaderProgram : public QOpenGLShaderProgram
{
public:
    ShaderProgram();

    void Create();

    bool SetVertexShader(const char *code);

    bool SetFragmentShader(const char *code);

    GLuint GetVertexShader();

    GLuint GetFragmentShader();

protected:
    QOpenGLShader *m_vertexShader = Q_NULLPTR;
    QOpenGLShader *m_fragmentShader = Q_NULLPTR;

public:
    GLuint uniform_screenscale;
    GLuint uniform_sampler;

private:
    const char *m_vertexShaderCode =
        "attribute vec2 position;\n"
        "attribute vec4 color;\n"
        "attribute vec2 inputTextureCoordinate0;\n"
        "uniform vec2 screenScale;\n"
        "varying vec4 colorVarying;\n"
        "varying vec2 textureCoordinate0;\n"
        "void main()\n"
        "{\n"
        "   gl_Position=vec4((position*screenScale-1.0), 0.0, 1.0);\n"
        "   colorVarying = color;\n"
        "   textureCoordinate0 = inputTextureCoordinate0;\n"
        "}";

    const char *m_fragmentShaderCode =
        "#ifdef GL_ES\n"
        "// define default precision for float, vec, mat.\n"
        "precision lowp float;\n"
        "#endif\n"
        "varying vec4 colorVarying;\n"
        "#ifdef GL_ES\n"
        "varying lowp vec2 textureCoordinate0;\n"
        "#else\n"
        "varying vec2 textureCoordinate0;\n"
        "#endif\n"
        "uniform sampler2D texture0;\n"
        "void main()\n"
        "{\n"
        "   vec4 color;\n"
        "   vec4 arg1;\n"
        "   vec4 arg2;\n"
        "   vec4 colorTex;\n"
        "   colorTex = texture2D(texture0, textureCoordinate0);\n"
        "   arg1 = colorVarying;\n"
        "   arg2 = colorTex;\n"
        "   color = arg1 * arg2;\n"
        "   gl_FragColor = color;\n"
        "}";
};

#endif // SHADERPROGRAM_H
