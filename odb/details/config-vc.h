/* file      : odb/details/config-vc.h
 * copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
 * license   : GNU GPL v2; see accompanying LICENSE file
 */

/* Configuration file for Windows/VC++. */

#ifndef ODB_DETAILS_CONFIG_VC_H
#define ODB_DETAILS_CONFIG_VC_H

#define ODB_THREADS_WIN32
#define ODB_THREADS_TLS_DECLSPEC

/* VC++10 has C++11 always enabled.
 */
#if _MSC_VER >= 1600
#  define ODB_CXX11
#endif

#endif /* ODB_DETAILS_CONFIG_VC_H */
