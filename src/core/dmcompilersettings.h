/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of VIBe2
 *
 * Copyright (C) 2011  Christian Urich

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */


#ifndef DMCOMPILERSETTINGS_H
#define DMCOMPILERSETTINGS_H
// @see http://gcc.gnu.org/wiki/Visibility
// @see http://people.redhat.com/drepper/dsohowto.pdf
// @see http://www.eventhelix.com/RealtimeMantra/HeaderFileIncludePatterns.htm
// Generic helper definitions for shared library support
#if defined(_WIN32) || defined(__CYGWIN__)
#    if defined(_MSC_VER)   // Windows && MS Visual C
#        if _MSC_VER < 1310    //Version < 7.1?
#            pragma message ("Compiling with a Visual C compiler version < 7.1 (2003) has not been tested!")
#        endif // Version > 7.1
#        define DM_HELPER_DLL_IMPORT __declspec(dllimport)
#        define DM_HELPER_DLL_EXPORT __declspec(dllexport)
#        define DM_HELPER_DLL_LOCAL
#    elif defined (__GNUC__)
#        define DM_HELPER_DLL_IMPORT __attribute__((dllimport))
#        define DM_HELPER_DLL_EXPORT __attribute__((dllexport))
#        define DM_HELPER_DLL_LOCAL
#    endif
#    define BOOST_ALL_NO_LIB  //disable the msvc automatic boost-lib selection in order to link against the static libs!
#else //defined(__linux__) || defined(linux) || defined(__linux)
#    if __GNUC__ >= 4    // TODO Makefile: add -fvisibility=hidden to compiler parameter in Linux version
#        define DM_HELPER_DLL_IMPORT __attribute__ ((visibility("default")))
#        define DM_HELPER_DLL_EXPORT __attribute__ ((visibility("default")))
#        define DM_HELPER_DLL_LOCAL  __attribute__ ((visibility("hidden")))
#    else
#        define DM_HELPER_DLL_IMPORT
#        define DM_HELPER_DLL_EXPORT
#        define DM_HELPER_DLL_LOCAL
#    endif
#endif

// Now we use the generic helper definitions above to define DM_API and DM_LOCAL.
// DM_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// DM_LOCAL is used for non-api symbols.

#ifndef DM_DLL
#define DM_DLL
#endif
// TODO Makefile: add DM_DLL and EXPORTS
#ifdef DM_DLL // defined if MYLIB is compiled as a DLL
#    ifdef DM_DLL_EXPORTS // defined if we are building the MYLIB DLL (instead of using it)
#        define DLL_PUBLIC DM_HELPER_DLL_EXPORT
#    else
#        define DLL_PUBLIC DM_HELPER_DLL_IMPORT
#    endif // DM_DLL_EXPORTS
#        define DLL_LOCAL DM_HELPER_DLL_LOCAL
#    else // DM_DLL is not defined: this means MYLIB is a static lib.
#        define DLL_PUBLIC
#        define DLL_LOCAL
#endif // DM_DLL



// disable some nasty MSVC Warnings
#ifdef _MSC_VER   // Windows && MS Visual C
//#        pragma warning( disable : 4996 )     // disable deprecation warnings
//#        pragma warning( disable : 4091 )    // disable typedef warning without variable declaration
#        pragma warning( disable : 4275 )    // non &#8211; DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#        pragma warning( disable : 4251 )    // like warning above but for templates (like std::string)
#endif
/*
#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif
*/

#endif //COMPILERSETTINGS_H
