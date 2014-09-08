#include <globjects/base/HeapOnly.h>

namespace globjects {

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

} // namespace globjects
