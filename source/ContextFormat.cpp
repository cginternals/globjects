
#include <cassert>
#include <sstream>

#include <glow/logging.h>
#include <glow/ContextFormat.h>


namespace glow
{

const ContextFormat::MinorsByMajors ContextFormat::s_validVersions
	= ContextFormat::validVersions();


ContextFormat::ContextFormat()
:	m_majorVersion(0)
,	m_minorVersion(0)

,	m_profile(CoreProfile)

,	m_redBufferSize(0)
,	m_greenBufferSize(0)
,	m_blueBufferSize(0)
,	m_alphaBufferSize(8)

,	m_depthBufferSize(24)
,	m_stencilBufferSize(0)

,	m_stereo(false)
,	m_swapBehavior(DoubleBuffering)

,	m_samples(0)
{
    setVersion(4, 1);   // Use setter to avoid invalid initialization.
}

ContextFormat::~ContextFormat()
{
}

ContextFormat::MinorsByMajors ContextFormat::validVersions()
{
	MinorsByMajors minorsByMajors;

    typedef std::pair<unsigned int, unsigned int> Version;

	minorsByMajors.insert(Version(1, 0));
	minorsByMajors.insert(Version(1, 1));
	minorsByMajors.insert(Version(1, 2));
	minorsByMajors.insert(Version(1, 3));
	minorsByMajors.insert(Version(1, 4));
	minorsByMajors.insert(Version(1, 5));

	minorsByMajors.insert(Version(2, 0));
	minorsByMajors.insert(Version(2, 1));

	minorsByMajors.insert(Version(3, 0));
	minorsByMajors.insert(Version(3, 1));

	minorsByMajors.insert(Version(3, 2));
	minorsByMajors.insert(Version(3, 3));

	minorsByMajors.insert(Version(4, 0));
	minorsByMajors.insert(Version(4, 1));
	minorsByMajors.insert(Version(4, 2));
	minorsByMajors.insert(Version(4, 3));
    minorsByMajors.insert(Version(4, 4));

	return minorsByMajors;
}

bool ContextFormat::nearestValidVersion(
    unsigned int & major
,   unsigned int & minor)
{
    typedef std::pair<MinorsByMajors::const_iterator, MinorsByMajors::const_iterator> MinorsRange;

    auto f = s_validVersions.lower_bound(major);

    assert(!s_validVersions.empty());

    if (s_validVersions.cend() == f)
    {
        --f;

        major = f->first;
        minor = f->second;

        return true;
    }

	// retrieve nearest valid major
	unsigned int ma = f->first;
	const MinorsRange minors(s_validVersions.equal_range(ma));

	assert(minors.first != s_validVersions.cend());

    // get nearest valid minor if m_majorVersion is major, else get highest valid minor
	unsigned int mi = 0;
    for (MinorsByMajors::const_iterator i = minors.first; i != minors.second; ++i)
    {
        const unsigned int m(i->second);

        if (mi <= m && (major == ma && m <= minor))
            mi = m;
    }

    const bool changed = !(ma == major && mi == minor);
    
    major = ma;
    minor = mi;

    return changed;
}

void ContextFormat::setVersion(
	const unsigned int major
,	const unsigned int minor)
{
    m_majorVersion = major;
    m_minorVersion = minor;

    if(nearestValidVersion(m_majorVersion, m_minorVersion))
		warning() << "Unknown OpenGL Version " << major << "." << minor << " was adjusted to " << m_majorVersion << "." << m_minorVersion << ".";
}

void ContextFormat::setVersionFallback(
    unsigned int major
,	unsigned int minor)
{
    nearestValidVersion(major, minor);

    if (major > m_majorVersion || (major == m_majorVersion && minor >= m_minorVersion))
        return;

    warning() << "Unsupported OpenGL Version " << m_majorVersion << "." << m_minorVersion << " was adjusted to " << major << "." << minor << ".";

    m_majorVersion = major;
    m_minorVersion = minor;
}

unsigned int ContextFormat::majorVersion() const
{
	return m_majorVersion;
}

unsigned int ContextFormat::minorVersion() const
{
	return m_minorVersion;
}

ContextFormat::Profile ContextFormat::profile() const
{
	return m_profile;
}

void ContextFormat::setProfile(const ContextFormat::Profile profile)
{
	m_profile = profile;
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

const std::string ContextFormat::profileString(const Profile profile)
{
    switch (profile)
    {
    case CoreProfile:
        return "CoreProfile";
    case CompatibilityProfile:
        return "CompatibilityProfile";
    default:
        return "";
    }
}

const std::string ContextFormat::swapBehaviorString(const SwapBehavior swapb)
{
    switch (swapb)
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

bool ContextFormat::verify(
	const ContextFormat & requested
,   const ContextFormat & created)
{
	bool result = true;

	result &= verifyVersionAndProfile(requested, created);

    // Version Disclaimer
    if (3 > created.majorVersion() || (3 == created.majorVersion() && 2 > created.minorVersion()))
        fatal() << "OpenGL Versions prior to 3.2 (" << created.majorVersion() << "." << created.minorVersion() << " created)"
            << " are not supported within glow. This might result in erroneous behaviour.";

	result &= verifyPixelFormat(requested, created);

	return result;
}

bool ContextFormat::verifyVersionAndProfile(
	const ContextFormat & requested
,   const ContextFormat & created)
{
	const bool sameProfiles(requested.profile() == created.profile());

	if (!sameProfiles)
	{
		warning() << "A context with a different profile as requested was created: "
            << profileString(requested.profile()) << " requested, "
            << profileString(created.profile()) << " created.";
	}

	if (requested.majorVersion() != created.majorVersion()
     || requested.minorVersion() != created.minorVersion())
	{
		warning() << "A context with a different OpenGL Version as requested was created: "
		    << requested.majorVersion() << "." << requested.minorVersion() << " requested, "
            << created.majorVersion() << "." << created.minorVersion() << "  created.";

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
		warning() << "A context with a different swap behavior as requested was initialized: "
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

	warning() << "Initialized Pixelformat did not match the Requested One:";
	for(const std::string & issue : issues)
		warning() << issue;

	return false;
}

} // namespace glow
