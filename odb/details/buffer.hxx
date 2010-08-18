// file      : odb/details/buffer.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_BUFFER_DETAILS_HXX
#define ODB_BUFFER_DETAILS_HXX

#include <new>
#include <cstddef> // std::size_t

namespace odb
{
  namespace details
  {
    class buffer
    {
    public:
      ~buffer ()
      {
        if (data_)
          operator delete (data_);
      }

      buffer ()
          : capacity_ (512)
      {
        data_ = static_cast<char*> (operator new (capacity_));
      }

      char*
      data ()
      {
        return data_;
      }

      const char*
      data () const
      {
        return data_;
      }

      std::size_t
      capacity () const
      {
        return capacity_;
      }

      void
      capacity (std::size_t, std::size_t data_size = 0);

    private:
      char* data_;
      std::size_t capacity_;
    };
  }
}

#endif // ODB_BUFFER_DETAILS_HXX
