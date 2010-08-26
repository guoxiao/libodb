// file      : odb/details/meta/polymorphic-p.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_DETAILS_META_POLYMORPHIC_HXX
#define ODB_DETAILS_META_POLYMORPHIC_HXX

#include <odb/pre.hxx>

#include <odb/details/meta/class-p.hxx>
#include <odb/details/meta/remove-cv.hxx>

namespace odb
{
  namespace details
  {
    namespace meta
    {
      template <typename CVX>
      struct polymorphic_p
      {
        typedef typename remove_cv<CVX>::r X;

        template <typename Y, bool C>
        struct impl
        {
          static const bool r = false;
        };

        template <typename Y>
        struct impl<Y, true>
        {
          struct t1: Y
          {
            t1 ();
          };

          struct t2: Y
          {
            t2 ();

            virtual
            ~t2 () throw ();
          };

          static const bool r = sizeof (t1) == sizeof (t2);
        };

        static const bool r = impl<X, class_p<X>::r>::r;
      };
    }
  }
}

#include <odb/post.hxx>

#endif // ODB_DETAILS_META_POLYMORPHIC_HXX
