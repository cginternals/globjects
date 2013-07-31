#pragma once

#include <map>

#include <glow/glow.h>


namespace glow
{

class GLOW_API ContextFormat
{
public:
	
	// This is based on QSurfaceFormat::OpenGLContextProfile
	enum Profile
	{
	    NoProfile       ///< OpenGL version is lower than 3.2.
	,   CoreProfile     ///< Functionality deprecated in OpenGL version 3.0 is not available.
	,   SurfaceFormat   ///< Functionality from earlier OpenGL versions is available.
	};

	// This is based on QSurfaceFormat::SwapBehavior
	enum SwapBehavior
	{
	    SingleBuffer    ///< Might result in flickering when is done directly to screen without an intermediate offscreen buffer.
	,   DoubleBuffer    ///< Rendering is done to the back buffer, which is then swapped with the front buffer.
	,   TripleBuffer    ///< Sometimes used in order to decrease the risk of skipping a frame when the rendering rate is just barely keeping up with the screen refresh rate.
	};

public:
	ContextFormat();
	virtual ~ContextFormat();

	// 24 by default
	int	depthBufferSize() const;
	void setDepthBufferSize(const int size);

	int	redBufferSize() const;
	void setRedBufferSize(const int size);

	int	greenBufferSize() const;
	void setGreenBufferSize(const int size);

	int	blueBufferSize() const;
	void setBlueBufferSize(const int size);

	// disabled by default
	int	alphaBufferSize() const;
	void setAlphaBufferSize(const int size);

	// disabled by default
	int	samples() const;
	void setSamples(int samples);

	// only valid version pairs are allowed, on invalid pairs, 
	// nearest major and minor are set.
	void setVersion(
	    const unsigned int major
	,   const unsigned int minor);

	unsigned int majorVersion() const; 
	unsigned int minorVersion() const;

	Profile profile() const;
	void setProfile(const Profile profile);

	// disabled by default
	int	stencilBufferSize() const;
	void setStencilBufferSize(const int size);

	// disabled by default
	bool stereo() const;
	void setStereo(const bool enable);

	// default: SwapBehavior::DoubleBuffer
	SwapBehavior swapBehavior() const;
	void setSwapBehavior(const SwapBehavior behavior);

protected:
	typedef std::multimap<unsigned int, unsigned int> MinorsByMajors;
	static const MinorsByMajors validVersions();

protected:
	unsigned int m_majorVersion;
	unsigned int m_minorVersion;

	static const MinorsByMajors m_validVersions;

	Profile m_profile;

	unsigned int  m_redBufferSize;
	unsigned int  m_greenBufferSize;
	unsigned int  m_blueBufferSize;
	unsigned int  m_alphaBufferSize;

	unsigned int  m_depthBufferSize;
	unsigned int  m_stencilBufferSize;

	unsigned int m_samples;
	bool m_stereo;

	SwapBehavior m_swapBehavior;
};

} // namespace glow
