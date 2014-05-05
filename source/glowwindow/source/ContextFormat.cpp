#include <glowwindow/ContextFormat.h>

#include <cassert>
#include <sstream>

#include <glowbase/baselogging.h>

using namespace glow;


namespace glowwindow
{

ContextFormat::ContextFormat()
: m_profile(AnyProfile)
, m_debugContext(false)
, m_forwardCompatibility(false)
, m_redBufferSize(0)
, m_greenBufferSize(0)
, m_blueBufferSize(0)
, m_alphaBufferSize(8)
, m_depthBufferSize(24)
, m_stencilBufferSize(0)
, m_stereo(false)
, m_swapBehavior(DoubleBuffering)
, m_samples(0)
{
}

ContextFormat::~ContextFormat()
{
}

void ContextFormat::setVersion(unsigned int major, unsigned int minor)
{
    setVersion(Version(major, minor));
}

void ContextFormat::setVersion(const Version & version)
{
    m_version = version;
}

int ContextFormat::majorVersion() const
{
    return m_version.majorVersion;
}

int ContextFormat::minorVersion() const
{
    return m_version.minorVersion;
}

const Version & ContextFormat::version() const
{
    return m_version;
}

ContextFormat::Profile ContextFormat::profile() const
{
	return m_profile;
}

void ContextFormat::setProfile(const ContextFormat::Profile profile)
{
	m_profile = profile;
}

bool ContextFormat::debugContext() const
{
    return m_debugContext;
}

void ContextFormat::setDebugContext(bool on)
{
    m_debugContext = on;
}

bool ContextFormat::forwardCompatible() const
{
    return m_forwardCompatibility;
}

void ContextFormat::setForwardCompatible(bool on)
{
    m_forwardCompatibility = on;
}

int ContextFormat::redBufferSize() const
{
	return m_redBufferSize;
}

void ContextFormat::setRedBufferSize(const int size)
{
	m_redBufferSize = size;
}

int ContextFormat::greenBufferSize() const
{
	return m_greenBufferSize;
}

void ContextFormat::setGreenBufferSize(const int size)
{
	m_greenBufferSize = size;
}

int  ContextFormat::blueBufferSize() const
{
	return m_blueBufferSize;
}

void ContextFormat::setBlueBufferSize(const int size)
{
	m_blueBufferSize = size;
}

int ContextFormat::alphaBufferSize() const
{
	return m_alphaBufferSize;
}

void ContextFormat::setAlphaBufferSize(const int size)
{
	m_alphaBufferSize = size;
}

int ContextFormat::depthBufferSize() const
{
	return m_depthBufferSize;
}

void ContextFormat::setDepthBufferSize(const int size)
{
	m_depthBufferSize = size;
}

int ContextFormat::stencilBufferSize() const
{
	return m_stencilBufferSize;
}

void ContextFormat::setStencilBufferSize(const int size)
{
	m_stencilBufferSize = size;
}

ContextFormat::SwapBehavior ContextFormat::swapBehavior() const
{
	return m_swapBehavior;
}

void ContextFormat::setSwapBehavior(const ContextFormat::SwapBehavior behavior)
{
	m_swapBehavior = behavior;
}

bool ContextFormat::stereo() const
{
	return m_stereo;
}

void ContextFormat::setStereo(const bool enable)
{
	m_stereo = enable;
}

int ContextFormat::samples() const
{
	return m_samples;
}

void ContextFormat::setSamples(const int samples)
{
	m_samples = samples;
}

const char* ContextFormat::profileString(const Profile profile)
{
    switch (profile)
    {
        case CoreProfile:
            return "CoreProfile";
        case CompatibilityProfile:
            return "CompatibilityProfile";
        case AnyProfile:
            return "AnyProfile";
        default:
            return "";
    }
}

const char* ContextFormat::swapBehaviorString(const SwapBehavior swapBehavior)
{
    switch (swapBehavior)
    {
        case SingleBuffering:
            return "SingleBuffering";
        case DoubleBuffering:
            return "DoubleBuffering";
        case TripleBuffering:
            return "TripleBuffering";
        default:
            return "";
    }
}

bool ContextFormat::verify(const ContextFormat & requested, const ContextFormat & created)
{
    return
        verifyVersionAndProfile(requested, created) &&
        verifyPixelFormat(requested, created);
}

bool ContextFormat::verifyVersionAndProfile(const ContextFormat & requested, const ContextFormat & created)
{
	const bool sameProfiles(requested.profile() == created.profile());

	if (!sameProfiles)
	{
        glow::warning() << "A context with a different profile as requested was created: "
            << profileString(requested.profile()) << " requested, "
            << profileString(created.profile()) << " created.";
	}

    if (requested.version() != created.version())
	{
        glow::warning() << "A context with a different OpenGL Version as requested was created: "
            << requested.version() << " requested, "
            << created.version() << "  created.";

		if (requested.profile() == CoreProfile)
			return false;
	}

	return sameProfiles;
}

inline void ContextFormat::verifyBufferSize(
	const unsigned int sizeRequested
,   const unsigned int sizeInitialized
,   const std::string & warning
,   std::vector<std::string> & issues)
{
	if (sizeRequested == sizeInitialized)
		return;

    std::stringstream ss;
    ss << warning << " size mismatch: " << sizeRequested << " requested, " << sizeInitialized << " created.";

    issues.push_back(ss.str());
}

bool ContextFormat::verifyPixelFormat(
	const ContextFormat & requested
,   const ContextFormat & created)
{
	std::vector<std::string> issues;

	const bool sameSwapBehaviors(requested.swapBehavior() == created.swapBehavior());

	if (!sameSwapBehaviors)
	{
        glow::warning() << "A context with a different swap behavior as requested was initialized: "
            << swapBehaviorString(requested.swapBehavior()) << " requested, "
            << swapBehaviorString(created.swapBehavior()) << " created.";
	}

	if (requested.depthBufferSize())
	{
		if (!created.depthBufferSize())
			issues.push_back("Depth Buffer requested, but none created.");
		else
            verifyBufferSize(requested.depthBufferSize(), created.depthBufferSize()
			    , "Depth Buffer", issues);
	}

	verifyBufferSize(requested.redBufferSize(), created.redBufferSize()
		, "Red Buffer", issues);
	verifyBufferSize(requested.greenBufferSize(), created.greenBufferSize()
		, "Green Buffer", issues);
	verifyBufferSize(requested.blueBufferSize(), created.blueBufferSize()
		, "Blue Buffer", issues);
	verifyBufferSize(requested.alphaBufferSize(), created.alphaBufferSize()
		, "Alpha Buffer", issues);

	if (requested.stencilBufferSize())
	{
		if (!created.stencilBufferSize())
			issues.push_back("Stencil Buffer requested, but none created.");
		else
			verifyBufferSize(requested.stencilBufferSize(), created.stencilBufferSize()
			    , "Stencil Buffer", issues);
	}

	if (requested.stereo() && !created.stereo())
		issues.push_back("Stereo Buffering requested, but not initialized.");

	if (requested.samples())
	{
		if (!created.samples())
			issues.push_back("Sample Buffers requested, but none initialized.");
		else
			verifyBufferSize(requested.samples(), created.samples()
			    , "Samples ", issues);
	}

	if (issues.empty())
		return true;

    glow::warning() << "Initialized Pixelformat did not match the Requested One:";
	for(const std::string & issue : issues)
        glow::warning() << issue;

	return false;
}

} // namespace glowwindow
