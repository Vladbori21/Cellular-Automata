#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <iostream>


namespace Screen {
	constexpr int WINDOW_WIDTH = 2200;
	constexpr int WINDOW_HEIGHT = 1400;
	constexpr int LOGICAL_WIDTH = WINDOW_WIDTH;
	constexpr int LOGICAL_HEIGHT = WINDOW_HEIGHT;
	constexpr int GRID_SIZE = 10;
	constexpr int GRID_WIDTH = LOGICAL_WIDTH / GRID_SIZE - 2;
	constexpr int GRID_HEIGHT = LOGICAL_HEIGHT / GRID_SIZE - 2;
}


enum Color{RED, BLUE, GREEN};


struct Cell {
	static const int maxAge = 4;
	bool state;
	int age = maxAge;
};


struct Rule {
	bool birth[9] = {};
	bool survive[9] = {};
};


class Grid {
private:
	std::vector<std::vector<Cell>> grid;
	Rule rule;

	int CountNeighbours(int row, int col);

public:
	Grid();
	void update();
	void setRule(Rule& r);
	void clear();
	bool getState(int row, int col) const;
	Cell getCell(int row, int col) const;
	void setState(int row, int col, bool state);
	void setAge(int row, int col, int age);
};


class Game {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Grid grid;
	Uint32  frameRate = 5;
	float frameDuration = 1000.0f / frameRate;
	bool paused;
	bool running;
	bool fog;
	Color color;
	SDL_Color calculateColor(int maxAge, int age, Color c);
	void SDL_DrawColor(SDL_Color color);


public:
	Game();
	~Game();
	void render();
	bool init();
	void run();
	void handleInput();
	void update();
	void cleanup();
	bool ReadRule(std::string& ruleStr);
	bool ReadColor(int c);
	void inputConfig();
};

