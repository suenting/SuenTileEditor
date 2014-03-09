#ifndef TILE_H
#define TILE_H
#include "pch.h"

class Tile
{
public:
	Tile();
	~Tile();
	void LoadTexture(SDL_Renderer *RENDERER, std::string path);
	void Render(SDL_Renderer *RENDERER, int x, int y )const;

	static int GetTargetWidth() { return mTargetWidth; }
	static void SetTargetWidth(int val) { mTargetWidth = val; }
	static int GetTargetHeight() { return mTargetHeight; }
	static void SetTargetHeight(int val) { mTargetHeight = val; }
	int GetSourceWidth() const { return mSourceWidth; }
	int GetSourceHeight() const { return mSourceHeight; }
protected:
private:
	SDL_Texture *mpTexture;
	int mSourceWidth;
	int mSourceHeight;
	static int mTargetWidth;
	static int mTargetHeight;
};

#endif