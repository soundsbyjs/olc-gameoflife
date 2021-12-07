#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
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
		for(int x = 0; x < 255; x++)
		{
			for(int y = 0; y < 255; y++)
			{
				gameBoard[x][y] = 0;
			}
		}
		gameState = 0;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		switch (gameState)
		{
			case 0:
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
			case 1:
			{
				for(int x = 0; x < 255; x++)
				{
					for(int y = 0; y < 255; y++)
					{
						// 1 is alive
						if(gameBoard[x][y] == 1) 
						{
							if(!dead(x, y))
							{
								Draw(x, y, olc::BLACK);
							}
							else
							{
								Draw(x, y, olc::WHITE);
								gameBoard[x][y] = 0;
							}
						// 0 is dead
						}
						else
						{
							// if it stays dead
							if(!alive(x, y))
								Draw(x, y, olc::WHITE);
							else
							{
								Draw(x, y, olc::BLACK);
								gameBoard[x][y] = 1;
							}
						}
					}
				}
			
			}
			default:
				return true;
		}
	}

	void getSurroundedBy(int x, int y)
	{
		if(x == 0 || y == 0 || x == 255 || y == 255)
		{
			for(int x2 = 0; x2 < 3; x2++)
			{
				for(int y2 = 0; y2 < 3; y2++)
				{
					surroundedBy[x2][y2] = 0;
				}
			}
		}
		else
		{
			for(int x2 = -1; x2 < 2; x2++)
			{
				for(int y2 = -1; y2 < 2; y2++)
				{
					surroundedBy[x2 + 1][y2 + 1] = gameBoard[x + x2][y + y2];
				}
			}
		}
	}

	bool dead(int x, int y)
	{
		getSurroundedBy(x, y);
		int count = 0;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				if(surroundedBy[i][j] == 1) count++;
			}
		}
		if (count <= 1) return true;
		else if (count >= 4) return true;
		else return false;
	}
	bool alive(int x, int y)
	{
		getSurroundedBy(x, y);
		int count = 0;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				if(surroundedBy[i][j] == 1) count++;
			}
		}
		if(count == 3) return true;
		else return false;
	}

private:
	int gameBoard[255][255];
	int gameState;
	int surroundedBy[3][3];
};


int main()
{
	Example demo;
	if (demo.Construct(255, 255, 2, 2, false, true))
		demo.Start();

	return 0;
}
