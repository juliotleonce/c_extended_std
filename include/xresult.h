/**
 * @file xresult.h
 * @brief Error handling utilities inspired by Rust's Result type.
 */

#ifndef EXTENDED_STD_XRESULT_H
#define EXTENDED_STD_XRESULT_H
#include <stdio.h>

/**
 * @brief Helper macro to refer to an XResult type of T.
 */
#define XResult(T) const XResult_##T

/**
 * @brief Helper macro to refer to an XPtr type of T.
 */
#define XPtr(T) const XPtr_##T

/**
 * @brief Defines a Result type for a given type T.
 * @param T The underlying type.
 */
#define DEFINE_XRESULT(T) typedef struct { T value; int error; } XResult_##T;

/**
 * @brief Defines a Pointer type for a given type T.
 * @param T The underlying type.
 */
#define DEFINE_XPTR(T) typedef T *XPtr_##T;

/**
 * @brief Creates an OK result.
 * @param T The underlying type.
 * @param val The value to wrap.
 */
#define OK(T, val) ((XResult_##T){ .value = (val), .error = 0 })

/**
 * @brief Creates an Error result.
 * @param T The underlying type.
 * @param err_code The error code.
 */
#define ERR(T, err_code) ((XResult_##T){ .error = (err_code) })

/**
 * @brief Checks if a result is OK.
 * @param xresult The result to check.
 */
#define IS_OK(xresult) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    _tmp.error == 0; \
})

/**
 * @brief Checks if a result is an Error.
 * @param xresult The result to check.
 */
#define IS_ERR(xresult) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    _tmp.error != 0; \
})

/**
 * @brief Unwraps the value from a result, panics if it's an error.
 * @param xresult The result to unwrap.
 */
#define UNWRAP(xresult) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    if (_tmp.error != 0) { \
        fprintf(stderr, "Panic: Unwrapped an error value %d\n", _tmp.error); \
        exit(_tmp.error); \
    } \
    _tmp.value; \
})

/**
 * @brief Unwraps the value or returns a default value if it's an error.
 * @param xresult The result to unwrap.
 * @param default_value The value to return if error.
 */
#define UNWRAP_OR(xresult, default_value) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    (_tmp.error == 0) ? _tmp.value : (default_value); \
})

/**
 * @brief Propagates the error if the result is an error.
 * @param xresult The result to check.
 * @param T The return type of the current function (must be an XResult type).
 */
#define PROPAGATE(xresult, T) do { \
    __typeof__(xresult) _tmp = (xresult); \
    if (_tmp.error != 0) return ERR(T, _tmp.error); \
} while (0)

/**
 * @brief Unwraps the value, panics with a custom message if it's an error.
 * @param xresult The result to unwrap.
 * @param msg The message to print on panic.
 */
#define EXPECT(xresult, msg) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    if (_tmp.error != 0) { \
        fprintf(stderr, "Fatal Error: %s (Code: %d)\n", msg, _tmp.error); \
        exit(_tmp.error); \
    } \
    _tmp.value; \
})

/**
 * @brief Tries to unwrap the value or propagates the error.
 * @param xresult The result to unwrap.
 * @param T The underlying type of the result being returned.
 */
#define TRY(xresult, T) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    if (_tmp.error != 0) return ERR(T, _tmp.error); \
    _tmp.value; \
})

#endif