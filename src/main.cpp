// TileEditor.cpp : Defines the entry point for the console application.
//


#include "pch.h"
#include "Tile.h"


namespace
{
	// SDL
	static SDL_Window *WINDOW;
	static SDL_Renderer *RENDERER;
	static const int WIDTH = 1920;
	static const int HEIGHT = 1080;
	const int RENDER_FRAMES_PER_SECOND = 30;
	size_t renderStart=1;
	size_t ticks;
	int displayPosition = 0;
	bool bMouseDown = false;
	bool bRun = true;

	// Tile data
	std::vector<Tile*> TileList;
	size_t actileTileIndex=0;

	// stage data
	std::string stagePath;
	size_t STAGE_WIDTH;
	size_t STAGE_HEIGHT;
	std::vector<size_t> STAGE_DATA;

	// UI
	SDL_Texture *pSaveExitButton;

	static void SetupWindow()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		if(0 == displayPosition)
		{
			WINDOW = SDL_CreateWindow("Tile Editor", 0,0,WIDTH,HEIGHT,SDL_WINDOW_BORDERLESS);
		}
		else if(1 == displayPosition)
		{
			WINDOW = SDL_CreateWindow("Tile Editor", WIDTH,0,WIDTH,HEIGHT,SDL_WINDOW_BORDERLESS);
		}
		
		RENDERER = SDL_CreateRenderer( WINDOW, -1, SDL_RENDERER_ACCELERATED);
	}
	static void OnInput()
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if( event.type == SDL_MOUSEBUTTONDOWN  )
			{
				bMouseDown = true;
			}
			if( event.type == SDL_MOUSEBUTTONUP  )
			{
				bMouseDown = false;
			}
			if( event.type == SDL_MOUSEBUTTONDOWN  )
			{
				if( event.button.button == SDL_BUTTON_LEFT )
				{ 
					//Get the mouse offsets
					int x = event.button.x;
					int y = event.button.y;


					// check tile menu
					{
						int px = 0;
						int py = 0;
						int index = 0;
						int pw = Tile::GetTargetWidth();
						int ph = Tile::GetTargetHeight();
						for(std::vector<Tile*>::iterator it = TileList.begin(); it!=TileList.end(); ++it)
						{
							if(( x > px ) && ( x < px + pw ) && ( y > py ) && ( y < py + ph ))
							{
								actileTileIndex = index;
							}
							px += 100;
							++index;
						}
					}
					// save and exit
					{
						if( (x>1700) && (y<100) )
						{
							bRun = false;
						}
					}
				}
			}
			if( ((event.type == SDL_MOUSEMOTION ) &&  bMouseDown) || event.type == SDL_MOUSEBUTTONDOWN)
			{
				//Get the mouse offsets
				int x = event.motion.x;
				int y = event.motion.y;
				if(event.type == SDL_MOUSEBUTTONDOWN)
				{
					int x = event.button.x;
					int y = event.button.y;
				}
				// check matrix
				{
					for(size_t hIter = 0; hIter < STAGE_HEIGHT; ++hIter)
					{
						for(size_t wIter = 0; wIter<STAGE_WIDTH; ++wIter)
						{
							int pw = Tile::GetTargetWidth();
							int ph = Tile::GetTargetHeight();
							int px = wIter*Tile::GetTargetWidth();
							int py = (hIter*Tile::GetTargetHeight())+150;
							if(( x > px ) && ( x < px + pw ) && ( y > py ) && ( y < py + ph ))
							{
								STAGE_DATA[wIter+(hIter*STAGE_WIDTH)] = actileTileIndex;
							}

						}
					}
				}
			}

			if( event.type == SDL_QUIT )
			{
				bRun=false;
			}

		}
	}

	static void OnRender()
	{

		// draw matrix
		for(size_t hIter = 0; hIter < STAGE_HEIGHT; ++hIter)
		{
			for(size_t wIter = 0; wIter<STAGE_WIDTH; ++wIter)
			{
				size_t tile = STAGE_DATA[wIter+(hIter*STAGE_WIDTH)];
				TileList[tile]->Render(RENDERER,(wIter*Tile::GetTargetWidth()),(hIter*Tile::GetTargetHeight())+150);
			}
		}

		// draw save exit button
		{
			SDL_Rect target_rect = {1700,0,200,100};
			SDL_Rect source_rect = {0,0,200,100};
			SDL_RenderCopy(RENDERER, pSaveExitButton, &source_rect, &target_rect);
		}

		// draw tiles
		std::vector<Tile*>::iterator et = TileList.end();
		int tileOffset = 0;
		for(std::vector<Tile*>::iterator it = TileList.begin(); it != et; ++it)
		{
			(*it)->Render(RENDERER,tileOffset,0);
			tileOffset+=(*it)->GetSourceWidth();
		}

		// draw active tile
		TileList[actileTileIndex]->Render(RENDERER,1600,0);

		SDL_RenderPresent(RENDERER);
	}
	static void MainLoop()
	{
		renderStart=SDL_GetTicks();

		OnInput();
		OnRender();

		// sdl delay
		ticks = SDL_GetTicks()-renderStart;
		if( ticks < (1000 / RENDER_FRAMES_PER_SECOND) )
		{
			SDL_Delay(( 1000 / RENDER_FRAMES_PER_SECOND ) - ticks);
		}
	}

	static void LoadSettings()
	{
		std::string line;
		std::ifstream myfile ("settings.txt");
		size_t index = 0;
		if (myfile.is_open())
		{
			while ( getline (myfile,line) )
			{
				// TODO: implement
				//std::cout << line << '\n';
				switch(index)
				{
				case 1: // WIDTH
					STAGE_WIDTH = atoi(line.c_str());
					break;
				case 3: // HEIGHT
					STAGE_HEIGHT = atoi(line.c_str());
					break;
				case 5: // FILE
					stagePath = line;
					break;
				case 7: // TARGET WIDTH
					Tile::SetTargetWidth(atoi(line.c_str()));
					break;
				case 9: // TARGET HEIGHT
					Tile::SetTargetHeight(atoi(line.c_str()));
					break;
				case 11: // DISPLAYPOSITION
					displayPosition = atoi(line.c_str());
					break;
				}
				++index;
			}
			myfile.close();
		}

		//
		STAGE_DATA.resize(STAGE_WIDTH*STAGE_HEIGHT);
	}

	static void LoadTiles()
	{
		std::string line;
		std::ifstream myfile ("./Tiles/tiles.txt");
		if (myfile.is_open())
		{
			while ( getline (myfile,line) )
			{
				Tile* pTile = new Tile();
				pTile->LoadTexture( RENDERER, line );
				TileList.push_back(pTile);
			}
			myfile.close();
		}

		// load save exit button
		{
			std::string tilePath(".\\UI\\SaveExit.png");
			SDL_Surface* surface = IMG_Load(tilePath.c_str());
			pSaveExitButton = SDL_CreateTextureFromSurface(RENDERER, surface);
			SDL_FreeSurface(surface);
		}

	}
	static void FreeTiles()
	{
		for(std::vector<Tile*>::iterator it = TileList.begin();it != TileList.end();++it)
		{
			delete (*it);
		}
		SDL_DestroyTexture(pSaveExitButton);
	}

	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}


	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}

	static void LoadStage()
	{
		std::string line;
		std::ifstream myfile (stagePath);
		bool bFirstLine = true;
		int tileIndex = 0;
		if (myfile.is_open())
		{
			while ( getline (myfile,line) )
			{
				std::vector<std::string> tokens = split(line,',');
				if(bFirstLine)
				{
					STAGE_WIDTH = atoi(tokens[0].c_str());
					STAGE_HEIGHT = atoi(tokens[1].c_str());
					bFirstLine = false;
				}
				else
				{
					for(std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
					{
						int tileType = atoi((*it).c_str());
						STAGE_DATA[tileIndex] = tileType;
						++tileIndex;
					}
				}
			}
			myfile.close();
		}
	}
	static void SaveStage()
	{
		std::ofstream myfile (stagePath);
		if (myfile.is_open())
		{
			myfile <<STAGE_WIDTH<< ","<<STAGE_HEIGHT<<std::endl;
			for(size_t hIter = 0; hIter < STAGE_HEIGHT; ++hIter)
			{
				for(size_t wIter = 0; wIter<STAGE_WIDTH; ++wIter)
				{
					myfile << STAGE_DATA[wIter+(hIter*STAGE_WIDTH)];
					if( (wIter+1 != STAGE_WIDTH))
					{
						myfile << ",";
					}
				}
				myfile <<"\n";
			}
			myfile.close();
		}
	}
}

int main(int argc, char* argv[])
{
	LoadSettings();
	SetupWindow();
	LoadTiles();
	LoadStage();
	while( bRun )
	{
		MainLoop();
	}
	SaveStage();
	FreeTiles();
	SDL_Quit();
	return 0;
}

