#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
 
// These vars will contain the hash
extern uint32_t h0, h1, h2, h3;
 
void md5(uint8_t *initial_msg, size_t initial_len);
 
char *md5sum(uint8_t *msg);

