#ifndef __UTIL_FILE_H__
#define __UTIL_FILE_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int file_read(const char *path, bool is_str, void **data, size_t *n_bytes);

int file_read_str(const char *path, void **data, size_t *n_bytes);

#ifdef UTIL_IMPL
int file_read(const char *path, bool is_str, void **data, size_t *n_bytes) {
  FILE *fp = fopen(path, "r");
  if (!fp)
    return -2;
  fseek(fp, 0, SEEK_END);
  *n_bytes = ftell(fp);
  rewind(fp);
  *data = malloc(*n_bytes + (is_str ? 1 : 0));
  if (fread(*data, *n_bytes, 1, fp) != 1) {
    fclose(fp);
    free(*data);
    return -1;
  }
  if (is_str) {
    char **_data = (char **)data;
    (*_data)[*n_bytes - 1] = '\0';
  }
  fclose(fp);
  return 0;
}

int file_read_str(const char *path, void **data, size_t *n_bytes) {
  return file_read(path, true, data, n_bytes);
}
#endif // UTIL_IMPL
#endif // __UITL_FILE_H__
