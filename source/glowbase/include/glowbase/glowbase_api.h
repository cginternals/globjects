#pragma once

// NOTE: don't export stl stuff (e.g. containers):
// http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
// don't do it: http://support.microsoft.com/kb/q168958/

#ifdef _MSC_VER
#	define GLOWBASE_API_EXPORT_DECLARATION __declspec(dllexport)
#	define GLOWBASE_API_IMPORT_DECLARATION __declspec(dllimport)
#elif __GNUC__
#	define GLOWBASE_API_EXPORT_DECLARATION __attribute__ ((visibility ("default")))
#	define GLOWBASE_API_IMPORT_DECLARATION __attribute__ ((visibility ("default")))
#else
#	define GLOWBASE_API_EXPORT_DECLARATION
#	define GLOWBASE_API_IMPORT_DECLARATION
#endif

#ifndef GLOWBASE_STATIC
#ifdef GLOWBASE_EXPORTS
#	define GLOWBASE_API GLOWBASE_API_EXPORT_DECLARATION
#else
#	define GLOWBASE_API GLOWBASE_API_IMPORT_DECLARATION
#endif
#else
#   define GLOWBASE_API
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
