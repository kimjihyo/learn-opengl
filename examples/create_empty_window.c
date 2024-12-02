#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
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

void frame() {}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("error initializing SDL: %s", SDL_GetError());
    return 1;
  }

  g.window =
      SDL_CreateWindow("Learn OpenGL", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  SDL_GL_MakeCurrent(g.window, g.gl_ctx);

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(frame, 0, 1);
#endif

  SDL_GL_DeleteContext(g.gl_ctx);
  SDL_DestroyWindow(g.window);
  return 0;
}
