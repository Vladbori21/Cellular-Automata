#include "Game.h"


Grid::Grid() : rule() {
	grid.resize(Screen::GRID_HEIGHT);
	for (int i = 0; i < Screen::GRID_HEIGHT; i++) {
		grid[i].resize(Screen::GRID_WIDTH);
	}
}


int Grid::CountNeighbours(int row, int col) {
	int neighbours = 0;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {

			if (i == 0 && j == 0) continue;

			int nx = col + j;
			int ny = row + i;

			if (ny >= 0 && ny < grid.size() && nx >= 0 && nx < grid[ny].size()) {
				neighbours += grid[ny][nx].state;
			}

		}
	}

	return neighbours;
}


void Grid::update() {
	std::vector<std::vector<Cell>> newGrid(grid);

	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[i].size(); j++) {
			int neighbours = CountNeighbours(i, j);

			if (grid[i][j].state == 0) {
				if (rule.birth[neighbours]) {
					newGrid[i][j].state = 1;
					newGrid[i][j].age = 0;
				}
				else {
					if(grid[i][j].age < Cell::maxAge) newGrid[i][j].age = ++grid[i][j].age;
				}
			}
			else if (grid[i][j].state == 1) {
				newGrid[i][j].state = rule.survive[neighbours];
			}
		}
	}

	grid.swap(newGrid);
}


void Grid::setRule(Rule& r) {
	rule = r;
}


void Grid::clear() {
	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[i].size(); j++) {
			grid[i][j].state = 0;
		}
	}
}


bool Grid::getState(int row, int col) const {
	return grid[row][col].state;
}


Cell Grid::getCell(int row, int col) const {
	return grid[row][col];
}


void Grid::setState(int row, int col, bool state) {
	if (row >= 0 && row < Screen::GRID_HEIGHT && col >= 0 && col < Screen::GRID_WIDTH) {
		grid[row][col].state = state;
	}
}


void Grid::setAge(int row, int col, int age) {
	if (row >= 0 && row < Screen::GRID_HEIGHT && col >= 0 && col < Screen::GRID_WIDTH && age >= 0) {
		grid[row][col].age = age;
	}
}


Game::Game() : window(nullptr), renderer(nullptr), paused(false), running(true), fog(false) {
	if (!init()) running = false;
}


Game::~Game() {
	cleanup();
}


bool Game::init() {

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL initialisation failed: %s", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("Cellular Automata", Screen::WINDOW_WIDTH, Screen::WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (!window) {
		SDL_Log("Window creation failed: %s", SDL_GetError());
		cleanup();
		return false;
	}

	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) {
		SDL_Log("Renderer creation failed: %s", SDL_GetError());
		cleanup();
		return false;
	}

	if (!SDL_SetRenderLogicalPresentation(renderer, Screen::LOGICAL_WIDTH, Screen::LOGICAL_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
		SDL_Log("Failed to set logical size: %s", SDL_GetError());
		cleanup();
		return false;
	}

	if (!SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND)) {
		SDL_Log("Failed to set Blend mod: %s", SDL_GetError());
		cleanup();
		return false;
	}

	return true;
}


void Game::cleanup() {
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	SDL_Quit();
}


void Game::handleInput() {

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			running = false;
		}
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
			grid.setState(event.button.y / Screen::GRID_SIZE, event.button.x / Screen::GRID_SIZE, true);
		}
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_RIGHT) {
			grid.setState(event.button.y / Screen::GRID_SIZE, event.button.x / Screen::GRID_SIZE, false);
		}

		if (event.type == SDL_EVENT_KEY_DOWN && event.key.repeat == 0) {

			if (event.key.key == SDLK_C) {
				grid.clear();
			}

			if (event.key.key == SDLK_SPACE) {
				paused = !paused;
			}

		}
	}
}


void Game::update() {
	if (paused) return;

	Uint64 frameStart = SDL_GetTicks();

	grid.update();

	Uint64 frameEnd = SDL_GetTicks();
	Uint64 deltaTime = frameEnd - frameStart;

	if (deltaTime < frameDuration) {
		SDL_Delay(frameDuration - deltaTime);
	}
}


SDL_Color Game::calculateColor(int maxAge, int age, Color c) {
	if (c == RED) {
		return {static_cast<Uint8>(fog ? (255 - 255 / maxAge * age) : 0), 0,0,255};
	}
	else if (c == GREEN) {
		return { 0, static_cast<Uint8>(fog ? (255 - 255 / maxAge * age) : 0),0,255 };
	}
	else if (c == BLUE) {
		return { 0, 0,static_cast<Uint8>(fog ? (255 - 255 / maxAge * age) : 0),255 };
	}

	return{ 0,0,0,255 };
}


void Game::SDL_DrawColor(SDL_Color color){
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}


void Game::render() {

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	for (int i = 0; i < Screen::GRID_HEIGHT; i++) {
		for (int j = 0; j < Screen::GRID_WIDTH; j++) {

			SDL_FRect rect = { Screen::GRID_SIZE * j, Screen::GRID_SIZE * i, Screen::GRID_SIZE, Screen::GRID_SIZE };

			Cell cell = grid.getCell(i, j);

			if (cell.state == 1) {
				SDL_SetRenderDrawColor(renderer, color == RED ? 255 : 0
					, color == GREEN ? 255 : 0
					, color == BLUE ? 255 : 0
					, 255);
			}
			else {
				
				SDL_DrawColor(calculateColor(Cell::maxAge, cell.age, color));
			}
			SDL_RenderFillRect(renderer, &rect);
		}
	}

	SDL_RenderPresent(renderer);

}


void Game::run() {
	inputConfig();

	while (running) {
		handleInput();
		update();
		render();
	}
}


void Game::inputConfig() {
	SDL_Log("Enter rule:\nB-amount of neighbors where the cell born\n"
			"S-amount of neighbors where the cell survives\n"
	 "Example: Conway's Game of Life is B3S23\n");
	std::string rule;

	do {
		std::cin >> rule;

	} while (!ReadRule(rule));

	SDL_Log("Enter color: 1-Red 2-Green 3-Blue\n");
	int color;

	do {
		std::cin >> color;

	} while (!ReadColor(color));

	SDL_Log("Fog effect: 1-Yes 2-No\n");
	int fogInput;

	do {
		std::cin >> fogInput;
		if (fogInput == 1) fog = true;

	} while (fogInput != 1 && fogInput != 2);
}


bool Game::ReadRule(std::string& ruleStr) {
	int i = 0;
	Rule rule;

	while (i < ruleStr.size()) {

		if (ruleStr[i] == 'B') {
			i++;

			while (i < ruleStr.size() && isdigit(ruleStr[i])) {
				rule.birth[ruleStr[i] - '0'] = true;
				i++;
			}
		}
		else if (ruleStr[i] == 'S') {
			i++;

			while (i < ruleStr.size() && isdigit(ruleStr[i])) {
				rule.survive[ruleStr[i] - '0'] = true;
				i++;
			}
		}
		else return false;

	}

	grid.setRule(rule);
	return true;
}


bool Game::ReadColor(int c) {
	switch (c) {
	case 1:
		color = RED;
		break;
	case 2:
		color = GREEN;
		break;
	case 3:
		color = BLUE;
		break;
	default:
		return false;
	}

	return true;
}

