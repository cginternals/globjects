#pragma once

#include <glow/glow.h>
#include <glow/ref_ptr.hpp>

namespace glow {

class GLOW_API Referenced
{
public:
	Referenced();
	virtual ~Referenced();

	void ref();
	void unref();

	int refCounter() const;

protected:
	Referenced(const Referenced &);
	Referenced & operator=(const Referenced &);

private:
    int m_refCounter;
};

} // namespace glow
