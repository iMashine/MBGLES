#include "renderpoints.h"

RenderPoints::RenderPoints(Camera *camera)
{
    g_camera = camera;
}

void RenderPoints::Create(QOpenGLFunctions *functions)
{
    const char *vs = \
                     "attribute vec2 v_position;\n"
                     "attribute vec4 v_color;\n"
                     "attribute float v_size;\n"

                     "uniform mat4 projectionMatrix;\n"
                     "varying vec4 f_color;\n"

                     "void main(void)\n"
                     "{\n"
                     "	f_color = v_color;\n"
                     "   gl_PointSize = v_size;\n"
                     "	gl_Position = projectionMatrix * vec4(v_position, 0.0, 1.0);\n"
                     "}\n";

    const char *fs = \
                     "#ifdef GL_ES\n"
                     "precision lowp float;\n"
                     "#endif\n"

                     "varying vec4 f_color;\n"
                     "vec4 color;\n"

                     "void main(void)\n"
                     "{\n"
                     "	color = f_color;\n"
                     "  gl_FragColor = color;\n"
                     "}\n";

    m_functions = *functions;

    m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vs);
    m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fs);

    m_vertexAttribute = 0;
    m_colorAttribute = 1;
    m_sizeAttribute = 2;

    m_program.bindAttributeLocation("v_position", m_vertexAttribute);
    m_program.bindAttributeLocation("v_color", m_colorAttribute);
    m_program.bindAttributeLocation("v_size", m_sizeAttribute);

    m_program.link();

    m_projectionUniform = m_program.uniformLocation("projectionMatrix");

    m_count = 0;
}

void RenderPoints::Destroy()
{

}

void RenderPoints::Vertex(const b2Vec2 &v, const b2Color &c, float32 size)
{
    if (m_count == e_maxVertices)
        Flush();

    m_vertices[m_count] = v;
    m_colors[m_count] = c;
    m_sizes[m_count] = size;
    ++m_count;
}

void RenderPoints::Flush()
{
    if (m_count == 0)
        return;

    m_program.bind();

    m_program.setUniformValue(m_projectionUniform, g_camera->BuildProjectionMatrix(0.2f));

    m_program.enableAttributeArray(m_vertexAttribute);
    m_program.setAttributeArray(m_vertexAttribute, GL_FLOAT, m_vertices, 2);

    m_program.enableAttributeArray(m_colorAttribute);
    m_program.setAttributeArray(m_colorAttribute, GL_FLOAT, m_colors, 4);

    m_program.enableAttributeArray(m_sizeAttribute);
    m_program.setAttributeArray(m_sizeAttribute, GL_FLOAT, m_sizes, 1);

    m_functions.glEnable(GL_PROGRAM_POINT_SIZE);
    m_functions.glDrawArrays(GL_POINTS, 0, m_count);
    m_functions.glDisable(GL_PROGRAM_POINT_SIZE);

    m_program.release();

    m_count = 0;
}
