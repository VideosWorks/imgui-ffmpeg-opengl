/*
 * engine.h is a file frome miniDart project
 * Author : Eric Bachard  / lundi 3 octobre 2016, 14:35:03 (UTC+0200)
 * This file is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

#ifndef __ENGINE_HPP
#define __ENGINE_HPP

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES 1
#endif

#include <SDL2/SDL.h>

class Engine
{
  public:
    Engine();
    ~Engine();

    int init_SDL();
    void clean_and_close();
    SDL_GLContext * getGL_Context();
    SDL_Window * getWindow();
    SDL_DisplayMode current;
    int getWidth();
    int getHeight();
  private:
    int mWidth;
    int mHeight;
    SDL_GLContext * glcontext;
    SDL_Window * window;
};

#endif /* __ENGINE_HPP */
