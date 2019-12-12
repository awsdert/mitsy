#ifndef INC_MCC_SYS_H
#define INC_MCC_SYS_H

#if defined( __sun__ )
#define MCC__SYS_sun
#elif defined( __sun )
#define MCC__SYS_sun
#elif defined( _sun_ )
#define MCC__SYS_sun
#elif defined( _sun_ )
#define MCC__SYS_sun
#elif defined( sun )
#define MCC__SYS_sun
#endif

#if defined( __SUN__ )
#define MCC__SYS_SUN
#elif defined( __SUN )
#define MCC__SYS_SUN
#elif defined( _SUN_ )
#define MCC__SYS_SUN
#elif defined( _SUN_ )
#define MCC__SYS_SUN
#elif defined( SUN )
#define MCC__SYS_SUN
#endif

#if defined( MCC__SYS_sun ) || defined( MCC__SYS_SUN )
#include <sys/isa_defs.h>
#endif

#if defined( __32bit__ )
#define MCC__SYS_32bit
#elif defined( __32bit )
#define MCC__SYS_32bit
#elif defined( _32bit_ )
#define MCC__SYS_32bit
#elif defined( _32bit_ )
#define MCC__SYS_32bit
#endif

#if defined( __32BIT__ )
#define MCC__SYS_32BIT
#elif defined( __32BIT )
#define MCC__SYS_32BIT
#elif defined( _32BIT_ )
#define MCC__SYS_32BIT
#elif defined( _32BIT_ )
#define MCC__SYS_32BIT
#endif

#if defined( __64bit__ )
#define MCC__SYS_64bit
#elif defined( __64bit )
#define MCC__SYS_64bit
#elif defined( _64bit_ )
#define MCC__SYS_64bit
#elif defined( _64bit_ )
#define MCC__SYS_64bit
#endif

#if defined( __64BIT__ )
#define MCC__SYS_64BIT
#elif defined( __64BIT )
#define MCC__SYS_64BIT
#elif defined( _64BIT_ )
#define MCC__SYS_64BIT
#elif defined( _64BIT_ )
#define MCC__SYS_64BIT
#endif

#if defined( __aix__ )
#define MCC__SYS_aix
#elif defined( __aix )
#define MCC__SYS_aix
#elif defined( _aix_ )
#define MCC__SYS_aix
#elif defined( _aix_ )
#define MCC__SYS_aix
#elif defined( aix )
#define MCC__SYS_aix
#endif

#if defined( __AIX__ )
#define MCC__SYS_AIX
#elif defined( __AIX )
#define MCC__SYS_AIX
#elif defined( _AIX_ )
#define MCC__SYS_AIX
#elif defined( _AIX_ )
#define MCC__SYS_AIX
#elif defined( AIX )
#define MCC__SYS_AIX
#endif

#if defined( __amd__ )
#define MCC__SYS_amd
#elif defined( __amd )
#define MCC__SYS_amd
#elif defined( _amd_ )
#define MCC__SYS_amd
#elif defined( _amd_ )
#define MCC__SYS_amd
#elif defined( amd )
#define MCC__SYS_amd
#endif

#if defined( __AMD__ )
#define MCC__SYS_AMD
#elif defined( __AMD )
#define MCC__SYS_AMD
#elif defined( _AMD_ )
#define MCC__SYS_AMD
#elif defined( _AMD_ )
#define MCC__SYS_AMD
#elif defined( AMD )
#define MCC__SYS_AMD
#endif

#if defined( __apple__ )
#define MCC__SYS_apple
#elif defined( __apple )
#define MCC__SYS_apple
#elif defined( _apple_ )
#define MCC__SYS_apple
#elif defined( _apple_ )
#define MCC__SYS_apple
#elif defined( apple )
#define MCC__SYS_apple
#endif

#if defined( __APPLE__ )
#define MCC__SYS_APPLE
#elif defined( __APPLE )
#define MCC__SYS_APPLE
#elif defined( _APPLE_ )
#define MCC__SYS_APPLE
#elif defined( _APPLE_ )
#define MCC__SYS_APPLE
#elif defined( APPLE )
#define MCC__SYS_APPLE
#endif

#if defined( __Apple__ )
#define MCC__SYS_Apple
#elif defined( __Apple )
#define MCC__SYS_Apple
#elif defined( _Apple_ )
#define MCC__SYS_Apple
#elif defined( _Apple_ )
#define MCC__SYS_Apple
#elif defined( Apple )
#define MCC__SYS_Apple
#endif

#if defined( __hpux__ )
#define MCC__SYS_hpux
#elif defined( __hpux )
#define MCC__SYS_hpux
#elif defined( _hpux_ )
#define MCC__SYS_hpux
#elif defined( _hpux_ )
#define MCC__SYS_hpux
#elif defined( hpux )
#define MCC__SYS_hpux
#endif

#if defined( __HPUX__ )
#define MCC__SYS_HPUX
#elif defined( __HPUX )
#define MCC__SYS_HPUX
#elif defined( _HPUX_ )
#define MCC__SYS_HPUX
#elif defined( _HPUX_ )
#define MCC__SYS_HPUX
#elif defined( HPUX )
#define MCC__SYS_HPUX
#endif

#if defined( __i386__ )
#define MCC__SYS_i386
#elif defined( __i386 )
#define MCC__SYS_i386
#elif defined( _i386_ )
#define MCC__SYS_i386
#elif defined( _i386_ )
#define MCC__SYS_i386
#elif defined( i386 )
#define MCC__SYS_i386
#endif

#if defined( __I386__ )
#define MCC__SYS_I386
#elif defined( __I386 )
#define MCC__SYS_I386
#elif defined( _I386_ )
#define MCC__SYS_I386
#elif defined( _I386_ )
#define MCC__SYS_I386
#elif defined( I386 )
#define MCC__SYS_I386
#endif

#if defined( __ia64__ )
#define MCC__SYS_ia64
#elif defined( __ia64 )
#define MCC__SYS_ia64
#elif defined( _ia64_ )
#define MCC__SYS_ia64
#elif defined( _ia64_ )
#define MCC__SYS_ia64
#elif defined( ia64 )
#define MCC__SYS_ia64
#endif

#if defined( __IA64__ )
#define MCC__SYS_IA64
#elif defined( __IA64 )
#define MCC__SYS_IA64
#elif defined( _IA64_ )
#define MCC__SYS_IA64
#elif defined( _IA64_ )
#define MCC__SYS_IA64
#elif defined( IA64 )
#define MCC__SYS_IA64
#endif

#if defined( __ilp32__ )
#define MCC__SYS_ilp32
#elif defined( __ilp32 )
#define MCC__SYS_ilp32
#elif defined( _ilp32_ )
#define MCC__SYS_ilp32
#elif defined( _ilp32_ )
#define MCC__SYS_ilp32
#elif defined( ilp32 )
#define MCC__SYS_ilp32
#endif

#if defined( __ILP32__ )
#define MCC__SYS_ILP32
#elif defined( __ILP32 )
#define MCC__SYS_ILP32
#elif defined( _ILP32_ )
#define MCC__SYS_ILP32
#elif defined( _ILP32_ )
#define MCC__SYS_ILP32
#elif defined( ILP32 )
#define MCC__SYS_ILP32
#endif

#if defined( __ilp64__ )
#define MCC__SYS_ilp64
#elif defined( __ilp64 )
#define MCC__SYS_ilp64
#elif defined( _ilp64_ )
#define MCC__SYS_ilp64
#elif defined( _ilp64_ )
#define MCC__SYS_ilp64
#elif defined( ilp64 )
#define MCC__SYS_ilp64
#endif

#if defined( __ILP64__ )
#define MCC__SYS_ILP64
#elif defined( __ILP64 )
#define MCC__SYS_ILP64
#elif defined( _ILP64_ )
#define MCC__SYS_ILP64
#elif defined( _ILP64_ )
#define MCC__SYS_ILP64
#elif defined( ILP64 )
#define MCC__SYS_ILP64
#endif

#if defined( __linux__ )
#define MCC__SYS_linux
#elif defined( __linux )
#define MCC__SYS_linux
#elif defined( _linux_ )
#define MCC__SYS_linux
#elif defined( _linux_ )
#define MCC__SYS_linux
#elif defined( linux )
#define MCC__SYS_linux
#endif

#if defined( __LINUX__ )
#define MCC__SYS_LINUX
#elif defined( __LINUX )
#define MCC__SYS_LINUX
#elif defined( _LINUX_ )
#define MCC__SYS_LINUX
#elif defined( _LINUX_ )
#define MCC__SYS_LINUX
#elif defined( LINUX )
#define MCC__SYS_LINUX
#endif

#if defined( __llp64__ )
#define MCC__SYS_llp64
#elif defined( __llp64 )
#define MCC__SYS_llp64
#elif defined( _llp64_ )
#define MCC__SYS_llp64
#elif defined( _llp64_ )
#define MCC__SYS_llp64
#elif defined( llp64 )
#define MCC__SYS_llp64
#endif

#if defined( __LLP64__ )
#define MCC__SYS_LLP64
#elif defined( __LLP64 )
#define MCC__SYS_LLP64
#elif defined( _LLP64_ )
#define MCC__SYS_LLP64
#elif defined( _LLP64_ )
#define MCC__SYS_LLP64
#elif defined( LLP64 )
#define MCC__SYS_LLP64
#endif

#if defined( __lp32__ )
#define MCC__SYS_lp32
#elif defined( __lp32 )
#define MCC__SYS_lp32
#elif defined( _lp32_ )
#define MCC__SYS_lp32
#elif defined( _lp32_ )
#define MCC__SYS_lp32
#elif defined( lp32 )
#define MCC__SYS_lp32
#endif

#if defined( __LP32__ )
#define MCC__SYS_LP32
#elif defined( __LP32 )
#define MCC__SYS_LP32
#elif defined( _LP32_ )
#define MCC__SYS_LP32
#elif defined( _LP32_ )
#define MCC__SYS_LP32
#elif defined( LP32 )
#define MCC__SYS_LP32
#endif

#if defined( __lp64__ )
#define MCC__SYS_lp64
#elif defined( __lp64 )
#define MCC__SYS_lp64
#elif defined( _lp64_ )
#define MCC__SYS_lp64
#elif defined( _lp64_ )
#define MCC__SYS_lp64
#elif defined( lp64 )
#define MCC__SYS_lp64
#endif

#if defined( __LP64__ )
#define MCC__SYS_LP64
#elif defined( __LP64 )
#define MCC__SYS_LP64
#elif defined( _LP64_ )
#define MCC__SYS_LP64
#elif defined( _LP64_ )
#define MCC__SYS_LP64
#elif defined( LP64 )
#define MCC__SYS_LP64
#endif

#if defined( __osf__ )
#define MCC__SYS_osf
#elif defined( __osf )
#define MCC__SYS_osf
#elif defined( _osf_ )
#define MCC__SYS_osf
#elif defined( _osf_ )
#define MCC__SYS_osf
#elif defined( osf )
#define MCC__SYS_osf
#endif

#if defined( __OSF__ )
#define MCC__SYS_OSF
#elif defined( __OSF )
#define MCC__SYS_OSF
#elif defined( _OSF_ )
#define MCC__SYS_OSF
#elif defined( _OSF_ )
#define MCC__SYS_OSF
#elif defined( OSF )
#define MCC__SYS_OSF
#endif

#if defined( __powerpc__ )
#define MCC__SYS_powerpc
#elif defined( __powerpc )
#define MCC__SYS_powerpc
#elif defined( _powerpc_ )
#define MCC__SYS_powerpc
#elif defined( _powerpc_ )
#define MCC__SYS_powerpc
#elif defined( powerpc )
#define MCC__SYS_powerpc
#endif

#if defined( __POWERPC__ )
#define MCC__SYS_POWERPC
#elif defined( __POWERPC )
#define MCC__SYS_POWERPC
#elif defined( _POWERPC_ )
#define MCC__SYS_POWERPC
#elif defined( _POWERPC_ )
#define MCC__SYS_POWERPC
#elif defined( POWERPC )
#define MCC__SYS_POWERPC
#endif

#if defined( __PowerPC__ )
#define MCC__SYS_PowerPC
#elif defined( __PowerPC )
#define MCC__SYS_PowerPC
#elif defined( _PowerPC_ )
#define MCC__SYS_PowerPC
#elif defined( _PowerPC_ )
#define MCC__SYS_PowerPC
#elif defined( PowerPC )
#define MCC__SYS_PowerPC
#endif

#if defined( __powerpc64__ )
#define MCC__SYS_powerpc64
#elif defined( __powerpc64 )
#define MCC__SYS_powerpc64
#elif defined( _powerpc64_ )
#define MCC__SYS_powerpc64
#elif defined( _powerpc64_ )
#define MCC__SYS_powerpc64
#elif defined( powerpc64 )
#define MCC__SYS_powerpc64
#endif

#if defined( __POWERPC64__ )
#define MCC__SYS_POWERPC64
#elif defined( __POWERPC64 )
#define MCC__SYS_POWERPC64
#elif defined( _POWERPC64_ )
#define MCC__SYS_POWERPC64
#elif defined( _POWERPC64_ )
#define MCC__SYS_POWERPC64
#elif defined( POWERPC64 )
#define MCC__SYS_POWERPC64
#endif

#if defined( __PowerPC64__ )
#define MCC__SYS_PowerPC64
#elif defined( __PowerPC64 )
#define MCC__SYS_PowerPC64
#elif defined( _PowerPC64_ )
#define MCC__SYS_PowerPC64
#elif defined( _PowerPC64_ )
#define MCC__SYS_PowerPC64
#elif defined( PowerPC64 )
#define MCC__SYS_PowerPC64
#endif

#if defined( __s390__ )
#define MCC__SYS_s390
#elif defined( __s390 )
#define MCC__SYS_s390
#elif defined( _s390_ )
#define MCC__SYS_s390
#elif defined( _s390_ )
#define MCC__SYS_s390
#elif defined( s390 )
#define MCC__SYS_s390
#endif

#if defined( __S390__ )
#define MCC__SYS_S390
#elif defined( __S390 )
#define MCC__SYS_S390
#elif defined( _S390_ )
#define MCC__SYS_S390
#elif defined( _S390_ )
#define MCC__SYS_S390
#elif defined( S390 )
#define MCC__SYS_S390
#endif

#if defined( __s390x__ )
#define MCC__SYS_s390x
#elif defined( __s390x )
#define MCC__SYS_s390x
#elif defined( _s390x_ )
#define MCC__SYS_s390x
#elif defined( _s390x_ )
#define MCC__SYS_s390x
#elif defined( s390x )
#define MCC__SYS_s390x
#endif

#if defined( __S390X__ )
#define MCC__SYS_S390X
#elif defined( __S390X )
#define MCC__SYS_S390X
#elif defined( _S390X_ )
#define MCC__SYS_S390X
#elif defined( _S390X_ )
#define MCC__SYS_S390X
#elif defined( S390X )
#define MCC__SYS_S390X
#endif

#if defined( __silp32__ )
#define MCC__SYS_silp32
#elif defined( __silp32 )
#define MCC__SYS_silp32
#elif defined( _silp32_ )
#define MCC__SYS_silp32
#elif defined( _silp32_ )
#define MCC__SYS_silp32
#elif defined( silp32 )
#define MCC__SYS_silp32
#endif

#if defined( __SILP32__ )
#define MCC__SYS_SILP32
#elif defined( __SILP32 )
#define MCC__SYS_SILP32
#elif defined( _SILP32_ )
#define MCC__SYS_SILP32
#elif defined( _SILP32_ )
#define MCC__SYS_SILP32
#elif defined( SILP32 )
#define MCC__SYS_SILP32
#endif

#if defined( __silp64__ )
#define MCC__SYS_silp64
#elif defined( __silp64 )
#define MCC__SYS_silp64
#elif defined( _silp64_ )
#define MCC__SYS_silp64
#elif defined( _silp64_ )
#define MCC__SYS_silp64
#elif defined( silp64 )
#define MCC__SYS_silp64
#endif

#if defined( __SILP64__ )
#define MCC__SYS_SILP64
#elif defined( __SILP64 )
#define MCC__SYS_SILP64
#elif defined( _SILP64_ )
#define MCC__SYS_SILP64
#elif defined( _SILP64_ )
#define MCC__SYS_SILP64
#elif defined( SILP64 )
#define MCC__SYS_SILP64
#endif

#if defined( __sinix__ )
#define MCC__SYS_sinix
#elif defined( __sinix )
#define MCC__SYS_sinix
#elif defined( _sinix_ )
#define MCC__SYS_sinix
#elif defined( _sinix_ )
#define MCC__SYS_sinix
#elif defined( sinix )
#define MCC__SYS_sinix
#endif

#if defined( __SINIX__ )
#define MCC__SYS_SINIX
#elif defined( __SINIX )
#define MCC__SYS_SINIX
#elif defined( _SINIX_ )
#define MCC__SYS_SINIX
#elif defined( _SINIX_ )
#define MCC__SYS_SINIX
#elif defined( SINIX )
#define MCC__SYS_SINIX
#endif

#if defined( __unix__ )
#define MCC__SYS_unix
#elif defined( __unix )
#define MCC__SYS_unix
#elif defined( _unix_ )
#define MCC__SYS_unix
#elif defined( _unix_ )
#define MCC__SYS_unix
#elif defined( unix )
#define MCC__SYS_unix
#endif

#if defined( __UNIX__ )
#define MCC__SYS_UNIX
#elif defined( __UNIX )
#define MCC__SYS_UNIX
#elif defined( _UNIX_ )
#define MCC__SYS_UNIX
#elif defined( _UNIX_ )
#define MCC__SYS_UNIX
#elif defined( UNIX )
#define MCC__SYS_UNIX
#endif

#if defined( __x86__ )
#define MCC__SYS_x86
#elif defined( __x86 )
#define MCC__SYS_x86
#elif defined( _x86_ )
#define MCC__SYS_x86
#elif defined( _x86_ )
#define MCC__SYS_x86
#elif defined( x86 )
#define MCC__SYS_x86
#endif

#if defined( __X86__ )
#define MCC__SYS_X86
#elif defined( __X86 )
#define MCC__SYS_X86
#elif defined( _X86_ )
#define MCC__SYS_X86
#elif defined( _X86_ )
#define MCC__SYS_X86
#elif defined( X86 )
#define MCC__SYS_X86
#endif

#if defined( __x64__ )
#define MCC__SYS_x64
#elif defined( __x64 )
#define MCC__SYS_x64
#elif defined( _x64_ )
#define MCC__SYS_x64
#elif defined( _x64_ )
#define MCC__SYS_x64
#elif defined( x64 )
#define MCC__SYS_x64
#endif

#if defined( __X64__ )
#define MCC__SYS_X64
#elif defined( __X64 )
#define MCC__SYS_X64
#elif defined( _X64_ )
#define MCC__SYS_X64
#elif defined( _X64_ )
#define MCC__SYS_X64
#elif defined( X64 )
#define MCC__SYS_X64
#endif

#if defined( __x86_64__ )
#define MCC__SYS_x86_64
#elif defined( __x86_64 )
#define MCC__SYS_x86_64
#elif defined( _x86_64_ )
#define MCC__SYS_x86_64
#elif defined( _x86_64_ )
#define MCC__SYS_x86_64
#elif defined( x86_64 )
#define MCC__SYS_x86_64
#endif

#if defined( __X86_64__ )
#define MCC__SYS_X86_64
#elif defined( __X86_64 )
#define MCC__SYS_X86_64
#elif defined( _X86_64_ )
#define MCC__SYS_X86_64
#elif defined( _X86_64_ )
#define MCC__SYS_X86_64
#elif defined( X86_64 )
#define MCC__SYS_X86_64
#endif

#if defined( __win16__ )
#define MCC__SYS_win16
#elif defined( __win16 )
#define MCC__SYS_win16
#elif defined( _win16_ )
#define MCC__SYS_win16
#elif defined( _win16_ )
#define MCC__SYS_win16
#elif defined( win16 )
#define MCC__SYS_win16
#endif

#if defined( __WIN16__ )
#define MCC__SYS_WIN16
#elif defined( __WIN16 )
#define MCC__SYS_WIN16
#elif defined( _WIN16_ )
#define MCC__SYS_WIN16
#elif defined( _WIN16_ )
#define MCC__SYS_WIN16
#elif defined( WIN16 )
#define MCC__SYS_WIN16
#endif

#if defined( __Win16__ )
#define MCC__SYS_Win16
#elif defined( __Win16 )
#define MCC__SYS_Win16
#elif defined( _Win16_ )
#define MCC__SYS_Win16
#elif defined( _Win16_ )
#define MCC__SYS_Win16
#elif defined( Win16 )
#define MCC__SYS_Win16
#endif

#if defined( __win32__ )
#define MCC__SYS_win32
#elif defined( __win32 )
#define MCC__SYS_win32
#elif defined( _win32_ )
#define MCC__SYS_win32
#elif defined( _win32_ )
#define MCC__SYS_win32
#elif defined( win32 )
#define MCC__SYS_win32
#endif

#if defined( __WIN32__ )
#define MCC__SYS_WIN32
#elif defined( __WIN32 )
#define MCC__SYS_WIN32
#elif defined( _WIN32_ )
#define MCC__SYS_WIN32
#elif defined( _WIN32_ )
#define MCC__SYS_WIN32
#elif defined( WIN32 )
#define MCC__SYS_WIN32
#endif

#if defined( __Win32__ )
#define MCC__SYS_Win32
#elif defined( __Win32 )
#define MCC__SYS_Win32
#elif defined( _Win32_ )
#define MCC__SYS_Win32
#elif defined( _Win32_ )
#define MCC__SYS_Win32
#elif defined( Win32 )
#define MCC__SYS_Win32
#endif

#if defined( __win64__ )
#define MCC__SYS_win64
#elif defined( __win64 )
#define MCC__SYS_win64
#elif defined( _win64_ )
#define MCC__SYS_win64
#elif defined( _win64_ )
#define MCC__SYS_win64
#elif defined( win64 )
#define MCC__SYS_win64
#endif

#if defined( __WIN64__ )
#define MCC__SYS_WIN64
#elif defined( __WIN64 )
#define MCC__SYS_WIN64
#elif defined( _WIN64_ )
#define MCC__SYS_WIN64
#elif defined( _WIN64_ )
#define MCC__SYS_WIN64
#elif defined( WIN64 )
#define MCC__SYS_WIN64
#endif

#if defined( __Win64__ )
#define MCC__SYS_Win64
#elif defined( __Win64 )
#define MCC__SYS_Win64
#elif defined( _Win64_ )
#define MCC__SYS_Win64
#elif defined( _Win64_ )
#define MCC__SYS_Win64
#elif defined( Win64 )
#define MCC__SYS_Win64
#endif

#if defined( MCC__SYS_aix ) || defined( MCC__SYS_AIX )
#define MCC_SYS_AIX
#elif defined( MCC__SYS_apple ) || defined( MCC__SYS_APPLE ) \
	|| defined( MCC__SYS_Apple )
#define MCC_SYS_APPLE
#elif defined( MCC__SYS_win64 ) || defined( MCC__SYS_WIN64 ) \
	|| defined( MCC__SYS_Win64 )
#define MCC_SYS_WIN64
#elif defined( MCC__SYS_win32 ) || defined( MCC__SYS_WIN32 ) \
	|| defined( MCC__SYS_Win32 )
#define MCC_SYS_WIN32
#elif defined( MCC__SYS_win16 ) || defined( MCC__SYS_WIN16 ) \
	|| defined( MCC__SYS_Win16 )
#define MCC_SYS_WIN16
#elif defined( MCC__SYS_hpux ) || defined( MCC__SYS_HPUX )
#define MCC_SYS_HPUX
#elif defined( MCC__SYS_osf ) || defined( MCC__SYS_OSF )
#define MCC_SYS_OSF
#elif defined( MCC__SYS_unix ) || defined( MCC__SYS_UNIX )
#define MCC_SYS_UNIX
#elif defined( MCC__SYS_linux ) || defined( MCC__SYS_LINUX )
#define MCC_SYS_LINUX
#else
#define MCC_SYS_OTHER_OS
#endif

#if defined( MCC__SYS_amd ) || defined( MCC__SYS_AMD )
#define MCC_SYS_AMD
#elif defined( MCC__SYS_s390x ) || defined( MCC__SYS_S390X )
#define MCC_SYS_S390X
#elif defined( MCC__SYS_powerpc64 ) || defined( MCC__SYS_POWERPC64 ) \
	|| defined( MCC__SYS_PowerPC64 )
#define MCC_SYS_POWERPC64
#elif defined( MCC__SYS_ia64 ) || defined( MCC__SYS_IA64 )
#define MCC_SYS_IA64
#elif defined( MCC__SYS_s390 ) || defined( MCC__SYS_S390 )
#define MCC_SYS_S390
#elif defined( MCC__SYS_powerpc ) || defined( MCC__SYS_POWERPC ) \
	|| defined( MCC__SYS_PowerPC )
#define MCC_SYS_POWERPC
#elif defined( MCC__SYS_i386 ) || defined( MCC__SYS_I386 )
#define MCC_SYS_I386
#else
#define MCC_SYS_OTHER_CPU
#endif

#if defined( MCC__SYS_64bit ) || defined( MCC__SYS_64BIT )
#defined MCC_SYS_64BIT
#elif defined( MCC__SYS_32bit ) || defined( MCC__SYS_32BIT )
#defined MCC_SYS_32BIT
#endif

#if defined( MCC_SYS_AMD ) || defined( MCC_SYS_S390X ) \
	|| defined( MCC_SYS_POWERPC64 ) || defined( MCC_SYS_IA64 )
#define MCC_CPU_INT_WIDTH 64
#elif defined( MCC_SYS_S390 ) || defined( MCC_SYS_POWERPC ) \
	|| defined( MCC_SYS_I386 )
#define MCC_CPU_INT_WIDTH 32
#endif

#if defined( MCC__SYS_llp64 ) || defined( MCC__SYS_LLP64 )
#define MCC_SYS_LLP64
#elif defined( MCC__SYS_silp64 ) || defined( MCC__SYS_SILP64 )
#define MCC_SYS_SILP64
#elif defined( MCC__SYS_ilp64 ) || defined( MCC__SYS_ILP64 )
#define MCC_SYS_ILP64
#elif defined( MCC__SYS_lp64 ) || defined( MCC__SYS_LP64 )
#define MCC_SYS_LP64
#elif defined( MCC__SYS_silp32 ) || defined( MCC__SYS_SILP32 )
#define MCC_SYS_SILP32
#elif defined( MCC__SYS_ilp32 ) || defined( MCC__SYS_ILP32 )
#define MCC_SYS_ILP32
#elif defined( MCC__SYS_lp32 ) || defined( MCC__SYS_LP32 )
#define MCC_SYS_LP32
#elif defined( MCC_SYS_AIX ) && defined( MCC_SYS_64BIT )
#define MCC_SYS_LP64
#elif defined( MCC_SYS_OSF )
#define MCC_SYS_LP64
#elif defined( MCC_SYS_WIN64 )
#define MCC_SYS_LLP64
#elif defined( MCC_SYS_WIN16 )
#define MCC_SYS_LP32
#elif defined( MCC_SYS_WIN32 )
#define MCC_SYS_ILP32
#elif defined( MCC_SYS_LINUX ) && defined( MCC_CPU_INT_WIDTH )
#	if MCC_CPU_INT_WIDTH == 64
/*	TODO: detect ILP64 && LLP64 situations */
#	define MCC_SYS_LP64
#	elif MCC_CPU_INT_WIDTH == 32
#	define MCC_SYS_ILP32
#	endif
#endif

#endif
