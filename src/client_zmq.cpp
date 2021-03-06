#include <cstdio>
#include <SDL2/SDL.h>

#include "zhelpers.hpp"

#include "../include/FaceTracker.h"

size_t size = sizeof(FaceTrack);

using namespace zmq;
using namespace std;


int main(void) {
    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "Tracking",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        FT_CHIP_SIZE,
        FT_CHIP_SIZE,
        0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    surface->format->format = SDL_PIXELFORMAT_RGBA32;

    //  Prepare our context and subscriber
    context_t context(1);
    socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5563");
    subscriber.setsockopt( ZMQ_SUBSCRIBE, "Tracking", 1);

    SDL_Event event;
    int quit = 0;
    while (!quit) {

        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }
        
        //  Read envelope with address
        string address = s_recv(subscriber);
        message_t msg;
        subscriber.recv(&msg);
        FaceTrack* faceTrack = (FaceTrack*) msg.data();
        SDL_Surface* image = SDL_CreateRGBSurfaceFrom(
            faceTrack->face, 
            FT_CHIP_SIZE,           // width
            FT_CHIP_SIZE,           // height
            FT_COLORS*8,            // depth
            FT_CHIP_SIZE*FT_COLORS, // pitch
            0xff0000,
            0x00ff00,
            0x0000ff,
            0x000000);
        SDL_BlitSurface(image, NULL, surface, NULL);
        SDL_FreeSurface(image);
        //surface->pixels = faceTrack->face;
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
