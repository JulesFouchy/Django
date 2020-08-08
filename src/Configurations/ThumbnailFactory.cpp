#include "ThumbnailFactory.h"

#include "OpenGL/ComputeShader.h"
#include "stb_image/stb_image_write.h"
#include "Helper/File.h"
#include "ConfigParams.h"
#include "RandomParams.h"

static constexpr int THUMBNAIL_SIZE = 256;
static constexpr int NB_PARTICLES = 100000;
static constexpr float RADIUS = 0.01f;
static constexpr int SSBO_BINDING = 4;

ThumbnailFactory::ThumbnailFactory()
	: m_positionsSSBO(SSBO_BINDING, GL_DYNAMIC_DRAW)
{
    // Read compute shader templates
    MyFile::ToString("internal-shaders/thumbnailShapeTemplate.comp", &m_shapeTemplateSrc);
	// Render pipeline
	m_renderPipeline.addShader(ShaderType::Vertex,   "internal-shaders/configThumbnail.vert");
	m_renderPipeline.addShader(ShaderType::Fragment, "internal-shaders/configThumbnail.frag");
	m_renderPipeline.createProgram();
	// Set SSBO size
	m_positionsSSBO.uploadData(NB_PARTICLES * 2, nullptr);
    // Vertex array
    GLCall(glGenVertexArrays(1, &m_vaoID));
    GLCall(glBindVertexArray(m_vaoID));
    // Vertex buffer
    GLCall(glGenBuffers(1, &m_vboID));
    float vertices[] = {
        -RADIUS, -RADIUS,  0.0f, 0.0f,
         RADIUS, -RADIUS,  1.0f, 0.0f,
         RADIUS,  RADIUS,  1.0f, 1.0f,

        -RADIUS, -RADIUS,  0.0f, 0.0f,
         RADIUS,  RADIUS,  1.0f, 1.0f,
        -RADIUS,  RADIUS,  0.0f, 1.0f
    };
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vboID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
        // Quad vertex positions
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));
        // Quad tex coords
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
}

ThumbnailFactory::~ThumbnailFactory() {
    GLCall(glDeleteBuffers(1, &m_vboID));
    GLCall(glDeleteVertexArrays(1, &m_vaoID));
}

unsigned int ThumbnailFactory::createTexture(ConfigType configType, const std::string& computeShaderCode) {
    unsigned int texID = genTexture();
    switch (configType) {
        case ConfigType::SHAPE_LAYOUT:
        case ConfigType::SVG_LAYOUT:
            createAndApplyComputeShader("#version 430\n" + computeShaderCode + m_shapeTemplateSrc);
            break;
        case ConfigType::STANDALONE:
            break;
        case ConfigType::TEXT:
            break;
        default:
            break;
    }
    drawOnTexture(texID);
    return texID;
}

void ThumbnailFactory::createAndApplyComputeShader(const std::string& shaderSrc) {
    ConfigParams params;
    RandomParams randParams;
    ComputeShader computeShader;
    computeShader.setSrcCode(shaderSrc);
    computeShader.get().bind();
    computeShader.get().setUniform1i("u_NbOfParticles", NB_PARTICLES);
    computeShader.get().setUniform1f("u_aspectRatio", 1.0f);
    computeShader.get().setUniform1i("u_count", params.count);
    computeShader.get().setUniform1i("u_intLR", params.intLR);
    computeShader.get().setUniform1i("u_intUD", params.intUD);
    computeShader.get().setUniform1f("u_wheel", params.wheel);
    computeShader.get().setUniform1f("u_ctrlWheel", params.ctrlWheel);
    computeShader.get().setUniform1f("u_shiftWheel", params.shiftWheel);
    computeShader.get().setUniform1f("u_altWheel", params.altWheel);
    computeShader.get().setUniform1f("u_seed", randParams.seed);
    computeShader.get().setUniform2f("u_xySeed", randParams.xySeed);
    computeShader.compute(NB_PARTICLES);
}

unsigned int ThumbnailFactory::genTexture() {
    unsigned int texID;
    GLCall(glGenTextures(1, &texID));
    GLCall(glBindTexture(GL_TEXTURE_2D, texID));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, THUMBNAIL_SIZE, THUMBNAIL_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    return texID;
}

void ThumbnailFactory::drawOnTexture(unsigned int texID) {
    m_framebuffer.setRenderTarget_Texture(texID, THUMBNAIL_SIZE, THUMBNAIL_SIZE);
    // Clear
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    // Draw
    m_renderPipeline.bind();
    GLCall(glBindVertexArray(m_vaoID));
    GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, NB_PARTICLES));
    //
    m_framebuffer.setRenderTarget_Screen();
}

void ThumbnailFactory::saveTexture(unsigned int texID, const std::string& filepath) {
    m_framebuffer.setRenderTarget_Texture(texID, THUMBNAIL_SIZE, THUMBNAIL_SIZE);
    unsigned char* data = new unsigned char[4 * THUMBNAIL_SIZE * THUMBNAIL_SIZE];
    glReadPixels(0, 0, THUMBNAIL_SIZE, THUMBNAIL_SIZE, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_flip_vertically_on_write(1);
    stbi_write_png(filepath.c_str(), THUMBNAIL_SIZE, THUMBNAIL_SIZE, 4, data, 0);
    delete[] data;
    m_framebuffer.setRenderTarget_Screen();
}