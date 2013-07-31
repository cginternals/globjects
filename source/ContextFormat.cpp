
#include <cassert>

#include <glow/ContextFormat.h>


namespace glow
{

const ContextFormat::MinorsByMajors ContextFormat::m_validVersions
	= ContextFormat::validVersions();


ContextFormat::ContextFormat()
:	m_majorVersion(3)
,	m_minorVersion(2)

,	m_profile(CoreProfile)

,	m_redBufferSize(8)
,	m_greenBufferSize(8)
,	m_blueBufferSize(8)
,	m_alphaBufferSize(8)

,	m_depthBufferSize(24)
,	m_stencilBufferSize(0)

,	m_stereo(false)
,	m_swapBehavior(DoubleBuffer)

,	m_samples(0)
{
}

ContextFormat::~ContextFormat()
{
}

const ContextFormat::MinorsByMajors ContextFormat::validVersions()
{
	MinorsByMajors minorsByMajors;

	minorsByMajors.insert(1, 0);
	minorsByMajors.insert(1, 1);
	minorsByMajors.insert(1, 2);
	minorsByMajors.insert(1, 3);
	minorsByMajors.insert(1, 4);
	minorsByMajors.insert(1, 5);

	minorsByMajors.insert(2, 0);
	minorsByMajors.insert(2, 1);

	minorsByMajors.insert(3, 0);
	minorsByMajors.insert(3, 1);
	minorsByMajors.insert(3, 2);
	minorsByMajors.insert(3, 3);

	minorsByMajors.insert(4, 0);
	minorsByMajors.insert(4, 1);
	minorsByMajors.insert(4, 2);
	minorsByMajors.insert(4, 3);

	return minorsByMajors;
}

void ContextFormat::setVersion(
	const unsigned int major
,	const unsigned int minor)
{
    typedef std::pair<MinorsByMajors::const_iterator, MinorsByMajors::const_iterator> MinorsRange;

	// retrieve nearest valid major
	m_majorVersion = m_validVersions.lower_bound(major)->first;
	const MinorsRange minors(m_validVersions.equal_range(m_majorVersion));

	assert(minors.first != m_validVersions.cend());

    // get nearest valid minor if m_majorVersion is major, else get highest valid minor
	m_minorVersion = 0;
    for (MinorsByMajors::const_iterator i = minors.first; i != minors.second; ++i)
    {
        const unsigned int m(i->second);

        if (m_minorVersion <= m && (major == m_majorVersion && m <= minor))
            m_minorVersion = m;
    }

//    if (minor != m_minorVersion
  //      || major != m_majorVersion)
        // TODO: log should support this!
		//warning("Unknown OpenGL Version %i.%i was adjusted to %i.%i."
        //  , major, minor, m_majorVersion, m_minorVersion);
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

} // namespace glow
