#include "file.h"
#include "str.h"
#include <GLES3/gl3.h>
#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 640

typedef struct {
  SDL_Window *window;
  SDL_GLContext *gl_ctx;

  unsigned int shader;
  unsigned int vertex_array;
  unsigned int vertex_buffer;
  unsigned int index_buffer;

} global_t;

global_t g;

static void parse_shader(const char *path, char *vertex_shader,
                         char *fragment_shader) {
  char *file_buffer;
  size_t n_bytes;
  if (file_read(path, (void *)&file_buffer, &n_bytes) != 0) {
    printf("error loading shader file\n");
    return;
  }
  const char *ptr = file_buffer;
  char line[128];
  int shader_type = 0;

  while (ptr != NULL && *ptr != '\0') {
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

static unsigned int compile_shader(unsigned int type, const char *source) {
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
    printf("error compiling %s: %s", shader_type, message);
    glDeleteShader(id);
    return 0;
  }
  return id;
}

static unsigned int create_shader(const char *vertex_shader,
                                  const char *fragment_shader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
  unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);
  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

static void frame() {
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(g.shader);

  glBindVertexArray(g.vertex_array);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.index_buffer);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
  SDL_GL_SwapWindow(g.window);
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("error initializing SDL: %s", SDL_GetError());
    return 1;
  }

  g.window = SDL_CreateWindow(
      "Learn OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

  if (!g.window) {
    printf("error initializing window: %s\n", SDL_GetError());
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  g.gl_ctx = SDL_GL_CreateContext(g.window);

  printf("GL Version = %s\n", glGetString(GL_VERSION));
  printf("GLSL Version = %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  SDL_GL_MakeCurrent(g.window, g.gl_ctx);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  float positions[] = {
      -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 0
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // 1
      0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // 2
      -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, // 3
  };

  unsigned int indices[] = {0, 1, 2, 2, 3, 0};

  glGenVertexArrays(1, &g.vertex_array);
  glBindVertexArray(g.vertex_array);

  glGenBuffers(1, &g.vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, g.vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(float), positions,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &g.index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices,
               GL_STATIC_DRAW);

  char vertex_shader[1024], fragment_shader[1024];
  parse_shader("/assets/shaders/test.shader", vertex_shader, fragment_shader);
  g.shader = create_shader(vertex_shader, fragment_shader);
  glUseProgram(g.shader);

  // Unbind buffers
  glUseProgram(0);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(frame, 0, 1);
#endif

  SDL_GL_DeleteContext(g.gl_ctx);
  SDL_DestroyWindow(g.window);
  return 0;
}
