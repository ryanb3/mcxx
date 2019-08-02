#
# SYNOPSIS
#
#   AX_CHECK_BGQ
#
# DESCRIPTION
#
#   Check that required header files for BlueGene/Q drivers are present.
#   Also finds whether -dynamic or -qnostaticbuild flags are necesary
#   to allow dynamic linkage.
#   Sets variables host_dep_CPPFLAGS and host_dep_LDFLAGS.
#   More information can be found at 
#   http://www-01.ibm.com/support/knowledgecenter/SSAT4T_15.1.0/com.ibm.xlf151.linux.doc/compiler_ref/opt_staticlink.html?lang=en
#
# LICENSE
#
#   Copyright (c) 2015 Jorge Bellon <jbellon@bsc.es>
#
#   This program is free software: you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation, either version 3 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <http://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.

AC_DEFUN([AX_CHECK_BGQ],
[
# This macro should be executed after checking for compiler
AC_BEFORE([AC_PROG_CXX],[$0])
AC_BEFORE([AX_COMPILER_VENDOR],[$0])

AC_LANG_PUSH([C++])

# Old BG/Q cppflags: -I/bgsys/drivers/V1R2M1/ppc64 -I/bgsys/drivers/V1R2M1/ppc64/spi/include/kernel/cnk
# The latter one might not be needed as all the header files included inside Nanox belong to a different
# directory
# Includes used:
#ifdef IS_BGQ_MACHINE
#include <spi/include/kernel/location.h>
#include <spi/include/kernel/process.h>
#endif

AX_APPEND_FLAG([-I/bgsys/drivers/ppcfloor/ppc64],[CPPFLAGS])

AC_CHECK_HEADERS(
  [spi/include/kernel/location.h spi/include/kernel/process.h],   
  [],
  [
    AC_MSG_ERROR([
----------------------------
Could not find the following BlueGene/Q driver headers:
/bgsys/drivers/ppcfloor/ppc64/spi/include/kernel/location.h
/bgsys/drivers/ppcfloor/ppc64/spi/include/kernel/process.h
----------------------------])
  ])

AS_CASE([$ax_cv_cxx_compiler_vendor],
  [ibm],[dyn_link_flag=-qnostaticlink],
  [gnu],[dyn_link_flag=-dynamic],
  [
    # Default
    AS_IF([test "$ac_cv_cxx_compiler_gnu" = yes],
      [dyn_link_flag=-dynamic]
      [dyn_link=no])
  ])

AS_IF([ test $dyn_link != no ],[
  AX_CHECK_COMPILE_FLAG([$dyn_link_flag],
    [
      AX_APPEND_FLAG([$dyn_link_flag],[CXXFLAGS])
    ],
    [dyn_link=no],[-Werror])
])

AS_IF([ test $dyn_link != no ],[
    AC_MSG_ERROR([
------------------------------
This compiler does not support dynamic link in BlueGene/Q.
Failed to link using either -dynamic or -qnostaticlink flags.
------------------------------])
])

AC_LANG_POP([C++])

AC_DEFINE([IS_BGQ_MACHINE],[1],[BlueGene/Q host compatibility is enabled.])

])dnl AX_CHECK_BGQ
