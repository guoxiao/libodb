// file      : odb/details/buffer.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_BUFFER_DETAILS_HXX
#define ODB_BUFFER_DETAILS_HXX

#include <odb/pre.hxx>

#include <new>
#include <cstddef> // std::size_t

#include <odb/details/export.hxx>

namespace odb
{
  namespace details
  {
    class LIBODB_EXPORT basic_buffer_base
    {
    public:
      ~basic_buffer_base ()
      {
        if (data_)
          operator delete (data_);
      }

      basic_buffer_base ()
        : capacity_ (512)
      {
        data_ = operator new (capacity_);
      }

      std::size_t
      capacity () const
      {
        return capacity_;
      }

      void
      capacity (std::size_t, std::size_t data_size = 0);

    protected:
      void* data_;
      std::size_t capacity_;
    };

    template <typename T>
    class basic_buffer: public basic_buffer_base
    {
    public:

      T*
      data ()
      {
        return static_cast<T*> (data_);
      }

      const T*
      data () const
      {
        return static_cast<T*> (data_);
      }
    };

    typedef basic_buffer<char> buffer;
    typedef basic_buffer<unsigned char> ubuffer;
  }
}

#include <odb/post.hxx>

#endif // ODB_BUFFER_DETAILS_HXX
