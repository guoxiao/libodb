// file      : odb/meta/remove-cv.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_META_REMOVE_CV_HXX
#define ODB_META_REMOVE_CV_HXX

#include <odb/meta/remove-c.hxx>
#include <odb/meta/remove-v.hxx>

namespace odb
{
  namespace meta
  {
    template <typename X>
    struct remove_cv
    {
      typedef typename remove_v<typename remove_c<X>::r>::r r;
    };
  }
}

#endif // ODB_META_REMOVE_CV_HXX
