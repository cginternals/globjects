#pragma once

#include <glowutils/glowutils.h>
#include <glowutils/AbstractTransparencyAlgorithm.h>

namespace glow {
	class Program;
	class FrameBufferObject;
	class Texture;
	class RenderBufferObject;
	class Buffer;
}

namespace glowutils {

	class Camera;
	class ScreenAlignedQuad;

	class GLOWUTILS_API HybridAlgorithm : public AbstractTransparencyAlgorithm {
	public:
		virtual void initialize(const std::string & transparencyShaderFilePath, glow::Shader *vertexShader, glow::Shader *geometryShader) override;
		virtual void draw(const DrawFunction& drawFunction, Camera* camera, int width, int height) override;
		virtual void resize(int width, int height) override;
		virtual glow::Texture* getOutput() override { return m_colorBuffer.get(); }

	private:
		// shared
		glow::ref_ptr<glow::FrameBufferObject> m_prepassFbo;
		glow::ref_ptr<glow::RenderBufferObject> m_depthBuffer;

		// opaque geometry pass
		glow::ref_ptr<glow::Program> m_opaqueProgram;
		glow::ref_ptr<glow::Texture> m_opaqueBuffer;

		// depth k-TAB pass
		glow::ref_ptr<glow::Program> m_depthKTabProgram;
		glow::ref_ptr<glow::Buffer> m_depthKTab;

		// visibility k-TAB pass
		glow::ref_ptr<glow::Program> m_visibilityKTabProgram;
		glow::ref_ptr<glow::Buffer> m_visibilityKTab;

		// translucent color pass
		glow::ref_ptr<glow::Program> m_colorProgram;
		glow::ref_ptr<glow::FrameBufferObject> m_colorFbo;
		glow::ref_ptr<glow::Texture> m_coreBuffer;
		glow::ref_ptr<glow::Texture> m_accumulationBuffer;
		glow::ref_ptr<glow::Buffer> m_depthComplexityBuffer;

		// composition pass
		glow::ref_ptr<glow::FrameBufferObject> m_compositionFbo;
		glow::ref_ptr<ScreenAlignedQuad> m_compositionQuad;
		glow::ref_ptr<glow::Texture> m_colorBuffer;
	};

} // namespace glow