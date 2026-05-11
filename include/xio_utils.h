/**
 * @file xio_utils.h
 * @brief I/O utility functions.
 */

#ifndef EXTENDED_STD_XIO_UTILS_H
#define EXTENDED_STD_XIO_UTILS_H
#include "xstring.h"

/**
 * @brief Reads a line of text from the console.
 * @param prompt The prompt to display to the user.
 * @return A pointer to a new XString containing the input line.
 */
XString *xio_read_console_line(const char *prompt);

#endif //EXTENDED_STD_XIO_UTILS_H