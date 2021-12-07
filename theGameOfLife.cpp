#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <array>
#include <unistd.h>

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}
	
	bool OnUserCreate() override
	{
		for(int x = 0; x < 256; x++)
		{
			for(int y = 0; y < 256; y++)
			{
				gameBoard[x][y] = 0;
			}
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		switch(gameState)
		{
			case 0: // insert mode
			{
				if (GetMouse(0).bHeld)
				{
					gameBoard[GetMouseX()][GetMouseY()] = 1;
				}

				if(GetKey(olc::Key::SPACE).bPressed) 
				{
					gameState = 1;
					return true;
				}

				for(int x = 0; x < 255; x++)
				{
					for(int y = 0; y < 255; y++)
					{
						if(gameBoard[x][y] == 0) Draw(x, y, olc::WHITE);
						else Draw(x, y, olc::BLACK);
					}
				}
				return true;
			}
			case 1: // run mode
			{
				if(GetKey(olc::Key::SPACE).bPressed)
				{
					gameState = 0;
					return true;
				}
				updateGameBoard();
				// Draw
				for(int x = 0; x < 256; x++)
				{
					for(int y = 0; y < 240; y++)
					{
						// dead
						if(gameBoard[x][y] == 0)
						{
							Draw(x, y, olc::WHITE);
						}
						// alive cell
						else
						{
							Draw(x, y, olc::BLACK);
						}
					}	
				}
				return true;
			}
			default: // useless
				return true;
		}
	}
	void updateGameBoard()
	{
		for(int x = 0; x < 256; x++)
		{
			for(int y = 0; y < 240; y++)
			{
				int count = getSurroundedBy(x, y);
				if(gameBoard[x][y] == 0)
				{
					if(count == 3)
						newGameBoard[x][y] = 1;
					else
						newGameBoard[x][y] = 0;
				}
				else
				{
					if(count < 2 || count > 3)
						newGameBoard[x][y] = 0;
					else
						newGameBoard[x][y] = 1;
				}
			}
		}
		copyArray();
	}
	int getSurroundedBy(int x, int y)
	{
		if(x == 0 || y == 0 || x == 256 || y == 240)
		{
			return 0;
		}
		else
		{
			int count = 0;
			for(int i = -1; i <= 1; i++)
			{
				for(int j = -1; j <= 1; j++)
				{
					if(gameBoard[x + i][y + j] == 1) count++;
				}
			}
			if (gameBoard[x][y] == 1) count--;
			return count;
		}
	}
	void copyArray()
	{
			for(int i = 0; i < 256; i++)
			{
				for(int j = 0; j < 240; j++)
				{
					gameBoard[i][j] = newGameBoard[i][j];
				}
			}
	}
private:
	int newGameBoard[256][240];
	int gameBoard[256][240];
	int gameState = 0;
};


int main()
{
	Example demo;
	if (demo.Construct(256, 240, 4, 4, false, true))
		demo.Start();

	return 0;
}

