/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include <random>

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> xDist(0, Graphics::ScreenWidth - 1);
	std::uniform_int_distribution<int> yDist(0, Graphics::ScreenHeight - 1);

	for (int i = 0; i < nP; i++)
	{
		painters[i] = Vec2(float(xDist(rng)), (float)yDist(rng));
		vel[i] = Vec2(float(pow(-1, rand() % 2)), float(pow(-1, rand() % 2))) * 2.5f;
	}
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	for (int k = 0; k < nP; k++)
	{
		painters[k] += vel[k];
	}
	PainterClamp();
}

void Game::PainterClamp()
{
	for (int k = 0; k < nP; k++)
	{
		if (painters[k].x >= Graphics::ScreenWidth)
		{
			painters[k].x = Graphics::ScreenWidth;
			vel[k].x *= -1.0f;
		}
		if (painters[k].x < 0)
		{
			painters[k].x = 0;
			vel[k].x *= -1.0f;
		}

		if (painters[k].y >= Graphics::ScreenHeight)
		{
			painters[k].y = Graphics::ScreenHeight;
			vel[k].y *= -1.0f;
		}
		if (painters[k].y < 0)
		{
			painters[k].y = 0;
			vel[k].y *= -1.0f;
		}
	}
}

void Game::ComposeFrame()
{
	for (int j = 0; j < Graphics::ScreenHeight; j++)
	{
		for (int i = 0; i < Graphics::ScreenWidth; i++)
		{
			float sum = 0.0f;
			for (int k = 0; k < nP; k++)
			{
				const float distSq = Vec2(float(i) - painters[k].x, float(j) - painters[k].y).GetLengthSq();
				if (distSq <= 90000.0f)
				{
					sum += 255.0f * float(pow(2.7182819, -distSq/6000.0f));
				}
			}
			sum = capping(255.0f, sum);

			if (sum < 128.0f)
			{
				gfx.PutPixel(i, j, 255 - 2 * char(sum), 1 * char(sum), char(sum/2));
			}
			else
			{
				gfx.PutPixel(i, j, char(sum/2), 255 - 2 * char(sum - 128), 1 * char(sum - 128));
			}
		}
	}
}
