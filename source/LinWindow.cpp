
#ifdef __linux__

#include <cassert>
#include <string>

#include <glow/logging.h>

#include "LinWindow.h"


namespace glow
{

std::set<LinWindow*> LinWindow::s_windows;



} // namespace glow

#endif
