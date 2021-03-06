dnl @synopsis AC_CXX_COMPILER_VENDOR(VENDOR-NAME, [NICKNAME])
dnl
dnl Set VENDOR-NAME to the lower-case name of the compiler vendor or
dnl `unknown' if the compiler's vendor is unknown. `compaq' means the
dnl CXX compiler as available on Tru64/OSF1/Digital Unix on Alpha
dnl machines. If NICKNAME is provided, set it to the compiler's usual
dnl name (eg. `g++', `cxx', `aCC', etc.).
dnl
dnl @category Cxx
dnl @author Ludovic Court?s <ludo@chbouib.org>
dnl @version 2004-09-07
dnl @license AllPermissive

AC_DEFUN([AC_CXX_COMPILER_VENDOR],
  [AC_REQUIRE([AC_PROG_CXX])
   AC_REQUIRE([AC_PROG_CXXCPP])
   AC_CACHE_CHECK([the C++ compiler vendor],
    [ac_cv_cxx_compiler_vendor],

    [AC_LANG_PUSH([C++])

     dnl GNU C++
     _AC_C_IFDEF([__GNUG__],
       [ac_cv_cxx_compiler_vendor=gnu],
       [_AC_C_IFDEF([__DECCXX],
	 [ac_cv_cxx_compiler_vendor=compaq],
	 [dnl HP's aCC
	  _AC_C_IFDEF([__HP_aCC],
	   [ac_cv_cxx_compiler_vendor=hp],
	   [dnl SGI CC
	    _AC_C_IFDEF([__sgi],
	     [ac_cv_cxx_compiler_vendor=sgi],
	     [dnl Note:  We are using the C compiler because VC++ doesn't
	      dnl recognize `.cc'(which is used by `configure') as a C++ file
	      dnl extension and requires `/TP' to be passed.
	      AC_LANG_PUSH([C])
	      _AC_C_IFDEF([_MSC_VER],
		[ac_cv_cxx_compiler_vendor=microsoft],
		[ac_cv_cxx_compiler_vendor=unknown])
	      AC_LANG_POP()])])])])

     AC_LANG_POP()])
   $1="$ac_cv_cxx_compiler_vendor"

   dnl The compiler nickname
   ifelse([$2], , [],
     [case "$ac_cv_cxx_compiler_vendor" in
	gnu)       $2=g++;;
	compaq)    $2=cxx;;
	hp)        $2=aCC;;
	sgi)       $2=CC;;
	microsoft) $2=cl;;
	*)         $2=unknown;;
      esac])])dnl
