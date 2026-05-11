#ifndef EXTENDED_STD_XRESULT_H
#define EXTENDED_STD_XRESULT_H
#include <stdio.h>

#define XResult(T) const XResult_##T
#define XPtr(T) const XPtr_##T

#define DEFINE_XRESULT(T) typedef struct { T value; int error; } XResult_##T;
#define DEFINE_XPTR(T) typedef T *XPtr_##T;

#define OK(T, val) ((XResult_##T){ .value = (val), .error = 0 })
#define ERR(T, err_code) ((XResult_##T){ .error = (err_code) })

#define IS_OK(xresult) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    _tmp.error == 0; \
})

#define IS_ERR(xresult) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    _tmp.error != 0; \
})

#define UNWRAP(xresult) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    if (_tmp.error != 0) { \
        fprintf(stderr, "Panic: Unwrapped an error value %d\n", _tmp.error); \
        exit(_tmp.error); \
    } \
    _tmp.value; \
})

#define UNWRAP_OR(xresult, default_value) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    (_tmp.error == 0) ? _tmp.value : (default_value); \
})

#define PROPAGATE(xresult, T) do { \
    __typeof__(xresult) _tmp = (xresult); \
    if (_tmp.error != 0) return ERR(T, _tmp.error); \
} while (0)

#define EXPECT(xresult, msg) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    if (_tmp.error != 0) { \
        fprintf(stderr, "Fatal Error: %s (Code: %d)\n", msg, _tmp.error); \
        exit(_tmp.error); \
    } \
    _tmp.value; \
})

#define TRY(xresult, T) ({ \
    __typeof__(xresult) _tmp = (xresult); \
    if (_tmp.error != 0) return ERR(T, _tmp.error); \
    _tmp.value; \
})


#endif