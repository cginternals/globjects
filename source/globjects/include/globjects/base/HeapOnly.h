
#pragma once

#include <globjects/globjects_api.h>


namespace globjects 
{


/** \brief The HeapOnly class
    Restricts creation to heap

    \see http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Requiring_or_Prohibiting_Heap-based_Objects
 */
class GLOBJECTS_API HeapOnly
{
public:
    HeapOnly();

    void destroy() const;

protected:
    virtual ~HeapOnly();
};


} // namespace globjects
