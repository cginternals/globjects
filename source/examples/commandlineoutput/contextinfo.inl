
#include <iostream>


namespace common
{

    void printContextInfo()
    {
        std::cout << std::endl
            << "OpenGL Version:  " << glbinding::ContextInfo::version() << std::endl
            << "OpenGL Vendor:   " << glbinding::ContextInfo::vendor() << std::endl
            << "OpenGL Renderer: " << glbinding::ContextInfo::renderer() << std::endl << std::endl;
    }

}