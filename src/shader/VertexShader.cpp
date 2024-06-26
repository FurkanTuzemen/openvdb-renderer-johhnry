#define GLEW_NO_GLU
#include <GL/glew.h>

#include <iostream>
#include <sstream>

#include "log/Log.hpp"

#include "VertexShader.hpp"

VertexShader::VertexShader(const char *filePath) : IShader(GL_VERTEX_SHADER, filePath) {}

VertexShader::~VertexShader() noexcept {
    std::ostringstream address;
    address << this;
    Log::info("Destructor VertexShader " + address.str());
}
