# kekken 
A little 2D fighting game engine designed to be easily extensible and portable. 

## to compile on linux:
Just pull SDL2, SDL_image, and SDL_ttf using your favorite package manager. If you happen to be on Arch Linux, this line should do the trick:

```sudo pacman -Syu sdl2 sdl2_image sdl2_ttf```

## macOS: 
Download the required .frameworks (or, if you prefer, build your own libraries and tell the compiler where to look for them):
SDL2: http://www.libsdl.org/download-2.0.php#source

SDL_image: https://www.libsdl.org/projects/SDL_image/ 

SDL_ttf: https://www.libsdl.org/projects/SDL_ttf/

1. Unpack and copy the frameworks into /Library/Frameworks/.
2. Copy the SDL_image and SDL_ttf headers from their respective framework folders into /Library/Frameworks/SDL2.framework/Headers. 
3. Use ```make mac``` to compile. 

### Alternatively, using xcode:
You can copy the .framework files into the source directory and just compile. 

## Windows:
Coming soon!

### TO DO: 
Genetic algo vs. Q-learning agent!
