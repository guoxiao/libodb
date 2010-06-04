// file      : odb/meta/answer.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_META_ANSWER_HXX
#define ODB_META_ANSWER_HXX

namespace odb
{
  namespace meta
  {
    struct yes
    {
      char filling;
    };

    struct no
    {
      char filling[2];
    };
  }
}

#endif // ODB_META_ANSWER_HXX
