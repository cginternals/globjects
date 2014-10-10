
#include <common/ContextFormat.h>

#include <cassert>
#include <sstream>
#include <map>

#include <globjects/base/baselogging.h>


using namespace globjects;


ContextFormat::ContextFormat()
: m_version(glbinding::Version(4, 5))
, m_profile(Profile::None)
, m_debugContext(false)
, m_forwardCompatibility(false)
, m_redBufferSize(8)
, m_greenBufferSize(8)
, m_blueBufferSize(8)
, m_alphaBufferSize(8)
, m_depthBufferSize(24)
, m_stencilBufferSize(0)
, m_stereo(false)
, m_swapBehavior(SwapBehavior::DoubleBuffering)
, m_samples(0)
{
}

ContextFormat::~ContextFormat()
{
}

void ContextFormat::setVersion(
    const unsigned int majorVersion
,   const unsigned int minorVersion)
{
    setVersion(glbinding::Version(majorVersion, minorVersion));
}

void ContextFormat::setVersion(const glbinding::Version & version)
{
    m_version = version;
}

glbinding::Version ContextFormat::validateVersion(
    const glbinding::Version & requestedVersion
,   const glbinding::Version & supportedMaximumVersion)
{
#ifdef MAC_OS
    return glbinding::Version(3, 2);
#endif

    glbinding::Version maxv(supportedMaximumVersion);

    if (maxv.isNull())
        maxv = glbinding::Version(3, 0);

    if (requestedVersion.isNull() || requestedVersion > maxv)
        return maxv;

    if (!requestedVersion.isValid())
    {
        glbinding::Version nearest = requestedVersion.nearest();
        return nearest > maxv ? maxv : nearest;
    }
    return requestedVersion;
}

int ContextFormat::majorVersion() const
{
    return m_version.m_major;
}

int ContextFormat::minorVersion() const
{
    return m_version.m_minor;
}

const glbinding::Version & ContextFormat::version() const
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

void ContextFormat::setDebugContext(const bool on)
{
    m_debugContext = on;
}

bool ContextFormat::forwardCompatible() const
{
    return m_forwardCompatibility;
}

void ContextFormat::setForwardCompatible(const bool on)
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

const std::string & ContextFormat::profileString(const Profile profile)
{
    static const std::map<Profile, std::string> profileIdentifier = {
        { Profile::Core, "Core" }
      , { Profile::Compatibility, "Compatibility" }
      , { Profile::None, "None" } };

    return profileIdentifier.at(profile);
}

const std::string & ContextFormat::swapBehaviorString(const SwapBehavior swapBehavior)
{
    static const std::map<SwapBehavior, std::string> swapbIdentifier = {
        { SwapBehavior::Default, "Default" }
      , { SwapBehavior::DoubleBuffering, "DoubleBuffering" }
      , { SwapBehavior::SingleBuffering, "SingleBuffering" }
      , { SwapBehavior::TripleBuffering, "TripleBuffering" } };

    return swapbIdentifier.at(swapBehavior);
}

bool ContextFormat::verify(const ContextFormat & requested, const ContextFormat & created)
{
    return
        verifyVersionAndProfile(requested, created) &&
        verifyPixelFormat(requested, created);
}

bool ContextFormat::verify(const ContextFormat & requested) const
{
    return verify(requested, *this);
}

bool ContextFormat::verifyVersionAndProfile(const ContextFormat & requested, const ContextFormat & created)
{
    const bool sameProfiles(requested.profile() == created.profile());

    if (!sameProfiles)
    {
        warning() << "Profile mismatch for the current context: "
            << profileString(requested.profile()) << " requested, "
            << profileString(created.profile()) << " created.";
    }

    if (requested.version() != created.version())
    {
        warning() << "Version mismatch for the current context: "
            << requested.version().toString() << " requested, "
            << created.version().toString() << " created.";

        if (requested.profile() == Profile::Core)
            return false;
    }
    return sameProfiles;
}

inline void ContextFormat::verifyBufferSize(
    const unsigned int sizeRequested
    , const unsigned int sizeInitialized
    , const std::string & warning
    , std::vector<std::string> & issues)
{
    if (sizeRequested == sizeInitialized)
        return;

    std::stringstream ss;
    ss << warning << " size mismatch: " << sizeRequested << " requested, " << sizeInitialized << " created.";

    issues.push_back(ss.str());
}

bool ContextFormat::verifyPixelFormat(
    const ContextFormat & requested
    , const ContextFormat & created)
{
    std::vector<std::string> issues;

    const bool sameSwapBehaviors(requested.swapBehavior() == created.swapBehavior());

    if (!sameSwapBehaviors)
    {
        warning() << "Swap behavior mismatch for the current context: "
            << swapBehaviorString(requested.swapBehavior()) << " requested, "
            << swapBehaviorString(created.swapBehavior()) << " created.";
    }

    if (requested.depthBufferSize())
    {
        if (!created.depthBufferSize())
            issues.push_back("- Depth Buffer requested, but none created.");
        else
            verifyBufferSize(requested.depthBufferSize(), created.depthBufferSize()
            , "- Depth Buffer", issues);
    }

    verifyBufferSize(requested.redBufferSize(), created.redBufferSize()
        , "- Red Buffer", issues);
    verifyBufferSize(requested.greenBufferSize(), created.greenBufferSize()
        , "- Green Buffer", issues);
    verifyBufferSize(requested.blueBufferSize(), created.blueBufferSize()
        , "- Blue Buffer", issues);
    verifyBufferSize(requested.alphaBufferSize(), created.alphaBufferSize()
        , "- Alpha Buffer", issues);

    if (requested.stencilBufferSize())
    {
        if (!created.stencilBufferSize())
            issues.push_back("- Stencil Buffer requested, but none created.");
        else
            verifyBufferSize(requested.stencilBufferSize(), created.stencilBufferSize()
            , "- Stencil Buffer", issues);
    }

    if (requested.stereo() && !created.stereo())
        issues.push_back("- Stereo Buffering requested, but not initialized.");

    if (requested.samples())
    {
        if (!created.samples())
            issues.push_back("- Sample Buffers requested, but none initialized.");
        else
            verifyBufferSize(requested.samples(), created.samples(), "- Samples ", issues);
    }

    if (issues.empty())
        return true;

    warning() << "Pixelformat mismatch for the current context:";
    for (const std::string & issue : issues)
        warning() << issue;

    return false;
}
