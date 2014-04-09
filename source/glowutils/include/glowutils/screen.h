#pragma once

#include <map>

#include <glowutils/glowutils_api.h>

namespace glowutils
{

/** \brief Static inquiries for primary screen properties.

    All information should be available at anytime.
 */
namespace screen
{
    using Resolutions = std::multimap<unsigned int, unsigned int>;

    GLOWUTILS_API unsigned int getNumScreens();

    /** Returns the primary screen's physical size in mm.
    */
    GLOWUTILS_API void getPhysicalSize(
        unsigned int & width
    ,   unsigned int & height);

    /** Returns the primary screen's maximum resolution in px. 
    */
    GLOWUTILS_API void getMaximumResolution(
        unsigned int & width
    ,   unsigned int & height);

    /** Returns the primary screen's current resolution in px. 
    */
    GLOWUTILS_API void getDesktopResolution(
        unsigned int & width
    ,   unsigned int & height);

    /** Returns the primary screen's pixel density in ppi. 
    */
    GLOWUTILS_API void getPixelDensity(
        float & ppiHorizontal
    ,   float & ppiVertical);


    /** Returns all unique valid resolutions sorted in px. 
    */
    GLOWUTILS_API void getValidResolutions(Resolutions & resolutions);

} // namespace screen

} // namespace glowutils
