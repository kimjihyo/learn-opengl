#include "str.h"

const char *strline(const char *str, char *line, size_t n_bytes,
                    size_t *line_len) {
  const char *end = strchr(str, '\n');
  if (end) {
    if ((size_t)(end - str) > n_bytes) {
      return NULL;
    }
    memcpy(line, str, end - str);
    line[end - str] = '\0';
    if (line_len != NULL) {
      *line_len = end - str;
    }
    return end + 1;
  } else {
    size_t len = strlen(str);
    if (line_len != NULL) {
      *line_len = len;
    }
    if (len > n_bytes) {
      return NULL;
    }
    memcpy(line, str, len);
    line[len] = '\0';
    return &line[len];
  }
}
