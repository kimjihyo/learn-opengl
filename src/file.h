#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int file_read(const char *path, void **data, size_t *n_bytes);
