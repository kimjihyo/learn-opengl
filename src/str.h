#pragma once

#include <stddef.h>
#include <stdio.h>
#include <string.h>

const char *strline(const char *str, char *line, size_t n_bytes,
                    size_t *line_len);
