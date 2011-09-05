// file      : odb/result.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_RESULT_HXX
#define ODB_RESULT_HXX

#include <odb/pre.hxx>

#include <odb/forward.hxx> // result
#include <odb/traits.hxx>

namespace odb
{
  template <typename T, class_kind kind = class_traits<T>::kind>
  class result;

  template <typename T, class_kind kind = class_traits<T>::kind>
  class result_iterator;

  template <typename T, class_kind kind>
  class result_impl;

  namespace core
  {
    using odb::result;
    using odb::result_iterator;
  }
}

#include <odb/post.hxx>

#endif // ODB_RESULT_HXX

// Include result specializations so that the user code only needs
// to include this header.
//

#include <odb/object-result.hxx>
#include <odb/view-result.hxx>
