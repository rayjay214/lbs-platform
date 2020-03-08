/* rdwrn.h

   Header file for rdwrn.c.
*/
#ifndef RDWRN_H
#define RDWRN_H

#include <sys/types.h>

ssize_t readn(int fd, void *buf, size_t len);

ssize_t preadn(int fd, void *buffer, size_t n, off_t offset);

ssize_t writen(int fd, const void *buf, size_t len);

ssize_t pwriten(int fd, const void *buffer, size_t n, off_t offset);

#endif
