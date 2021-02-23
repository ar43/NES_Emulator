#include "display.h"
#include "../logger/logger.h"
#include "../utility/utility.h"
#include "../machine/memory/memory.h"
#include <cassert>


bool Display::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) 
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to initialize SDL: " + std::string(SDL_GetError()));
        return false;
    }

    window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH*GetScale(), SCREEN_HEIGHT*GetScale(), 0);

    if (window == nullptr)
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to create window " + std::string(SDL_GetError()));
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to create renderer " + std::string(SDL_GetError()));
        return false;
    }

    if (SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT) < 0)
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to set render logical size " + std::string(SDL_GetError()));
        return false;
    }

    if (!palette.Load("default"))
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to load default.pal");
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    return true;
}

void Display::SetScale(uint8_t scale)
{
    if (scale < 1 || scale > 6)
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Display::SetScale scale too large: " + std::to_string(scale));
        return;
    }
    this->scale = scale;
}

void Display::DrawTile(Memory *mem, uint8_t bank, uint8_t index, int x, int y)
{
    assert(index <= 255 && index >= 0 && bank >= 0 && bank <= 1);
    SDL_Color colors[4];
    palette.GetColor(&colors[0], 0x0F);
    palette.GetColor(&colors[1], 0x02);
    palette.GetColor(&colors[2], 0x36);
    palette.GetColor(&colors[3], 0x16);
    uint32_t offset = 0;
    if (bank == 1)
        offset = 0x1000;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            uint8_t data1 = mem->ReadPPU(offset + 16 * index + i);
            uint8_t data2 = mem->ReadPPU(offset + 16 * index + i + 8);
            int bit1 = utility::IsBitSet(data1, 7-j);
            int bit2 = utility::IsBitSet(data2, 7-j);
            int value = 1 * bit1 + 2 * bit2;
            pixels[(y + i) * SCREEN_WIDTH * 4 + (x + j) * 4] = colors[value].r;
            pixels[(y + i) * SCREEN_WIDTH * 4 + (x + j) * 4+1] = colors[value].g;
            pixels[(y + i) * SCREEN_WIDTH * 4 + (x + j) * 4+2] = colors[value].b;
            pixels[(y + i) * SCREEN_WIDTH * 4 + (x + j) * 4+3] = colors[value].a;
        }
    }
}

void Display::RenderStart()
{
    std::fill(std::begin(pixels), std::end(pixels), 0);
}

void Display::DrawChrRom(Memory *mem)
{
    for(int j = 0; j < 16;j++)
    { 
        for (int i = 0; i < 16; i++)
        {
            DrawTile(mem, 0, i+j*16, 8*i, j*8);
        }
    }
    for(int j = 0; j < 10;j++)
    { 
        for (int i = 0; i < 16; i++)
        {
            DrawTile(mem, 1, i+j*16, 8*i, j*8+128);
        }
    }
}

void Display::Render(Memory *mem)
{
    RenderStart();
    DrawChrRom(mem);
    RenderFrame();
}

void Display::RenderFrame()
{
    SDL_RenderClear(renderer);
    uint32_t *cur_pixels = nullptr;
    int pitch = SCREEN_WIDTH * 4;
    int stat = SDL_LockTexture(texture, NULL, (void**)&cur_pixels, &pitch);

    if(stat != 0)
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to lock Texture: " + std::string(SDL_GetError()));
        return;
    }

    for (uint32_t i = 0; i < pixels.size() / 4; i++)
    {
        cur_pixels[i] = pixels[i * 4] << 24 | pixels[i * 4 + 1] << 16 | pixels[i * 4 + 2] << 8 | pixels[i * 4 + 3];
    }

    SDL_UnlockTexture(texture);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

uint8_t Display::GetScale()
{
    return scale;
}

Display::~Display()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}