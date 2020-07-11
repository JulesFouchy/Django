#include "ShapeLayoutConfig.h"

#include "Particles/ParticlesSystem.h"

#include <fstream>
#include <streambuf>

ShapeLayoutConfig::ShapeLayoutConfig(const std::string& shapeFilepath, const std::string& layoutFilepath) {
    // Read shapeFilepath into a string
    // Thanks to https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    std::ifstream shapeFilestream(shapeFilepath);
    if (!shapeFilestream.is_open()) {
        spdlog::warn("[ShapeLayoutConfig::ShapeLayoutConfig] Failed to open shape file : '{}'", shapeFilepath);
        return;
    }
    std::string shapeSrc;
    shapeFilestream.seekg(0, std::ios::end);
    shapeSrc.reserve(shapeFilestream.tellg());
    shapeFilestream.seekg(0, std::ios::beg);
    shapeSrc.assign(
        (std::istreambuf_iterator<char>(shapeFilestream)),
         std::istreambuf_iterator<char>()
    );
    shapeFilestream.close();
    // Open layoutFilepath
    std::ifstream layoutFilestream(layoutFilepath);
    if (!layoutFilestream.is_open()) {
        spdlog::warn("[ShapeLayoutConfig::ShapeLayoutConfig] Failed to open layout file : '{}'", layoutFilepath);
        return;
    }
    // Parse and include shapeSrc
    std::string shaderSrc = "";
    std::string currLine = "";
    while (std::getline(layoutFilestream, currLine)) {
        shaderSrc += currLine + '\n';
        if (currLine.find("#version") != std::string::npos)
            shaderSrc += shapeSrc;
    }
    layoutFilestream.close();
    //
    m_computeShader.initWithCode(shaderSrc);
}

void ShapeLayoutConfig::applyTo(ParticlesSystem& particlesSystem) {
    m_computeShader.get().bind();
    m_computeShader.get().setUniform1i("u_NbOfParticles", particlesSystem.getNbParticles());
    m_computeShader.get().setUniform1i("u_NbClusters", 3);
    m_computeShader.get().setUniform1f("u_Radius", 0.8);
    m_computeShader.get().setUniform1f("u_Seed", 0);
    m_computeShader.get().setUniform1f("a", 0.1);
    m_computeShader.get().setUniform2f("v", glm::vec2(0.8, 0.7));
    m_computeShader.get().setUniform2f("xyOff", glm::vec2(12, 14.51));
    m_computeShader.get().setUniform1f("delta", 2);
    m_computeShader.compute(particlesSystem.getNbParticles());
    m_computeShader.get().unbind();
}