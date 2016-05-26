
#pragma once

#include <globjects/globjects_api.h>


namespace globjects
{

/**
* @brief
*   The interface for a callable function with variadic argument types.
*
* @see
*   FunctionCall
*/
class GLOBJECTS_API AbstractFunctionCall
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractFunctionCall();

    /**
    * @brief
    *    Virtual destructor
    *
    * Required for correct cleanup of subclasses
    */
    virtual ~AbstractFunctionCall();

    /**
    * @brief
    *   Entry point for the main functionality of a subclass
    *
    * As this interface represents some kind of method object,
    * a subclass defines this operator to implement its full behavior.
    */
    virtual void operator()() = 0;

    /**
    * @brief
    *   Accessor for an arbitrary identifier that can be used as identity
    *
    * @return
    *   An identifier for identity tracking
    */
    virtual void * identifier() const = 0;
};


} // namespace globjects
