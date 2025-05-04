// ***************************************************************************
// static_assert - compile-time assertion checking
// ***************************************************************************

#pragma once

// ---------------------------------------------------------------------------
// static_assert(expr);
// ---------------------------------------------------------------------------

#ifndef NSTATICDEBUG
#define static_assert_concat_impl(x,y) x##y
#define static_assert_concat(x,y) static_assert_concat_impl(x,y)
#define static_assert_unique_name static_assert_concat(static_assert_, __COUNTER__)
//#define static_assert(expr) extern const struct { char sizeof_check[1 - 2*!!(expr)]; } UNIQUE_NAME __attribute__((unused))
#define static_assert(expr) ; enum { static_assert_unique_name = 1/((int)(!!(expr))) }
#else
#define static_assert(expr)
#endif

// ***************************************************************************
// end of file
// ***************************************************************************
