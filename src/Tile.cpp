#include "Tile.h"




int Tile::mTargetWidth;
int Tile::mTargetHeight;

Tile::Tile()
{

}

Tile::~Tile()
{
	SDL_DestroyTexture(mpTexture);
}

void Tile::Render(SDL_Renderer *RENDERER, int x, int y ) const
{
	SDL_Rect target_rect = {x,y,mTargetWidth,mTargetHeight};
	SDL_Rect source_rect = {0,0,mSourceWidth,mSourceHeight};
	SDL_RenderCopy(RENDERER, mpTexture, &source_rect, &target_rect);
}

void Tile::LoadTexture( SDL_Renderer *RENDERER, std::string path )
{
	std::string tilePath(".\\Tiles\\"+path);
	SDL_Surface* surface = IMG_Load(tilePath.c_str());
	mpTexture = SDL_CreateTextureFromSurface(RENDERER, surface);
	mSourceWidth = surface->w;
	mSourceHeight = surface->h;
	SDL_FreeSurface(surface);
}

