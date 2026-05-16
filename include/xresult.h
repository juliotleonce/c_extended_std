/**
 * @file xresult.h
 * @brief Error handling utilities inspired by Rust's Result type.
 */

#ifndef EXTENDED_STD_XRESULT_H
#define EXTENDED_STD_XRESULT_H
#include <stdio.h>

/**
 * @brief Error struct for details
 */
typedef struct XError {
    int code;
    char *message;
} XError;

/**
 * @brief Helper macro to refer to an XResult type of T.
 * @param T The underlying type.
 */
#define XRESULT_INNER(T) const XResult_of_##T

/**
 * @brief Macro to refer to an XResult of type T.
 */
#define XResult(T) XRESULT_INNER(T)

/**
 * @brief Helper macro to refer to an XPtr type of T.
 */
#define XPtr(T) T##_ptr

/**
 * @brief Internal macro to define a Result type for a given type T.
 * @param T The underlying type.
 */
#define DEFINE_XRESULT_INNER(T) typedef struct { \
    bool success;\
    union { \
        T data;\
        XError error; \
    } result; \
} XResult_of_##T;

/**
 * @brief Defines an XResult type for type T.
 * @note This macro uses typedef. Use it once per file or in a shared header to avoid redefinition.
 */
#define DEFINE_XRESULT_OF(T) DEFINE_XRESULT_INNER(T)

/**
 * @brief Defines a Pointer type for a given type T.
 * @param T The underlying type.
 */
#define DEFINE_XPTR_OF(T) typedef T* T##_ptr;

/**
 * @brief Creates an OK result.
 * @param T The underlying type.
 * @param val The value to wrap.
 */
#define OK_INNER(T, val) ((XResult_of_##T){ .success = true, .result.data = (val) })
#define OK(T, val) OK_INNER(T, val)

/**
 * @brief Creates an Error result.
 * @param T The underlying type.
 * @param err_code The error code.
 * @param err_message The error details.
 */
#define ERR_INNER(T, err_code, err_message) ((XResult_of_##T){ \
    .success = false, \
    .result.error = (XError){ .code = (err_code), .message = (err_message)} \
})
#define ERR(T, err_code, err_message) ERR_INNER(T, err_code, err_message)

/**
 * @brief Checks if a result is OK.
 * @param xresult The result to check.
 */
#define IS_OK(xresult) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    _tmp.success == true; \
})

/**
 * @brief Checks if a result is an Error.
 * @param xresult The result to check.
 */
#define IS_ERR(xresult) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    _tmp.success == false; \
})

/**
 * @brief Unwraps the value from a result, panics if it's an error.
 * @param xresult The result to unwrap.
 */
#define UNWRAP(xresult) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    if (_tmp.success == false) { \
        fprintf(stderr, "Panic: Unwrapped an error value %d\n", _tmp.result.error.code); \
        fprintf(stderr, "Details: %s", _tmp.result.error.message); \
        exit(_tmp.result.error.code); \
    } \
    _tmp.result.data; \
})

/**
 * @brief Unwraps the value or returns a default value if it's an error.
 * @param xresult The result to unwrap.
 * @param default_value The value to return if it's an error.
 */
#define UNWRAP_OR(xresult, default_value) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    (_tmp.success == false) ? (default_value) : _tmp.result.data; \
})

/**
 * @brief Propagates the error if the result is an error.
 * @param xresult The result to check.
 * @param T The return type of the current function (must be an XResult type).
 */
#define PROPAGATE(xresult, T) do { \
    __typeof__(xresult) _tmp = (xresult); \
    if (_tmp.success == false) \
        return ERR(T, _tmp.result.error.code, _tmp.result.error.message); \
} while (0)

/**
 * @brief Unwraps the value, panics with a custom message if it's an error.
 * @param xresult The result to unwrap.
 * @param msg The message to print on panic.
 */
#define EXPECT(xresult, msg) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    if (_tmp.success == false) { \
        fprintf(stderr, "Fatal Error: %s (Code: %d)\n", msg, _tmp.result.error.code); \
        fprintf(stderr, "Details: %s", _tmp.result.error.message); \
        exit(_tmp.result.error.code); \
    } \
    _tmp.result.data; \
})

/**
 * @brief Tries to unwrap the value or propagates the error.
 * @param xresult The result to unwrap.
 * @param T The underlying type of the result being returned.
 */
#define TRY(xresult, T) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    if (_tmp.success == false) \
        return ERR(T, _tmp.result.error.code, _tmp.result.error.message); \
    _tmp.result.data; \
})


#endif