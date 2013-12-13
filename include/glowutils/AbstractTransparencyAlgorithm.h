#pragma once

#include <functional>

#include <glow/ref_ptr.h>

#include <glowutils/glowutils.h>

namespace glow {
	class Program;
	class Texture;
	class Shader;
}

namespace glowutils {

	class Camera;

	class GLOWUTILS_API AbstractTransparencyAlgorithm {
	public:
		typedef std::function<void(glow::Program*)> DrawFunction;

		virtual void initialize(const std::string & transparencyShaderFilePath, glow::Shader *vertexShader, glow::Shader *geometryShader) {}
		virtual void draw(const DrawFunction& drawFunction, Camera* camera, int width, int height) = 0;
		virtual void resize(int width, int height) {}
		virtual glow::Texture* getOutput() = 0;
	protected:
		glow::Texture* createColorTex();
	};

} // namespace glow
