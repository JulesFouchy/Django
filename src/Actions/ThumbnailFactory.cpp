#include "ThumbnailFactory.h"

#include "stb_image/stb_image_write.h"
#include "Helper/File.h"
#include "Configurations/ConfigParams.h"
#include "Configurations/RandomParams.h"

static constexpr int THUMBNAIL_SIZE = 256;
static constexpr int NB_PARTICLES = 1500;
static constexpr float RADIUS = 0.03f;
static constexpr int SSBO_BINDING = 4;

ThumbnailFactory::ThumbnailFactory()
	: m_positionsSSBO(SSBO_BINDING, GL_DYNAMIC_DRAW)
{
    // Read compute shader templates
    MyFile::ToString("internal-shaders/thumbnailTemplate_Shape.comp",         &m_shapeTemplateSrc);
    MyFile::ToString("internal-shaders/thumbnailTemplate_Layout_before.comp", &m_layoutBeforeTemplateSrc);
    MyFile::ToString("internal-shaders/thumbnailTemplate_Layout_after.comp",  &m_layoutAfterTemplateSrc);
    MyFile::ToString("internal-shaders/thumbnailTemplate_Standalone.comp",    &m_standaloneTemplateSrc);
    // Create SVG compute shader (it's the same shader for every SVG, so we can create it upfront)
    std::string svgSrc;
    MyFile::ToString("internal-shaders/thumbnailTemplate_SVG.comp", &svgSrc);
    svgSrc += m_shapeTemplateSrc;
    m_svgComputeShader.setSrcCode(svgSrc);
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

unsigned int ThumbnailFactory::createTexture(ActionType actionType, const std::string& computeShaderCode) {
    unsigned int texID = genTexture();
    switch (actionType) {
        case ActionType::SHAPE:
            createAndApplyComputeShader("#version 430\n" + computeShaderCode + m_shapeTemplateSrc);
            break;
        case ActionType::LAYOUT:
            createAndApplyComputeShader("#version 430\n" + m_layoutBeforeTemplateSrc + computeShaderCode + m_layoutAfterTemplateSrc);
            break;
        case ActionType::STANDALONE:
            createAndApplyComputeShader("#version 430\n" + computeShaderCode + m_standaloneTemplateSrc);
            break;
        case ActionType::TEXT:
        case ActionType::REROLL_RANDOM:
        case ActionType::SVG_SHAPE:
        default:
            assert(false && "You shouldn't have called this function for this type of action !");
            GLCall(glDeleteTextures(1, &texID));
            return -1;
    }
    drawOnTexture(texID);
    return texID;
}

unsigned int ThumbnailFactory::createTextureForSVG(unsigned int nbCurves, unsigned int offsetInSsbo) {
    unsigned int texID = genTexture();
    m_svgComputeShader.get().bind();
    m_svgComputeShader.get().setUniform1i("u_nbCurves", nbCurves);
    m_svgComputeShader.get().setUniform1i("u_offsetInSsbo", offsetInSsbo);
    m_svgComputeShader.get().setUniform1i("u_NbOfParticles", NB_PARTICLES);
    m_svgComputeShader.get().setUniform1f("u_aspectRatio", 1.0f);
    m_svgComputeShader.compute(NB_PARTICLES);
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
    computeShader.get().setUniform1i("u_LR", params.getLR());
    computeShader.get().setUniform1i("u_UD", params.getUD());
    computeShader.get().setUniform1i("u_wheel", params.getWheel());
    computeShader.get().setUniform1i("u_ctrlWheel", params.getCtrlWheel());
    computeShader.get().setUniform1i("u_shiftWheel", params.getShiftWheel());
    computeShader.get().setUniform1i("u_altWheel", params.getAltWheel());
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