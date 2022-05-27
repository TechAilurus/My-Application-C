// stdcpp.h - originally written and placed in the public domain by Wei Dai

/// \file stdcpp.h
/// \brief Common C++ header files

#ifndef CRYPTOPP_STDCPP_H
#define CRYPTOPP_STDCPP_H

#if _MSC_VER >= 1500
#define _DO_NOT_DECLARE_INTERLOCKED_INTRINSICS_IN_MEMORY
#include <intrin.h>
#endif

#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/string"
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/memory"
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/exception"
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/typeinfo"
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/algorithm"
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/functional"
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/utility"
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/vector"
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/limits"
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/deque"
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/list"
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/map"
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/new"

// http://connect.microsoft.com/VisualStudio/feedback/details/1600701/type-info-does-not-compile-with-has-exceptions-0
#if defined(_MSC_VER) && (_MSC_VER < 1900) && defined(_HAS_EXCEPTIONS) && (_HAS_EXCEPTIONS == 0)
namespace std {
  using ::type_info;
}
#endif

// workaround needed for IBM XLC and debug heaps on AIX
#if defined(_AIX) && (defined(__xlc__) || defined(__xlC__) || defined(__ibmxl__))
# if defined(__DEBUG_ALLOC__)
namespace std {
  using ::_debug_memset;
  using ::_debug_memcpy;
}
# endif
#endif

// make_unchecked_array_iterator
#if _MSC_VER >= 1600
#include <iterator>
#endif

#if defined(CRYPTOPP_CXX11_ATOMIC)
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/atomic"
#endif

#if defined(CRYPTOPP_CXX11_SYNCHRONIZATION)
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/mutex"
#endif

#if defined(CRYPTOPP_CXX11_RVALUES)
# include <utility>
#endif

#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/cstdlib"
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/cstring"
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/climits"
#include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/cmath"

// It is 2019 and VS2017/Win10 still can't compile a
// program that includes <cstddef> without making users
// do something special. "Epic fail" comes to mind.
// Also see https://github.com/weidai11/cryptopp/issues/781
#ifndef _MSC_VER
# include "../../../../../../../Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/cstddef"
#endif

// uintptr_t and ptrdiff_t
#if defined(__SUNPRO_CC)
# if (__SUNPRO_CC >= 0x5100)
#  include <stdint.h>
# endif
#elif defined(_MSC_VER)
# if (_MSC_VER >= 1700)
#  include <stdint.h>
# else
#  include <stddef.h>
# endif
#elif (__cplusplus < 201103L)
# include <stdint.h>
#endif

// workaround needed on Sun Studio 12u1 Sun C++ 5.10 SunOS_i386 128229-02 2009/09/21
#ifdef CRYPTOPP_INCLUDE_VECTOR_CC
# include <vector.cc>
#endif

// C++Builder's standard library (Dinkumware) do not have C's global log() function
// https://github.com/weidai11/cryptopp/issues/520
#ifdef __BORLANDC__
using std::log;
#endif

#endif  // CRYPTOPP_STDCPP_H
