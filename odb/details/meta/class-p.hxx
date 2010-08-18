// file      : odb/details/meta/class-p.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_META_CLASS_HXX
#define ODB_DETAILS_META_CLASS_HXX

#include <odb/details/meta/answer.hxx>

namespace odb
{
  namespace details
  {
    namespace meta
    {
      // g++ cannot have these inside class_p.
      //
      template <typename Y> no class_p_test (...);
      template <typename Y> yes class_p_test (void (Y::*) ());

      template <typename X>
      struct class_p
      {
        static bool const r = sizeof (class_p_test<X> (0)) == sizeof (yes);
      };
    }
  }
}

#endif // ODB_DETAILS_META_CLASS_HXX
