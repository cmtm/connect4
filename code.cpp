#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>   //remove this once project is complete
#include <string>

//Surfaces
SDL_Surface* screen = NULL;
SDL_Surface* optimizedImage = NULL;
SDL_Surface* introBackground = NULL;
SDL_Surface* blackRectangle = NULL;
SDL_Surface* message = NULL;
SDL_Surface* redDisk = SDL_CreateRGBSurface(SDL_SWSURFACE, 80, 80, 32, 0, 0, 0, 0);
SDL_Surface* blueDisk = SDL_CreateRGBSurface(SDL_SWSURFACE, 80, 80, 32, 0, 0, 0, 0);

TTF_Font* font = NULL;

SDL_Rect tileCutter;

SDL_Color textColor = {255,255,255};

SDL_Event event;

int grid[7][6]={0};
int turn;



bool intro_screen()
{
	SDL_BlitSurface(introBackground,NULL, screen,NULL);
	SDL_Flip(screen);
	while(true)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_KEYDOWN)
				if(event.key.keysym.sym == SDLK_SPACE)
					return true;
			if(event.type == SDL_QUIT)
				return false;
		}
	}
}

void init()
{
	std::cout<<"Console debug initiated \n";   //remove when project is finnished
	
	//Start SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(560, 640, 32, SDL_SWSURFACE);
	TTF_Init();

	
	//load background
	SDL_Surface* initialLoad = IMG_Load("Connect 4 background.png");
	introBackground = SDL_DisplayFormat(initialLoad);
	SDL_FreeSurface(initialLoad);

	//create black rectangle
	blackRectangle = SDL_CreateRGBSurface(SDL_SWSURFACE, 560, 640, 32,0,0,0,0);

	//create redDisk, blueDisk and tile (for the tile, we only create the cutter)
	{
		initialLoad = IMG_Load("Connect 4.png");    //file name
		optimizedImage = SDL_DisplayFormat(initialLoad);
		SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );

		SDL_Rect redCutter;
		redCutter.x = 0; redCutter.y = 0; redCutter.h = 80; redCutter.w = 80; 

		SDL_Rect blueCutter;
		blueCutter.x = 80; blueCutter.y = 0; blueCutter.h = 80; blueCutter.w = 80;

		tileCutter.x = 160; tileCutter.y = 0; tileCutter.h = 80; tileCutter.w = 80;

		SDL_BlitSurface(optimizedImage, &redCutter, redDisk, NULL);
		SDL_BlitSurface(optimizedImage, &blueCutter, blueDisk, NULL);

		SDL_FreeSurface(initialLoad);
	}

	//Load font
	font = TTF_OpenFont("font.ttf", 35);
	
}

void display_top_text()
{
	std::string msg;
	if(turn ==1)
		msg = "Red's Turn";
	if(turn ==2)
		msg = "Blue's Turn";
	message = TTF_RenderText_Solid(font, msg.c_str()  , textColor);
	SDL_Rect temp;
	temp.x = (screen->w - message->w)/2;
	temp.y = 40;
	SDL_BlitSurface(message,NULL,screen,&temp);
}

void render_graphics()
{
	//Fill the surface black
	SDL_BlitSurface(blackRectangle, NULL, screen, NULL);


	//Draw the disks and tile
	SDL_Rect temp;
	for(int i=0; i<7;i++)
	{
		for(int j=0; j<6;j++)
		{
			temp.x = i*80;
			temp.y = (j*80)+160;
			if(grid[i][j]==1)
				SDL_BlitSurface(redDisk,NULL,screen,&temp);
			if(grid[i][j]==2)
				SDL_BlitSurface(blueDisk,NULL,screen,&temp);
			SDL_BlitSurface(optimizedImage,&tileCutter,screen,&temp);
		}
	}

	display_top_text();

	//Display the screen
	SDL_Flip(screen);
}



int get_lowest_free(int column)
{
	for(int i=0; i<6; i++)
		if(grid[column][i] != 0)
			return (i-1);
	return 5;
}

void handle_events()
{
	if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
	{
		int column;
		
		//find where the mouse button was pushed
		if(event.button.x <= 80)
		{
			column=0;
			std::cout<<"Got click in first column \n"; //debug
		}
		if(event.button.x <= 160 && event.button.x >80)
		{
			column=1;
			std::cout<<"Got click in second column \n";  //debug
		}
		if(event.button.x <= 240 && event.button.x > 160)
			column=2;
		if(event.button.x <= 320 && event.button.x > 240)
			column=3;
		if(event.button.x <= 400 && event.button.x > 320)
			column=4;
		if(event.button.x <= 480 && event.button.x > 400)
			column=5;
		if(event.button.x <= 560 && event.button.x > 480)
			column=6;

		//Find potential disk location
		if(grid[column][0] == 0)
		{
			//put the disk in the proper slot
			grid[column][get_lowest_free(column)]=turn;
			std::cout<<"got grid click! \n"; //debug

			//switch turn
			if(turn == 1)
				turn = 2;
			else
				turn = 1;
			std::cout<<"turn switch \n";  //debug

		}		
	}
}


void quit_()
{
	// free the used surfaces
	SDL_FreeSurface(blackRectangle);
	SDL_FreeSurface(redDisk);
	SDL_FreeSurface(blueDisk);
	SDL_FreeSurface(optimizedImage);

	TTF_CloseFont(font);

	TTF_Quit();
	SDL_Quit();
}

void end_game()
{
	std::string msg;
	if(turn ==2)
		msg = "RED PLAYER WINS!";
	if(turn == 1)
		msg = "BLUE PLAYER WINS!";

	
	for(int i=1; i<51; i++)
	{
		//Fill the surface black
		SDL_BlitSurface(blackRectangle, NULL, screen, NULL);


		//Draw the disks and tile
		SDL_Rect temp;
		for(int i=0; i<7;i++)
		{
			for(int j=0; j<6;j++)
			{
				temp.x = i*80;
				temp.y = (j*80)+160;
				if(grid[i][j]==1)
					SDL_BlitSurface(redDisk,NULL,screen,&temp);
				if(grid[i][j]==2)
					SDL_BlitSurface(blueDisk,NULL,screen,&temp);
				SDL_BlitSurface(optimizedImage,&tileCutter,screen,&temp);
			}
		}

		TTF_CloseFont(font);
		font = TTF_OpenFont("font.ttf", i);
		message = TTF_RenderText_Solid(font, msg.c_str()  , textColor);
		temp.x = (screen->w - message->w)/2;
		temp.y = i*6;

		//draw the message
		SDL_BlitSurface(message,NULL,screen,&temp);
		SDL_Flip(screen);
		SDL_FreeSurface(message);
		SDL_Delay(50);
	}
}


bool check_for_win()
{
	for(int t=1; t<3;t++)
	{
		//Diagonal left to right
		for(int x=0; x<4; x++)
			for(int y=0; y<3;y++)
				if(grid[x][y]==t && grid[x+1][y+1]==t && grid[x+2][y+2]==t && grid[x+3][y+3]==t)
				{
					end_game();
					return true;
				}
		
		//Diagonal right to left
		for(int x=3; x<7; x++)
			for(int y=0; y<3; y++)
				if(grid[x][y]==t && grid[x-1][y+1]==t && grid[x-2][y+2]==t && grid[x-3][y+3]==t)
				{
					end_game();
					return true;
				}

		//Vertical
		for(int x=0; x<7; x++)
			for(int y=0; y<3; y++)
				if(grid[x][y]==t && grid[x][y+1]==t && grid[x][y+2]==t && grid[x][y+3]==t)
				{
					end_game();
					return true;
				}

		//Horizontal
		for(int x=0; x<4; x++)
			for(int y=0; y<6; y++)
				if(grid[x][y]==t && grid[x+1][y]==t && grid[x+2][y]==t && grid[x+3][y]==t)
				{
					end_game();
					return true;
				}		
	}
	return false;
}




int main(int argc, char* args[])
{
	bool quit = false;
	init();

	// set background and wait for response
	if(	!intro_screen())
	{
		quit_();
		return 0;
	}
	SDL_FreeSurface(introBackground);

	//set turn
	turn = 1;

	//start game loop
	while(quit==false)
	{
		render_graphics();

		if(check_for_win())
		{
			quit_();
			return 0;
		}

		while(SDL_PollEvent(&event))
		{
			if(event.type==SDL_QUIT)
			{
				quit = true;
				quit_();
				return 0;
			}
			else
				handle_events();
			std::cout<<turn<<'\n';  //debug
			//check for win and quit if there is one
		}
	}
	return 0;
}


	
