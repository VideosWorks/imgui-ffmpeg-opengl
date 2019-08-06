/*
 * Fichier input.cpp pour le projet miniDart
 * Auteur : Eric Bachard  / mardi 4 octobre 2016, 10:40:50 (UTC+0200)
 * Ce document est sous Licence GPL v2
 * voir : http://www.gnu.org/licenses/gpl-2.0.html
 */


#include <SDL2/SDL.h>
#include "input.hpp"

Input::Input() : mdPos_x(0), mdPos_y(0), mdRelPos_x(0),mdRelPos_y(0), mb_Quit(false)
{
  mbIsInputAKey[SDL_NUM_SCANCODES-1] = {0};
  mbIsInputMiceButton[MICE_BUTTONS_NUMBER-1] = {0};
}

Input::~Input()
{
}

void Input::updateInputEvents()
{
  mdRelPos_x = 0;
  mdRelPos_y = 0;

  while(SDL_PollEvent(&mInputEvent))
  {
    switch(mInputEvent.type)
    {
      case SDL_KEYDOWN:
        mbIsInputAKey[mInputEvent.key.keysym.scancode] = true;
      break;

      case SDL_KEYUP:
        mbIsInputAKey[mInputEvent.key.keysym.scancode] = false;
      break;

      case SDL_MOUSEBUTTONDOWN:
        mbIsInputMiceButton[mInputEvent.button.button] = true;
      break;

      case SDL_MOUSEBUTTONUP:
        mbIsInputMiceButton[mInputEvent.button.button] = false;
      break;

      case SDL_MOUSEMOTION:
      {
        mdPos_x = mInputEvent.motion.x;
        mdPos_y = mInputEvent.motion.y;

        mdRelPos_x = mInputEvent.motion.xrel;
        mdRelPos_y = mInputEvent.motion.yrel;
      }
      break;

      case SDL_WINDOWEVENT:
      {
        if(mInputEvent.window.event == SDL_WINDOWEVENT_CLOSE)
          mb_Quit = true;
      }
      break;

      case SDL_DROPFILE:
          printf("Dropped file %s\n", mInputEvent.drop.file);
      break;

      // FIXME : add other SDL_WINDOW_EVENTS, like focus, and so on to manage several windows

      default:
      break;
    }
  }
}

bool Input::bProgramQuit()
{
  return mb_Quit;
}

bool Input::bIsCurrentKey(const SDL_Scancode aKey) const
{
  return mbIsInputAKey[aKey];
}

bool Input::bIsCurrentMiceButton(const Uint8 aMiceButton) const
{
  return mbIsInputMiceButton[aMiceButton];
}

bool Input::bIsMouseMotion() const
{
  if ((mdRelPos_x == 0) && (mdRelPos_y == 0))
    return false;
  else
    return true;
}

int Input::getPos_x() const
{
  return mdPos_x;
}

int Input::getPos_y() const
{
  return mdPos_y;
}

int Input::getRelPos_x() const
{
  return mdRelPos_x;
}

int Input::getRelPos_y() const
{
  return mdRelPos_y;
}


