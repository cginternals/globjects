#include <globjects/base/HeapOnly.h>

namespace glo {

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

} // namespace glo
