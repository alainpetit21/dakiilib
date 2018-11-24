TODO
- WRITE Description


HOW TO COMPILE


- Install SDL
  - sudo dnf install SDL2
  - sudo dnf install SDL2-devel
- install SDL_image
  - sudo dnf install SDL2_image
  - sudo dnf install SDL2_image-devel
- install SDL_mixer
  - sudo dnf install SDL2_mixer
  - sudo dnf install SDL2_mixer-devel

- install CODEBLOCK & libs
  - install CODEBLOCK
  - sudo dnf install gcc-c++
  - sudo dnf install glut-devel
  - sudo dnf install automake
  - sudo yum groupinstall "X Software Development"
  - Under Compiler setting, Other compiler options :
    - -fexceptions
    - `sdl2-config --cflags`
  - Under Compiler setting, #defines :
    - _LINUX
    - _DEBUG for debug build target
    - NDEBUG for Release build target
  - Under Linker Setting, Link librairies
    - libSDL2_image
    - libSDL2_mixer
  - Under Linker Setting, Other linker options :
    - `sdl2-config --libs`
  - add in Search Directories, under Compiler :
    -  lib
    - dfc
    - helper
    - /usr/include/SDL2
