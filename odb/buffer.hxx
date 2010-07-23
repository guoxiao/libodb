// file      : odb/buffer.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_BUFFER_HXX
#define ODB_BUFFER_HXX

#include <cstddef> // std::size_t

namespace odb
{
  class buffer
  {
  public:
    buffer ();

    void
    grow (std::size_t new_capacity, std::size_t data_size);

  private:
    char* data_;
    std::size_t capacity_;
  };
}

#endif // ODB_BUFFER_HXX
