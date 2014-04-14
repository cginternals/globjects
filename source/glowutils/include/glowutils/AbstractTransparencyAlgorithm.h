#pragma once

#include <functional>
#include <string>

#include <glow/ref_ptr.h>

#include <glowutils/glowutils_api.h>

namespace glow {
	class Program;
	class Texture;
	class Shader;
}

namespace glowutils {

class Camera;

/**
    \brief AbstractTransparencyAlgorithm is the super class for different transparency rendering implementations.
    Rendering scenes with transparency using AbstractTransparencyAlgorithm requires the following steps:
        1. Call AbstractTransparencyAlgorithm#initialize
            - pass the directory path with a trailing slash where the transparency shader files are located
            - pass the vertex shader used to render your scene (see AbstractTransparencyAlgorithm#initialize for the requirements regarding the fragment shader)
            - pass the geometry shader used to create scene or nullptr if no geometry shader is used

        2. Call AbstractTransparencyAlgorithm#resize and pass the current viewport size (Should always be called when the viewport size changes).
        3. Call AbstractTransparencyAlgorithm#draw to render your scene with transparency.
            - pass the DrawFunction that creates your scene
            - pass the camera that will be used to render your scene
            - pass the current width and height of the viewport
        4. Use AbstractTransparencyAlgorithm#getOutput to obtain the texture that contains the rendered scene
*/
class GLOWUTILS_API AbstractTransparencyAlgorithm : public glow::Referenced {
public:
    using DrawFunction = std::function<void(glow::Program*)>;

    /**
        \brief initializes the transparency algorithm. Must only be called once.
        \param transparencyShaderFilePath the path with a trailing slash where the transparency shader files are located
        \param vertexShader the vertex shader used to render the scene.
        \param geometryShader the geometry shader used to create the scene or nullptr if no geometry shader is used

        The vertex shader must emit two varying variables:
            float z : The z value (depth) of the vertex in camera space
            vec4 vertex_color : The color of the vertex
        If a geometry shader is used the varying variables must be passed through.
    */
    virtual void initialize(const std::string & transparencyShaderFilePath, glow::Shader *vertexShader, glow::Shader *geometryShader);

    /**
        \brief draws the given scene with transparency
        \param drawFunction the DrawFunction that creates the scene
        \param camera the camera settings to render the scene
        \param width the width of the viewport
        \param height the height of the viewport
    */
    virtual void draw(const DrawFunction& drawFunction, Camera* camera, int width, int height) = 0;

    /**
        \brief resizes the structures used by the transparency algorithm to fit the given viewport
        \param width the width of the viewport
        \param height the height of the viewport
    */
    virtual void resize(int width, int height);

    /**
        \return the texture that contains the scene rendered with transparency after AbstractTransparencyAlgorithm#draw has been called and returned
    */
    virtual glow::Texture* getOutput() = 0;

protected:
    glow::Texture* createColorTex();
};

} // namespace glow
