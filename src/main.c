#define UTIL_IMPL
#include "shader.h"
#include <GLES3/gl3.h>
#include <SDL2/SDL.h>
#include <cglm/cglm.h>
#include <stdint.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 640

typedef struct {
  float x, y, z;
  float r, g, b;
  int16_t u, v;
} vertex_t;

typedef struct {
  SDL_Window *window;
  SDL_GLContext *gl_ctx;

  unsigned int shader;
  unsigned int vertex_array;
  unsigned int vertex_buffer;
  unsigned int index_buffer;
  unsigned int model_loc;
} global_t;

global_t g;

static void frame() {
  uint32_t ticks = SDL_GetTicks();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(g.shader);

  glBindVertexArray(g.vertex_array);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.index_buffer);

  mat4 model = GLM_MAT4_IDENTITY_INIT;
  glm_rotate(model, glm_rad((float)ticks * 0.1f), (vec3){1.0f, 1.0f, 0.0f});
  glUniformMatrix4fv(g.model_loc, 1, GL_FALSE, (float *)model);

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

  SDL_GL_SwapWindow(g.window);
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("error initializing SDL");
    return 1;
  }

  g.window = SDL_CreateWindow(
      "Learn OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  g.gl_ctx = SDL_GL_CreateContext(g.window);

  printf("GL Version "
         "= %s\n",
         glGetString(GL_VERSION));
  printf("GLSL "
         "Version = "
         "%s\n",
         glGetString(GL_SHADING_LANGUAGE_VERSION));

  SDL_GL_MakeCurrent(g.window, g.gl_ctx);

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

  vertex_t vertices[] = {
      {-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0, 0},
      { 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1, 0},
      { 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1, 1},
      {-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0, 1},

      {-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0, 0},
      { 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1, 0},
      { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1, 1},
      {-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0, 1},

      { 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0, 0},
      { 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1, 0},
      { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1, 1},
      { 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0, 1},

      {-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0, 0},
      { 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1, 0},
      { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1, 1},
      {-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0, 1},

      {-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0, 0},
      {-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1, 0},
      {-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1, 1},
      {-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0, 1},
      //
      {-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0, 0},
      { 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1, 0},
      { 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1, 1},
      {-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0, 1},
  };

  unsigned int indices[] = {
      0,  1,  2,  2,  3,  0,  // bottom
      4,  5,  6,  6,  7,  4,  // top
      8,  9,  10, 10, 11, 8,  // right
      12, 13, 14, 14, 15, 12, // back
      16, 17, 18, 18, 19, 16, // left
      20, 21, 22, 22, 23, 20, // front
  };

  glGenVertexArrays(1, &g.vertex_array);
  glBindVertexArray(g.vertex_array);

  glGenBuffers(1, &g.vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, g.vertex_buffer);
  // glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vertex_t), vertices,
  // GL_STATIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vertex_t), vertices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(vertex_t),
                        (void *)(3 * sizeof(float) + 3 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glGenBuffers(1, &g.index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.index_buffer);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices,
  //              GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), indices,
               GL_STATIC_DRAW);
  stbi_set_flip_vertically_on_load(1);
  int width, height, channels;
  unsigned char *tex_data =
      stbi_load("/assets/nikke.png", &width, &height, &channels, 4);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  unsigned int texture;
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, tex_data);

  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(tex_data);

  char vertex_shader[1024], fragment_shader[1024];
  parse_shader("/assets/shaders/test.shader", vertex_shader, fragment_shader);
  g.shader = create_shader(vertex_shader, fragment_shader);
  glUseProgram(g.shader);

  mat4 model                  = GLM_MAT4_IDENTITY_INIT;
  mat4 view                   = GLM_MAT4_IDENTITY_INIT;
  mat4 projection             = GLM_MAT4_IDENTITY_INIT;

  g.model_loc                 = glGetUniformLocation(g.shader, "model");
  unsigned int view_loc       = glGetUniformLocation(g.shader, "view");
  unsigned int projection_loc = glGetUniformLocation(g.shader, "projection");

  glm_rotate(model, glm_rad(-55.0f), (vec3){1.0f, 0.0f, 0.0f});
  glm_translate(view, (vec3){0.0f, 0.0f, -5.0f});
  glm_perspective(glm_rad(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
                  0.1f, 100.0f, projection);

  glUniformMatrix4fv(g.model_loc, 1, GL_FALSE, (float *)model);
  glUniformMatrix4fv(view_loc, 1, GL_FALSE, (float *)view);
  glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float *)projection);

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
