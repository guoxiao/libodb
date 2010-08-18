// file      : odb/details/meta/remove-cv.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_META_REMOVE_CV_HXX
#define ODB_DETAILS_META_REMOVE_CV_HXX

#include <odb/details/meta/remove-c.hxx>
#include <odb/details/meta/remove-v.hxx>

namespace odb
{
  namespace details
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
}

#endif // ODB_DETAILS_META_REMOVE_CV_HXX
