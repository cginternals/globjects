#pragma once

#include <map>

#include <glowutils/glowutils.h>


namespace glowutils
{

/** \brief Static inquiries for primary screen properties.

    All information should be available at anytime.
 */
class GLOWUTILS_API Screen
{
public:

    static unsigned int getNumScreens();

    /** Returns the primary screen's physical size in mm.
    */
    static void getPhysicalSize(
        unsigned int & width
    ,   unsigned int & height);

    /** Returns the primary screen's maximum resolution in px. 
    */
    static void getMaximumResolution(
        unsigned int & width
    ,   unsigned int & height);

    /** Returns the primary screen's current resolution in px. 
    */
    static void getDesktopResolution(
        unsigned int & width
    ,   unsigned int & height);

    /** Returns the primary screen's pixel density in ppi. 
    */
    static void getPixelDensity(
        float & ppiHorizontal
    ,   float & ppiVertical);


    /** Returns all unique valid resolutions sorted in px. 
    */
    typedef std::multimap<unsigned int, unsigned int> Resolutions;
    static void getValidResolutions(Resolutions & resolutions);

private:
    Screen();
    ~Screen();
};

} // namespace glowutils
