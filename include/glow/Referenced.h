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
private:
	int _refCounter;
protected:
	Referenced(const Referenced&);
	Referenced& operator=(const Referenced&);
};

} // namespace glow
