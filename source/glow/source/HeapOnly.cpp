#include <glow/HeapOnly.h>

namespace glow {

HeapOnly::HeapOnly()
{
}

void HeapOnly::destroy() const
{
    delete this;
}

HeapOnly::~HeapOnly()
{
}

} // namespace glow
