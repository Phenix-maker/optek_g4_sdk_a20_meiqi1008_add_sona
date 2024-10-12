/*
 * pc_simulasion.c
 *
 *  Created on: 2024��4��7��
 *      Author: Windows
 */
#include <stdio.h>


// Dummy function for read system call.
// Always returns -1 to indicate an error.
int read(int fd, void *buf, int count) {
    return -1; // Return a negative value to indicate an error
}

// Dummy function for write system call.
// Always returns -1 to indicate an error.
int write(int fd, const void *buf, int count) {
    return -1; // Return a negative value to indicate an error
}

// Dummy function for open system call.
// Always returns -1 to indicate an error.
int open(const char *pathname, int flags, int mode) {
    return -1; // Return a negative value to indicate an error
}

// Dummy function for close system call.
// Always returns -1 to indicate an error.
int close(int fd) {
    return -1; // Return a negative value to indicate an error
}

// Dummy function for lseek system call.
// Always returns -1 to indicate an error.
int lseek(int fd, int offset, int whence) {
    return -1; // Return a negative value to indicate an error
}

// Dummy function for sbrk system call.
// Always returns 0 to indicate an error.
void *sbrk(int increment) {
    return (void *)0; // Return a negative value to indicate an error
}

// Dummy function for times system call.
// Always returns -1 to indicate an error.
int times(void *buf) {
    return -1; // Return a negative value to indicate an error
}

// Dummy function for unlink system call.
// Always returns -1 to indicate an error.
int unlink(void *pathname) {
    return -1; // Return a negative value to indicate an error
}
