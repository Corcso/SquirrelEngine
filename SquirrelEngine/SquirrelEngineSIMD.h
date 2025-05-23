#pragma once

// Taken from DirectXMath - Seems to be required for JOLT in PCH, not sure why
#if (defined(_M_IX86) || defined(_M_X64) || __i386__ || __x86_64__) && !defined(_M_HYBRID_X86_ARM64) && !defined(_M_ARM64EC) && defined(JOLT)
#include <xmmintrin.h>
#include <emmintrin.h>
#endif