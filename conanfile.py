from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
import os


class VtkExamples(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    generators = "VirtualBuildEnv", "VirtualRunEnv"

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("vulkan-headers/1.3.239.0@ft/rc1")
        self.requires("vulkan-loader/1.3.239.0@ft/rc1")
        self.requires("vulkan-validationlayers/1.3.239.0@ft/rc1")
        self.requires("glfw/3.3.8@ft/rc1")
        self.requires("glm/cci.20230113@ft/rc1")
        self.requires("stb/cci.20230920@ft/rc1")
        self.requires("tinyobjloader/1.0.7@ft/rc1")
        self.requires("gtest/1.14.0@ft/rc1")
        self.requires("openvdb/11.0.0@ft/rc1")
        self.requires("opengl/system")
        self.requires("boost/1.80.0")
        self.requires("zlib/1.3")
        self.requires("c-blosc/1.17.0@ft/rc1")
        self.requires("imath/3.1.9")
        self.requires("openexr/3.1.9")
        self.requires("libpng/1.6.40")
        self.requires("onetbb/2020.3")
        self.requires("glew/2.2.0")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        
    def generate(self):
        deps = CMakeDeps(self)
        tc = CMakeToolchain(self)
        tc.cache_variables["MY_CUSTOM_VARIABLE"] = "SomeValue"
        tc.generate()
        deps.generate()
        self.output.info("Generated conan_toolchain.cmake")
        self.output.info(tc.cache_variables["MY_CUSTOM_VARIABLE"])
