#pragma once

#include <glow/ref_ptr.hpp>

namespace glow {

class Referenced
{
public:
	Referenced();

	virtual ~Referenced();

	void ref();
	void unref();

	int refCounter() const;
private:
	int _refCounter;
};

} // namespace glow
