#include "rendertriangles.h"

RenderTriangles::RenderTriangles(Camera *camera)
{
    g_camera = camera;
}

void RenderTriangles::Create(QOpenGLFunctions *functions)
{
    const char *vs = \
                     "attribute vec2 v_position;\n"
                     "attribute vec4 v_color;\n"

                     "uniform mat4 projectionMatrix;\n"
                     "varying vec4 f_color;\n"

                     "void main()\n"
                     "{\n"
                     "	f_color = v_color;\n"
                     "  gl_Position = projectionMatrix * vec4(v_position, 0.0, 1.0);\n"
                     "}";

    const char *fs = \
                     "#ifdef GL_ES\n"
                     "precision lowp float;\n"
                     "#endif\n"

                     "varying vec4 f_color;\n"
                     "  vec4 color;\n"

                     "void main()\n"
                     "{\n"
                     "	color = f_color;\n"
                     "  gl_FragColor = color;\n"
                     "}";

    m_functions = *functions;

    m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vs);
    m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fs);

    m_vertexAttribute = 0;
    m_colorAttribute = 1;

    m_program.bindAttributeLocation("v_position", m_vertexAttribute);
    m_program.bindAttributeLocation("v_color", m_colorAttribute);

    m_program.link();

    m_projectionUniform = m_program.uniformLocation("projectionMatrix");

    m_count = 0;
}

void RenderTriangles::Destroy()
{

}

void RenderTriangles::Vertex(const b2Vec2 &v, const b2Color &c)
{
    if (m_count == e_maxVertices) {
        Flush();
    }

    m_vertices[m_count] = v;
    m_colors[m_count] = c;
    ++m_count;
}

void RenderTriangles::Flush()
{
    if (m_count == 0)
        return;

    m_program.bind();

    m_program.setUniformValue(m_projectionUniform, g_camera->BuildProjectionMatrix(0.2f));

    m_program.enableAttributeArray(m_vertexAttribute);
    m_program.setAttributeArray(m_vertexAttribute, GL_FLOAT, m_vertices, 2);

    m_program.enableAttributeArray(m_colorAttribute);
    m_program.setAttributeArray(m_colorAttribute, GL_FLOAT, m_colors, 4);

    m_functions.glEnable(GL_BLEND);
    m_functions.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_functions.glDrawArrays(GL_TRIANGLES, 0, m_count);
    m_functions.glDisable(GL_BLEND);

    m_program.release();

    m_count = 0;
}

////

//    const char *vs = \
//                     "#version 330\n"
//                     "uniform mat4 projectionMatrix;\n"
//                     "layout(location = 0) in vec2 v_position;\n"
//                     "layout(location = 1) in vec4 v_color;\n"
//                     "out vec4 f_color;\n"
//                     "void main(void)\n"
//                     "{\n"
//                     "    f_color = v_color;\n"
//                     "    gl_Position = projectionMatrix * vec4(v_position, 0.0f, 1.0f);\n"
//                     "}\n";

//    const char *fs = \
//                     "#version 330\n"
//                     "in vec4 f_color;\n"
//                     "out vec4 color;\n"
//                     "void main(void)\n"
//                     "{\n"
//                     "    color = f_color;\n"
//                     "}\n";
