#ifndef OPENVDB_RENDERER_SRC_SHADER_VERTEXSHADER_HPP_
#define OPENVDB_RENDERER_SRC_SHADER_VERTEXSHADER_HPP_

#define GLEW_NO_GLU
#include <GL/glew.h>

#include "shader/IShader.hpp"

class VertexShader : public IShader {
public:
    VertexShader(const char *filePath);

    VertexShader(const VertexShader &other)            = delete;
    VertexShader(VertexShader &&other)                 = delete;
    VertexShader &operator=(const VertexShader &other) = delete;
    VertexShader &operator=(VertexShader &&other)      = delete;

    virtual ~VertexShader() noexcept;
};

#endif /* OPENVDB_RENDERER_SRC_SHADER_VERTEXSHADER_HPP_ */
