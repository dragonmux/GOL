#include <inttypes.h>
#include <GTK++.h>
#include <vector>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#define GOL_SIZE	200

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
	GTKCairoDrawingArea *image;
	GTKLabel *label;
	typedef gol<golWidth, golHeight> golBoard;
	uint32_t generation;

public:
	gol(GTKWindow *window, GTKCairoDrawingArea *img, GTKLabel *lbl) : image(img), label(lbl), generation(0)
	{
		for (uint32_t i = 0; i < golHeight; i++)
		{
			// Initialise the grid to "dead"
			golC.push_back(std::vector<bool>(golWidth));
			golN.push_back(std::vector<bool>(golWidth));
		}
		window->RegisterTimedCallback(1000, staticStep, this);
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

private:
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

typedef gol<GOL_SIZE, GOL_SIZE> golGrid;

inline void gliderGunH(golGrid *golBoard, uint32_t x, uint32_t y)
{
	golBoard->set(x + 24, y + 0, true);
	golBoard->set(x + 22, y + 1, true);
	golBoard->set(x + 24, y + 1, true);
	golBoard->set(x + 12, y + 2, true);
	golBoard->set(x + 13, y + 2, true);
	golBoard->set(x + 20, y + 2, true);
	golBoard->set(x + 21, y + 2, true);
	golBoard->set(x + 34, y + 2, true);
	golBoard->set(x + 35, y + 2, true);
	golBoard->set(x + 11, y + 3, true);
	golBoard->set(x + 15, y + 3, true);
	golBoard->set(x + 20, y + 3, true);
	golBoard->set(x + 21, y + 3, true);
	golBoard->set(x + 34, y + 3, true);
	golBoard->set(x + 35, y + 3, true);
	golBoard->set(x + 0, y + 4, true);
	golBoard->set(x + 1, y + 4, true);
	golBoard->set(x + 10, y + 4, true);
	golBoard->set(x + 16, y + 4, true);
	golBoard->set(x + 20, y + 4, true);
	golBoard->set(x + 21, y + 4, true);
	golBoard->set(x + 0, y + 5, true);
	golBoard->set(x + 1, y + 5, true);
	golBoard->set(x + 10, y + 5, true);
	golBoard->set(x + 14, y + 5, true);
	golBoard->set(x + 16, y + 5, true);
	golBoard->set(x + 17, y + 5, true);
	golBoard->set(x + 22, y + 5, true);
	golBoard->set(x + 24, y + 5, true);
	golBoard->set(x + 10, y + 6, true);
	golBoard->set(x + 16, y + 6, true);
	golBoard->set(x + 24, y + 6, true);
	golBoard->set(x + 11, y + 7, true);
	golBoard->set(x + 15, y + 7, true);
	golBoard->set(x + 12, y + 8, true);
	golBoard->set(x + 13, y + 8, true);
}

inline void gliderGunV(golGrid *golBoard, uint32_t x, uint32_t y)
{
	golBoard->set(x + 0, y + 24, true);
	golBoard->set(x + 1, y + 22, true);
	golBoard->set(x + 1, y + 24, true);
	golBoard->set(x + 2, y + 12, true);
	golBoard->set(x + 2, y + 13, true);
	golBoard->set(x + 2, y + 20, true);
	golBoard->set(x + 2, y + 21, true);
	golBoard->set(x + 2, y + 34, true);
	golBoard->set(x + 2, y + 35, true);
	golBoard->set(x + 3, y + 11, true);
	golBoard->set(x + 3, y + 15, true);
	golBoard->set(x + 3, y + 20, true);
	golBoard->set(x + 3, y + 21, true);
	golBoard->set(x + 3, y + 34, true);
	golBoard->set(x + 3, y + 35, true);
	golBoard->set(x + 4, y + 0, true);
	golBoard->set(x + 4, y + 1, true);
	golBoard->set(x + 4, y + 10, true);
	golBoard->set(x + 4, y + 16, true);
	golBoard->set(x + 4, y + 20, true);
	golBoard->set(x + 4, y + 21, true);
	golBoard->set(x + 5, y + 0, true);
	golBoard->set(x + 5, y + 1, true);
	golBoard->set(x + 5, y + 10, true);
	golBoard->set(x + 5, y + 14, true);
	golBoard->set(x + 5, y + 16, true);
	golBoard->set(x + 5, y + 17, true);
	golBoard->set(x + 5, y + 22, true);
	golBoard->set(x + 5, y + 24, true);
	golBoard->set(x + 6, y + 10, true);
	golBoard->set(x + 6, y + 16, true);
	golBoard->set(x + 6, y + 24, true);
	golBoard->set(x + 7, y + 11, true);
	golBoard->set(x + 7, y + 15, true);
	golBoard->set(x + 8, y + 12, true);
	golBoard->set(x + 8, y + 13, true);
}

inline void scrubber(golGrid *golBoard, uint32_t x, uint32_t y)
{
	golBoard->set(x + 4, y + 0, true);
	golBoard->set(x + 2, y + 1, true);
	golBoard->set(x + 3, y + 1, true);
	golBoard->set(x + 4, y + 1, true);
	golBoard->set(x + 1, y + 2, true);
	golBoard->set(x + 1, y + 3, true);
	golBoard->set(x + 4, y + 3, true);
	golBoard->set(x + 5, y + 3, true);
	golBoard->set(x + 6, y + 3, true);
	golBoard->set(x + 0, y + 4, true);
	golBoard->set(x + 1, y + 4, true);
	golBoard->set(x + 3, y + 4, true);
	golBoard->set(x + 7, y + 4, true);
	golBoard->set(x + 3, y + 5, true);
	golBoard->set(x + 7, y + 5, true);
	golBoard->set(x + 3, y + 6, true);
	golBoard->set(x + 7, y + 6, true);
	golBoard->set(x + 9, y + 6, true);
	golBoard->set(x + 10, y + 6, true);
	golBoard->set(x + 4, y + 7, true);
	golBoard->set(x + 5, y + 7, true);
	golBoard->set(x + 6, y + 7, true);
	golBoard->set(x + 9, y + 7, true);
	golBoard->set(x + 9, y + 8, true);
	golBoard->set(x + 6, y + 9, true);
	golBoard->set(x + 7, y + 9, true);
	golBoard->set(x + 8, y + 9, true);
	golBoard->set(x + 6, y + 10, true);
}

int main(int argc, char **argv)
{
	golGrid *golBoard;
	GTKWindow *window;
	GTKCairoDrawingArea *image;
	GTKLabel *generations;
	GTKVBox *vBox;
	char *label;

	GTK::GTKInit(argc, argv);

	window = new GTKWindow(GTK_WINDOW_TOPLEVEL);
	window->SetLocation(GTK_WIN_POS_CENTER);
	window->SetTitle("Game Of Life - C++ Simulator");
	window->SetResizable(TRUE);
	window->SetBorder(5);
	vBox = new GTKVBox(false, 0);
	image = new GTKCairoDrawingArea(GOL_SIZE * 2, GOL_SIZE * 2);
	vBox->AddChild(image);
	generations = new GTKLabel();
	vBox->AddChild(generations);
	window->AddChild(vBox);
	window->ShowWindow();

	golBoard = new golGrid(window, image, generations);
	label = formatGenerations(golBoard->getGeneration());
	generations->SetText(label);
	delete [] label;

	gliderGunH(golBoard, 110, 110);
	gliderGunH(golBoard, 150, 200);
	gliderGunH(golBoard, 200, 200);
	gliderGunV(golBoard, 110, 150);
	scrubber(golBoard, 250, 110);
	golBoard->forceDraw();

	window->DoMessageLoop();
	delete golBoard;
	delete window;

	return 0;
}
