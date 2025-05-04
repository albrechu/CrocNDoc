// ***************************************************************************
// monitor - runtime monitoring
// ***************************************************************************

#pragma once
#include <lib/debug.h>
#include <lib/print.h>

// ---------------------------------------------------------------------------
// monitor(text, var);
// ---------------------------------------------------------------------------

#ifndef NDEBUG
#define monitor(text, expr) \
	if(__builtin_types_compatible_p(typeof(expr), unsigned int)) \
	{ \
		unsigned int temp_expr = (unsigned int) ((long unsigned int) expr); \
        _f_debug(text "\x80", DEBUG_TYPE_UNSIGNED_INT, &temp_expr, __FILE__, sizeof(__FILE__), (unsigned long int) __LINE__); \
	} \
	else if(__builtin_types_compatible_p(typeof(expr), signed int)) \
	{ \
		signed int temp_expr = (signed int) ((long signed int) expr); \
        _f_debug(text "\x80", DEBUG_TYPE_SIGNED_INT, &temp_expr, __FILE__, sizeof(__FILE__), (unsigned long int) __LINE__); \
	} \
	else if(__builtin_types_compatible_p(typeof(expr), long unsigned int)) \
	{ \
		long unsigned int temp_expr = (long unsigned int) expr; \
        _f_debug(text "\x80", DEBUG_TYPE_LONG_UNSIGNED_INT, &temp_expr, __FILE__, sizeof(__FILE__), (unsigned long int) __LINE__); \
 	} \
	else if(__builtin_types_compatible_p(typeof(expr), long signed int)) \
	{ \
		long signed int temp_expr = (long signed int) expr; \
        _f_debug(text "\x80", DEBUG_TYPE_LONG_SIGNED_INT, &temp_expr, __FILE__, sizeof(__FILE__), (unsigned long int) __LINE__); \
	} \
	else if(__builtin_types_compatible_p(typeof(expr), char)) \
	{ \
		char temp_expr = (char) (long signed int) expr; \
        _f_debug(text "\x80", DEBUG_TYPE_CHAR, &temp_expr, __FILE__, sizeof(__FILE__), (unsigned long int) __LINE__); \
	} \
	else if(__builtin_types_compatible_p(typeof(expr), void*)) \
	{ \
		long unsigned int temp_expr = (long unsigned int) expr; \
		void* p_temp_expr = (void*) temp_expr; \
		_f_debug(text "\x80", DEBUG_TYPE_POINTER, &p_temp_expr, __FILE__, sizeof(__FILE__), (unsigned long int) __LINE__); \
	}
#else
#define monitor(text, expr)
#endif

#ifndef NDEBUG
#define monitor_binary(text, expr) \
	if(__builtin_types_compatible_p(typeof(expr), unsigned int)) \
	{ \
        _f_debug(text "\x80", DEBUG_TYPE_BINARY, &expr, __FILE__, sizeof(__FILE__), (unsigned long int) __LINE__); \
	} \
	else if(__builtin_types_compatible_p(typeof(expr), signed int)) \
	{ \
        _f_debug(text "\x80", DEBUG_TYPE_BINARY, &expr, __FILE__, sizeof(__FILE__), (unsigned long int) __LINE__); \
	} \
	else if(__builtin_types_compatible_p(typeof(expr), long unsigned int)) \
	{ \
        _f_debug(text "\x80", DEBUG_TYPE_LONG_BINARY, &expr, __FILE__, sizeof(__FILE__), (unsigned long int) __LINE__); \
 	} \
	else if(__builtin_types_compatible_p(typeof(expr), long signed int)) \
	{ \
        _f_debug(text "\x80", DEBUG_TYPE_LONG_BINARY, &expr, __FILE__, sizeof(__FILE__), (unsigned long int) __LINE__); \
	} \
	else if(__builtin_types_compatible_p(typeof(expr), char)) \
	{ \
        _f_debug(text "\x80", DEBUG_TYPE_BINARY, &expr, __FILE__, sizeof(__FILE__), (unsigned long int) __LINE__); \
	} \
	else if(__builtin_types_compatible_p(typeof(expr), void*)) \
	{ \
        _f_debug(text "\x80", DEBUG_TYPE_LONG_BINARY, &expr, __FILE__, sizeof(__FILE__), (unsigned long int) __LINE__); \
	}
#else
#define monitor(text, expr)
#endif

// ***************************************************************************
// end of file
// ***************************************************************************
