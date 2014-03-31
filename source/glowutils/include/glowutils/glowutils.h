#pragma once

// NOTE: don't export stl stuff (e.g. containers):
// http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
// don't do it: http://support.microsoft.com/kb/q168958/

#ifdef _MSC_VER
#	define GLOWUTILS_API_EXPORT_DECLARATION __declspec(dllexport)
#	define GLOWUTILS_API_IMPORT_DECLARATION __declspec(dllimport)
#elif __GNUC__
#	define GLOWUTILS_API_EXPORT_DECLARATION __attribute__ ((visibility ("default")))
#	define GLOWUTILS_API_IMPORT_DECLARATION __attribute__ ((visibility ("default")))
#else
#	define GLOWUTILS_API_EXPORT_DECLARATION
#	define GLOWUTILS_API_IMPORT_DECLARATION
#endif

#ifndef GLOW_STATIC
#ifdef GLOWUTILS_EXPORTS
#	define GLOWUTILS_API GLOWUTILS_API_EXPORT_DECLARATION
#else
#	define GLOWUTILS_API GLOWUTILS_API_IMPORT_DECLARATION
#endif
#else
#	define GLOWUTILS_API
#endif


#ifdef N_DEBUG
#	define IF_DEBUG(statement)
#	define IF_NDEBUG(statement) statement
#else
#	define IF_DEBUG(statement) statement
#	define IF_NDEBUG(statement)
#endif // N_DEBUG

// http://stackoverflow.com/questions/18387640/how-to-deal-with-noexcept-in-visual-studio
#ifndef NOEXCEPT
#	ifdef _MSC_VER
#		define NOEXCEPT
#	else
#		define NOEXCEPT noexcept
#	endif
#endif
