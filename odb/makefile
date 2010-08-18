# file      : odb/makefile
# author    : Boris Kolpackov <boris@codesynthesis.com>
# copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
# license   : GNU GPL v2; see accompanying LICENSE file

include $(dir $(lastword $(MAKEFILE_LIST)))../build/bootstrap.make

cxx_tun   :=        \
shared-ptr/base.cxx \
exception.cxx       \
exceptions.cxx      \
database.cxx        \
transaction.cxx

cxx_tun   += details/buffer.cxx

# POSIX-based implementation details.
#
cxx_tun   += details/posix/exceptions.cxx details/posix/thread.cxx

cxx_obj   := $(addprefix $(out_base)/,$(cxx_tun:.cxx=.o))
cxx_od    := $(cxx_obj:.o=.o.d)

odb.l             := $(out_base)/odb.l
odb.l.cpp-options := $(out_base)/odb.l.cpp-options

default   := $(out_base)/
install   := $(out_base)/.install
clean     := $(out_base)/.clean


# Build.
#
$(odb.l): $(cxx_obj) -lpthread

$(cxx_obj) $(cxx_od): $(odb.l.cpp-options)
$(odb.l.cpp-options): value := -I$(src_root)

$(call include-dep,$(cxx_od))


# Convenience alias for default target.
#
$(out_base)/: $(odb.l)

# Install.
#
$(install): $(odb.l)
	$(call install-lib,$<,$(install_lib_dir)/$(ld_lib_prefix)odb$(ld_lib_suffix))
	$(call install-dir,$(src_base),$(install_inc_dir)/odb,\
'(' -name '*.hxx' -o -name '*.ixx' -o -name '*.txx' ')')

# Clean.
#
$(clean): $(odb.l).o.clean           \
  $(odb.l.cpp-options).clean         \
  $(addsuffix .cxx.clean,$(cxx_obj)) \
  $(addsuffix .cxx.clean,$(cxx_od))


# How to.
#
$(call include,$(bld_root)/cxx/o-l.make)
$(call include,$(bld_root)/cxx/cxx-o.make)
$(call include,$(bld_root)/cxx/cxx-d.make)
