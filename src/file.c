#include "file.h"

int file_read(const char *path, void **data, size_t *n_bytes) {
  FILE *fp = fopen(path, "r");
  if (!fp)
    return -2;
  fseek(fp, 0, SEEK_END);
  *n_bytes = ftell(fp);
  rewind(fp);
  *data = malloc(*n_bytes);
  if (fread(*data, *n_bytes, 1, fp) != 1) {
    fclose(fp);
    free(*data);
    return -1;
  }
  fclose(fp);
  return 0;
}
