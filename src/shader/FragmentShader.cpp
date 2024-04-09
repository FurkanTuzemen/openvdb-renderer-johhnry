#define GLEW_NO_GLU
#include <GL/glew.h>

#include <iostream>
#include <sstream>

#include "log/Log.hpp"

#include "FragmentShader.hpp"

FragmentShader::FragmentShader(const char *filePath) : IShader(GL_FRAGMENT_SHADER, filePath) {}

FragmentShader::~FragmentShader() noexcept {
    std::ostringstream address;
    address << this;
    Log::info("Destructor FragmentShader " + address.str());
}
