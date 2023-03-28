#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "main.h";

using namespace std;


int main(int argc, char* argv[])
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  initSDL(window, renderer);

  waitUntilKeyPressed();
  quitSDL(window, renderer);
  return 0;
}
