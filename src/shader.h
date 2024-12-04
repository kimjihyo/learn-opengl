#ifndef __UTIL_SHADER_H__
#define __UTIL_SHADER_H__

unsigned int compile_shader(unsigned int type, const char *src);

unsigned int create_shader(const char *vs_str, const char *fs_str);

void parse_shader(const char *path, char *vs_dest, char *fs_dest);

#ifdef UTIL_IMPL
#include "file.h"
#include "str.h"
#include <GLES3/gl3.h>

void parse_shader(const char *path, char *vertex_shader,
                  char *fragment_shader) {
  char *file_buffer;
  size_t n_bytes;
  int res = file_read_str(path, (void *)&file_buffer, &n_bytes);
  if (res != 0) {
    printf("error loading shader file\n");
    return;
  }
  const char *ptr = file_buffer;
  char line[128];
  int shader_type = 0;

  while (*ptr != '\0') {
    ptr = strline(ptr, line, 128, NULL);
    if (strstr(line, "#shader")) {
      if (strstr(line, "vertex")) {
        shader_type = 1;
      } else if (strstr(line, "fragment")) {
        shader_type = 2;
      }
    } else if (shader_type == 1) {
      strcat(vertex_shader, line);
      strcat(vertex_shader, "\n");
    } else if (shader_type == 2) {
      strcat(fragment_shader, line);
      strcat(fragment_shader, "\n");
    }
  }
  free(file_buffer);
}

unsigned int compile_shader(unsigned int type, const char *source) {
  unsigned int id = glCreateShader(type);
  glShaderSource(id, 1, &source, NULL);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    const char *shader_type =
        type == GL_VERTEX_SHADER ? "vertex_shader" : "fragment_shader";
    printf("error compiling %s %s", shader_type, message);
    glDeleteShader(id);
    return 0;
  }
  return id;
}

unsigned int create_shader(const char *vertex_shader,
                           const char *fragment_shader) {
  unsigned int program = glCreateProgram();
  unsigned int vs      = compile_shader(GL_VERTEX_SHADER, vertex_shader);
  unsigned int fs      = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);
  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}
#endif // UTIL_IMPL
#endif // __UTIL_SHADER_H__
