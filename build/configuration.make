# file      : build/configuration.make
# author    : Boris Kolpackov <boris@codesynthesis.com>
# copyright : Copyright (c) 2005-2011 Code Synthesis Tools CC
# license   : GNU GPL v2; see accompanying LICENSE file

$(call include-once,$(scf_root)/configuration-rules.make,$(dcf_root))

# Dynamic configuration.
#
libodb_threads :=

$(call -include,$(dcf_root)/configuration-dynamic.make)

ifdef libodb_threads

$(out_root)/%: libodb_threads := $(libodb_threads)

else

.NOTPARALLEL:

endif
