#pragma once

#include <vector>
#include <string>

#include <glow/Version.h>

#include <glowwindow/glowwindow.h>

namespace glowwindow
{

class GLOWWINDOW_API ContextFormat
{
public:
	
	// This is based on QSurfaceFormat::OpenGLContextProfile
	enum Profile
	{
	    CoreProfile          ///< Functionality deprecated in OpenGL version 3.0 is not available.
	,   CompatibilityProfile ///< Functionality from earlier OpenGL versions is available.
    ,   AnyProfile
	};

	// This is based on QSurfaceFormat::SwapBehavior
	enum SwapBehavior
	{
	    SingleBuffering     ///< Might result in flickering when is done directly to screen without an intermediate offscreen buffer.
	,   DoubleBuffering     ///< Rendering is done to the back buffer, which is then swapped with the front buffer.
	,   TripleBuffering     ///< Sometimes used in order to decrease the risk of skipping a frame when the rendering rate is just barely keeping up with the screen refresh rate.
	};

    static const char * profileString(Profile profile);
    static const char * swapBehaviorString(SwapBehavior swapBehavior);

public:
	ContextFormat();
	virtual ~ContextFormat();

	// 24 by default
	int	depthBufferSize() const;
    void setDepthBufferSize(int size);

	int	redBufferSize() const;
    void setRedBufferSize(int size);

	int	greenBufferSize() const;
    void setGreenBufferSize(int size);

	int	blueBufferSize() const;
    void setBlueBufferSize(int size);

	// disabled by default
	int	alphaBufferSize() const;
    void setAlphaBufferSize(int size);

	// disabled by default
	int	samples() const;
	void setSamples(int samples);

	/** For major and minor parameters only valid version pairs are allowed,
        on invalid pairs, nearest major and minor are set.

        Note: OpenGL versions previous to 3.2. are not supported and might not
        work. It is not taken into account in the development of glow.
    */
    void setVersion(const glow::Version & version);
    void setVersion(unsigned int major, unsigned int minor);

    int majorVersion() const;
    int minorVersion() const;
    const glow::Version & version() const;

	Profile profile() const;
    void setProfile(Profile profile);

    bool debugContext() const;
    void setDebugContext(bool on);

    bool forwardCompatible() const;
    void setForwardCompatible(bool on);

	// disabled by default
	int	stencilBufferSize() const;
    void setStencilBufferSize(int size);

	// disabled by default
	bool stereo() const;
    void setStereo(bool enable);

	// default: SwapBehavior::DoubleBuffering
	SwapBehavior swapBehavior() const;
    void setSwapBehavior(SwapBehavior behavior);

public:

    /** Compares the created format against the requested one.
    */
    static bool verify(const ContextFormat & requested, const ContextFormat & created);

protected:

    /** Compares (logged if erroneous) version and profile between both formats
    */
    static bool verifyVersionAndProfile(const ContextFormat & requested, const ContextFormat & current);

    /** Compares (logged if erroneous) buffer sizes and more between both formats
    */
    static bool verifyPixelFormat(const ContextFormat & requested, const ContextFormat & current);


    /** Used as inline by verifyPixelFormat 
    */
    static void verifyBufferSize(
        unsigned int sizeRequested
    ,   unsigned int sizeInitialized
    ,   const std::string & warning
    ,   std::vector<std::string> & issues);

protected:
    glow::Version m_version;

	Profile m_profile;

    bool m_debugContext;
    bool m_forwardCompatibility;

	unsigned int  m_redBufferSize;
	unsigned int  m_greenBufferSize;
	unsigned int  m_blueBufferSize;
	unsigned int  m_alphaBufferSize;

	unsigned int  m_depthBufferSize;
	unsigned int  m_stencilBufferSize;

	bool m_stereo;

	SwapBehavior m_swapBehavior;
	unsigned int m_samples;
};

} // namespace glowwindow
