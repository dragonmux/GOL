/*
 * This file is part of GOL
 * Copyright © 2013 Rachel Mant (dx-mon@users.sourceforge.net)
 *
 * GOL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GOL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <inttypes.h>
#include <GTK++.h>
#include <vector>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#define GOL_WIDTH	800
#define GOL_HEIGHT	400

static const GTKCairoColour black = {0, 0, 0};
static const GTKCairoColour white = {1, 1, 1};

inline char *strFormatString(const char *format, ...)
{
	int len;
	char *ret;
	va_list args;
	va_start(args, format);
	len = vsnprintf(NULL, 0, format, args);
	va_end(args);
	ret = new char[len + 1]();
	va_start(args, format);
	vsprintf(ret, format, args);
	va_end(args);
	return ret;
}

inline char *formatGenerations(uint32_t generation)
{
	return strFormatString("Generation: %u", generation);
}

template <uint32_t golWidth, uint32_t golHeight> class gol
{
private:
	std::vector<std::vector<bool> > golC, golN;
	GTKWindow *window;
	GTKCairoDrawingArea *image;
	GTKLabel *label;
	typedef gol<golWidth, golHeight> golBoard;
	uint32_t generation;
	uint32_t callbackID;

public:
	gol(GTKWindow *wnd, GTKCairoDrawingArea *img, GTKLabel *lbl) : window(wnd), image(img), label(lbl), generation(0)
	{
		for (uint32_t i = 0; i < golHeight; i++)
		{
			// Initialise the grid to "dead"
			golC.push_back(std::vector<bool>(golWidth));
			golN.push_back(std::vector<bool>(golWidth));
		}
		registerTimer();
	}

	~gol()
	{
		image = NULL;
		golC.clear();
		golN.clear();
	}

	void set(uint32_t x, uint32_t y, bool alive)
	{
		assert(y < golHeight);
		assert(x < golWidth);
		golC[y][x] = alive;
	}

	void forceDraw()
	{
		plot();
	}

	uint32_t getGeneration() const
	{
		return generation;
	}

#include "gliderGun.h"
#include "scrubber.h"
#include "P49GliderShuttle.h"
#include "breeder2.h"
#include "tenEngineCordership.h"
#include "sevenEngineCordership.h"
#include "threeEngineCordership.h"
#include "diamondRing.h"
#include "harbour.h"
#include "ecologist.h"
#include "puffer2.h"

	static void doStep(GtkWidget *widget, void *data)
	{
		doStop(widget, data);
		golBoard::staticStep(data);
	}

	static void doStart(GtkWidget *widget, void *data)
	{
		golBoard *board = (golBoard *)data;
		if (board->callbackID == 0)
			board->registerTimer();
	}

	static void doStop(GtkWidget *widget, void *data)
	{
		golBoard *board = (golBoard *)data;
		if (board->callbackID != 0)
			board->unregisterTimer();
	}

private:
	void unregisterTimer()
	{
		window->UnregisterTimedCallback(callbackID);
		callbackID = 0;
	}

	void registerTimer()
	{
		callbackID = window->RegisterTimedCallback(10, staticStep, this);
	}

	uint8_t neighbourhood(uint32_t x, uint32_t y)
	{
		uint8_t size = 0;
		if (x > 0)
		{
			if (y > 0 && golC[y - 1][x - 1])
				size++;
			if (y < golHeight - 1 && golC[y + 1][x - 1])
				size++;
			if (golC[y][x - 1])
				size++;
		}
		if (x < golWidth - 1)
		{
			if (y > 0 && golC[y - 1][x + 1])
				size++;
			if (y < golHeight - 1 && golC[y + 1][x + 1])
				size++;
			if (golC[y][x + 1])
				size++;
		}
		if (y > 0 && golC[y - 1][x])
			size++;
		if (y < golHeight - 1 && golC[y + 1][x])
			size++;
		return size;
	}

	void eval()
	{
		for (uint32_t y = 0; y < golHeight; y++)
		{
			for (uint32_t x = 0; x < golWidth; x++)
			{
				uint8_t size = neighbourhood(x, y);
				if (golC[y][x])
				{
					if (size < 2)
						golN[y][x] = false;
					else if (size < 4)
						golN[y][x] = true;
					else
						golN[y][x] = false;
				}
				else
				{
					if (size == 3)
						golN[y][x] = true;
					else
						golN[y][x] = false;
				}
			}
		}
	}

	void update()
	{
		for (uint32_t y = 0; y < golHeight; y++)
		{
			for (uint32_t x = 0; x < golWidth; x++)
				golC[y][x] = golN[y][x];
		}
	}

	void plot()
	{
		char *generations;
		for (uint32_t y = 0; y < golHeight; y++)
		{
			for (uint32_t x = 0; x < golWidth; x++)
			{
				if (golC[y][x] != golN[y][x] || generation <= 1)
					image->DrawRect(x << 1, y << 1, 2, 2, (golN[y][x] ? black : white));
			}
		}
		image->FinishDrawing();
		generations = formatGenerations(generation);
		label->SetText(generations);
		delete [] generations;
	}

	void step()
	{
		eval();
		generation++;
		plot();
		update();
	}

	/*!
	 * The sole existence of this function is to thunk from a GTK++ callback
	 * back over to our lovely C++ template class instance
	 */
	static bool staticStep(void *data)
	{
		golBoard *board = (golBoard *)data;
		board->step();
		return true;
	}
};

typedef gol<GOL_WIDTH, GOL_HEIGHT> golGrid;

int main(int argc, char **argv)
{
	golGrid *golBoard;
	GTKWindow *window;
	GTKCairoDrawingArea *image;
	GTKLabel *generations;
	GTKVBox *vBox;
	GTKHBox *hBox;
	GTKButton *btnStep, *btnStart, *btnStop;
	char *label;

	GTK::GTKInit(argc, argv);

	window = new GTKWindow(GTK_WINDOW_TOPLEVEL);
	window->SetLocation(GTK_WIN_POS_CENTER);
	window->SetTitle("Game Of Life - C++ Simulator");
	window->SetResizable(TRUE);
	window->SetBorder(5);
	vBox = new GTKVBox(false, 0);
	image = new GTKCairoDrawingArea(GOL_WIDTH * 2, GOL_HEIGHT * 2);
	vBox->AddChild(image);
	hBox = new GTKHBox(true, 0);
	generations = new GTKLabel();
	//vBox->AddChild(generations);
	hBox->AddChild(generations);
	btnStart = new GTKButton("Start");
	btnStop = new GTKButton("Stop");
	btnStep = new GTKButton("Step");
	hBox->AddChild(btnStart);
	hBox->AddChild(btnStop);
	hBox->AddChild(btnStep);
	vBox->AddChild(hBox);
	window->AddChild(vBox);
	window->ShowWindow();

	golBoard = new golGrid(window, image, generations);
	btnStart->SetOnClicked((void *)golGrid::doStart, golBoard);
	btnStop->SetOnClicked((void *)golGrid::doStop, golBoard);
	btnStep->SetOnClicked((void *)golGrid::doStep, golBoard);
	label = formatGenerations(golBoard->getGeneration());
	generations->SetText(label);
	delete [] label;

	//golBoard->gliderGunH(310, 110);
	//golBoard->gliderGunH(350, 200);
	// Don't enable this one if diamondRing is enabled below
	//golBoard->gliderGunH(400, 200);
	//golBoard->gliderGunV(310, 150);
	//golBoard->scrubber(450, 110);
	//golBoard->P49GliderShuttle(450, 110);
	//golBoard->P49GliderShuttle(110, 110);
	//golBoard->P49GliderShuttle(300, 110);
	//golBoard->P49GliderShuttle(635, 110);
	//golBoard->breeder2(0, 200);
	//golBoard->tenEngineCordership(GOL_WIDTH - 99, GOL_HEIGHT - 99);
	//golBoard->sevenEngineCordership(GOL_WIDTH - 99, GOL_HEIGHT - 99);
	//golBoard->threeEngineCordership(GOL_WIDTH - 99, GOL_HEIGHT - 99);
	//golBoard->diamondRing(394, 194);
	//golBoard->harbour(10, 10);
	//golBoard->ecologist(GOL_WIDTH - 50, 200);
	//golBoard->puffer2V(400, GOL_HEIGHT - 10);
	golBoard->puffer2H(GOL_WIDTH - 10, 200);
	golBoard->forceDraw();

	window->DoMessageLoop();
	delete golBoard;
	delete window;

	return 0;
}
