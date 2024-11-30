#include <GLES3/gl3.h>
#include <SDL2/SDL.h>
#include <alloca.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 640

typedef struct {
  SDL_Window *window;
  SDL_GLContext *gl_ctx;
} global_t;

global_t g;

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

  glClearColor(0.0f, 1.0f, 0.0f, 0.0f);

  float positions[] = {
      -0.5f, -0.5f, // 0
      0.5f,  -0.5f, // 1
      0.5f,  0.5f,  // 2
      -0.5f, 0.5f,  // 3
  };

  unsigned int indices[] = {0, 1, 2, 2, 3, 0};

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  unsigned int ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices,
               GL_STATIC_DRAW);

  const char *vertex_shader = "#version 300 es\n"
                              "\n"
                              "layout(location = 0) in vec4 position;\n"
                              "void main()\n"
                              "{\n"
                              " gl_Position = position;\n"
                              "}\n";

  const char *fragment_shader = "#version 300 es\n"
                                "\n"
                                "precision mediump float;\n"
                                "layout(location = 0) out vec4 color;\n"
                                "void main()\n"
                                "{\n"
                                " color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
                                "}\n";

  unsigned int shader = create_shader(vertex_shader, fragment_shader);
  glUseProgram(shader);

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(frame, 0, 1);
#endif

  SDL_GL_DeleteContext(g.gl_ctx);
  SDL_DestroyWindow(g.window);
  return 0;
}
