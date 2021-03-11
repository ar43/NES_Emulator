#include "display.h"
#include "../../../logger/logger.h"
#include "../../../utility/utility.h"
#include "../../memory/memory.h"
#include <cassert>
#include <cmath>


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

    SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 0xFF);
    //texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    texture = nullptr;
    surface = SDL_CreateRGBSurfaceWithFormat(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_PIXELFORMAT_RGBA8888);
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

//void Display::DrawBackgroundTile(Memory *mem, uint8_t bank, uint8_t index, SDL_Color* color_pointer, int x, int y)
//{
//    assert(index <= 255 && index >= 0 && bank >= 0 && bank <= 1);
//    uint32_t* pixels = (uint32_t*)surface->pixels;
//    for (int i = 0; i < TILE_HEIGHT; i++)
//    {
//        for (int j = 0; j < TILE_WIDTH; j++)
//        {
//            uint8_t value = mem->pixel_values[bank][index*PIXEL_PER_TILE + i*TILE_WIDTH+j];
//            int loc = (y + i) * SCREEN_WIDTH + (x + j);
//            if (value == 0)
//                continue;
//            if (loc >= SCREEN_HEIGHT*SCREEN_WIDTH || x+j > 255 || loc < 0 || x+j < 0)
//                continue;
//            pixels[loc] = color_pointer[value].r << 24 | color_pointer[value].g << 16 | color_pointer[value].b << 8 | 0xFF;
//            //counter++;
//        }
//    }
//}

//void Display::DrawBackgroundTileOverride(Memory *mem, uint8_t bank, uint8_t index, SDL_Color* color_pointer, int x, int y)
//{
//    assert(index <= 255 && index >= 0 && bank >= 0 && bank <= 1);
//    uint32_t* pixels = (uint32_t*)surface->pixels;
//    for (int i = 0; i < TILE_HEIGHT; i++)
//    {
//        for (int j = 0; j < TILE_WIDTH; j++)
//        {
//            uint8_t value = mem->pixel_values[bank][index*PIXEL_PER_TILE + i*TILE_WIDTH+j];
//            pixels[(y + i) * SCREEN_WIDTH + (x + j)] = color_pointer[value].r << 24 | color_pointer[value].g << 16 | color_pointer[value].b << 8 | 0xFF;
//            //counter++;
//        }
//    }
//}

void Display::DrawSprite(Memory *mem, uint8_t bank, uint8_t index, uint8_t palette_id, bool flip_h, bool flip_v, int x, int y)
{
    assert(index <= 255 && index >= 0 && bank >= 0 && bank <= 1);
    SDL_Color colors[4];
    palette.GetColor(&colors[0], palette.universal_background);

    for(int i = 0; i < 3;i++)
        palette.GetColor(&colors[i+1], palette.sprite[palette_id][i]);

    uint32_t* pixels = (uint32_t*)surface->pixels;

    int loc = 0;
    int x_calc = 0;
    int y_calc = 0;
    //logger::PrintLine(logger::LogType::DEBUG, "loc: " + std::to_string(x) + " " + std::to_string(y));
    for (int i = 0; i < TILE_HEIGHT; i++)
    {
        for (int j = 0; j < TILE_WIDTH; j++)
        {

            uint8_t value = mem->pixel_values[bank][index*PIXEL_PER_TILE + i*TILE_WIDTH+j];
            if (value == 0)
                continue;

            if (!flip_h && !flip_v)
            {
                x_calc = x + j;
                y_calc = y + i;
            }
            else if (flip_h && flip_v)
            {
                x_calc = (x + (7 - j));
                y_calc = (y + (7 - i));
            }
            else if(flip_h)
            {
                x_calc = (x + (7 - j));
                y_calc = y + i;
            }
            else if (flip_v)
            {
                x_calc = x + j;
                y_calc = (y + (7 - i));
            }

            loc = y_calc * SCREEN_WIDTH + x_calc;
            if (x_calc >= SCREEN_WIDTH || x_calc < 0 || y_calc >= SCREEN_HEIGHT || y_calc < 0 || !mem->ppu_registers->ppumask.IsBitSet(MaskBits::SHOW_SPRITES_LEFT) && x_calc < 8)
                continue;
            
            pixels[loc] = colors[value].r << 24 | colors[value].g << 16 | colors[value].b << 8 | 0xFF;
            //counter++;
        }
    }
}

void Display::RenderStart(Memory *mem)
{
    palette.LoadSprite(mem);
    palette.LoadBackground(mem);
    SDL_Color color;
    palette.GetColor(&color, palette.universal_background);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xfe);
    SDL_RenderClear(renderer);
    SDL_FillRect(surface, NULL,SDL_MapRGB(surface->format,color.r,color.g,color.b));
    if (texture != nullptr)
        SDL_DestroyTexture(texture);
    DrawSprites(mem,true);
}

void Display::DrawChrRom(Memory *mem)
{
    /*for(int j = 0; j < 16;j++)
    { 
        for (int i = 0; i < 16; i++)
        {
            DrawBackgroundTile(mem, 0, i+j*16, 8*i, j*8);
        }
    }
    for(int j = 0; j < 16;j++)
    { 
        for (int i = 0; i < 16; i++)
        {
            DrawBackgroundTile(mem, 1, i+j*16, 8*i+128, j*8);
        }
    }*/
}

void Display::DrawSprites(Memory *mem, bool behind)
{
    bool toggle = mem->ppu_registers->ppumask.IsBitSet(MaskBits::SHOW_SPRITES);
    if (!toggle)
        return;

    if (mem->ppu_registers->ppuctrl.IsBitSet(ControllerBits::SPRITE_SIZE))
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unimplemented 8-16 sprite mode");

    int nametable = mem->ppu_registers->ppuctrl.GetNametable(mem->ppu_registers->v);
    uint8_t bank = mem->ppu_registers->ppuctrl.IsBitSet(ControllerBits::SPRITE_PATTERN);

    for (int i = NUM_SPRITES-1; i >= 0; i--)
    {
        uint8_t index = i * 4;
        uint8_t attributes = mem->oam_data[index + 2];
        if (utility::IsBitSet(attributes, 5) != behind)
            continue;
        
        uint8_t y = mem->oam_data[index];
        if (y >= 0xEF)
            continue;
        uint8_t x = mem->oam_data[index + 3];
        uint8_t tile_id = mem->oam_data[index + 1];
        
        uint8_t palette_id = attributes & 3;
        
        bool flip_h = utility::IsBitSet(attributes, 6);
        bool flip_v = utility::IsBitSet(attributes, 7);
        DrawSprite(mem, bank, tile_id, palette_id, flip_h, flip_v, x, y+1);
    }
}

void Display::GetBackgroundMetaTileColor(Memory *mem, SDL_Color *color, int x, int y, int nametable)
{
    int index = (y / 4) * 8 + (x / 4);
    uint8_t byte = mem->ReadPPU(nametable + 960 + index);
    bool x_odd = (bool)((x / 2) % 2);
    bool y_odd = (bool)((y / 2) % 2);
    if (x_odd && y_odd)
    {
        index = (byte >> 6) & 3;
    }
    else if (!x_odd && y_odd)
    {
        index = (byte >> 4) & 3;
    }
    else if (x_odd && !y_odd)
    {
        index = (byte >> 2) & 3;
    }
    else
    {
        index = byte & 3;
    }
    palette.GetColor(&color[0], palette.universal_background);
    palette.GetColor(&color[1], palette.background[index][0]);
    palette.GetColor(&color[2], palette.background[index][1]);
    palette.GetColor(&color[3], palette.background[index][2]);
}

void Display::DrawBackgroundTileLine(Memory *mem, uint8_t bank, uint8_t index, SDL_Color* color_pointer, uint8_t read_line, int x, int line)
{
    assert(index <= 255 && index >= 0 && bank >= 0 && bank <= 1);
    uint32_t* pixels = (uint32_t*)surface->pixels;
    
    for (int j = 0; j < TILE_WIDTH; j++)
    {
        uint8_t value = mem->pixel_values[bank][index*PIXEL_PER_TILE + ((read_line) & 7)*TILE_WIDTH+j];
        int loc = line * SCREEN_WIDTH + (x + j);
        if (value == 0)
            continue;
        if (loc >= SCREEN_HEIGHT*SCREEN_WIDTH || x+j > 255 || loc < 0 || x+j < 0 || !mem->ppu_registers->ppumask.IsBitSet(MaskBits::SHOW_BACKGROUND_LEFT) && (x+j) < 8)
            continue;
        pixels[loc] = color_pointer[value].r << 24 | color_pointer[value].g << 16 | color_pointer[value].b << 8 | 0xFF;
        //counter++;
    }
}

void Display::DrawBackgroundLineHSA(Memory* mem, uint8_t x_shift, uint8_t y_shift, int nametable, uint8_t bank, int line)
{
    bool toggle = mem->ppu_registers->ppumask.IsBitSet(MaskBits::SHOW_BACKGROUND);
    if (!toggle)
        return;

    SDL_Color colors[4];
    int test = x_shift / 8;
    for (int x = test; x < 32; x++)
    {
        int final_x = x * 8 - x_shift;
        GetBackgroundMetaTileColor(mem, colors, x, (line+y_shift)/8, nametable);
        uint8_t index = mem->ReadPPU(nametable + ((line+y_shift)/8) * 32 + x);
        DrawBackgroundTileLine(mem, bank, index, colors, line+y_shift, final_x, line);
    }
}

void Display::DrawBackgroundLineHSB(Memory* mem, uint8_t x_shift, uint8_t y_shift, int nametable, uint8_t bank, int line)
{
    bool toggle = mem->ppu_registers->ppumask.IsBitSet(MaskBits::SHOW_BACKGROUND);
    if (!toggle)
        return;

    SDL_Color colors[4];
    nametable = utility::GetOtherNametable(nametable, 0);
    int test = (int)ceil(double(x_shift) / double(8));
    for (int x = 0; x < test; x++)
    {
        int final_x = x * 8 + 256 - x_shift;
        GetBackgroundMetaTileColor(mem, colors, x, (line+y_shift)/8, nametable);
        uint8_t index = mem->ReadPPU(nametable + ((line+y_shift)/8) * 32 + x);
        DrawBackgroundTileLine(mem, bank, index, colors, line+y_shift, final_x, line);
    }
}

void Display::DrawBackgroundLineVSB(Memory* mem, uint8_t x_shift, uint8_t y_shift, int nametable, uint8_t bank, int line)
{
    bool toggle = mem->ppu_registers->ppumask.IsBitSet(MaskBits::SHOW_BACKGROUND);
    if (!toggle)
        return;

    int new_line = line - (239-y_shift)-1;

    SDL_Color colors[4];
    nametable = utility::GetOtherNametable(nametable, 1);
    int test = x_shift / 8;
    for (int x = test; x < 32; x++)
    {
        int final_x = x * 8 - x_shift;
        GetBackgroundMetaTileColor(mem, colors, x, (new_line)/8, nametable);
        uint8_t index = mem->ReadPPU(nametable + ((new_line)/8) * 32 + x);
        DrawBackgroundTileLine(mem, bank, index, colors, new_line, final_x, line);
    }
}

void Display::Render(Memory *mem)
{
    DrawSprites(mem,false);
    RenderEnd();
}

SDL_Window* Display::GetWindow()
{
    return window;
}

void Display::RenderEnd()
{
    texture = SDL_CreateTextureFromSurface(renderer, surface);

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