#include "game/GameLogicInterface.h"

#include "engine/AnimatedSprite.h"

#include <thread>
#include <memory>

#include <map>
#include <unordered_set>

// this stops the variables declared here from becoming globaly accessable
namespace {

	const float tileWidth = 3.0f / 150;
	const float tileHeight = 2.0f / 100;

	std::unordered_set<std::pair<int, int>> cells;

	bool paused = true;

	float camX = 0.0f;
	float camY = 0.0f;
	float zoom = 1.0f;
	float camSpeed = 0.01f;
}

void GameLogicInterface::init() {

	window.setResolution(1920, 1080);
	window.setTitle("Game of Life");
	
	srand(time(NULL));

	for (int i = 0; i < 700; i++) {
		cells.insert({ rand() % 100 - 50, rand() % 100 - 50 });
	}

}

void GameLogicInterface::update(float deltaTime)
{
	static unsigned int tCount = 0;
	tCount++;
	
	if (!paused)
		applyRules();

	glClearColor(0.6f, 0.6f, 0.62f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	static BatchQuads quads;
	quads.clear();
	for (auto& cell : cells) {
		float x = (cell.first * tileWidth + camX) * zoom;
		float y = (cell.second * tileHeight + camY) * zoom;

		// if not on screen then dont render
		if (x > ViewportManager::getRightViewportBound() || x < ViewportManager::getLeftViewportBound()) {
			continue;
		}
		if (y > ViewportManager::getTopViewportBound() || y < ViewportManager::getBottomViewportBound()) {
			continue;
		}

		quads.addQuad(x, y, tileWidth * zoom, tileHeight * zoom, 1, 1, 1, 1);
	}
	quads.renderAll();

	if (paused) {
		float dx = (window.getMouseX() / zoom) - camX;
		float dy = (window.getMouseY() / zoom) - camY;
		int x = roundf(dx / tileWidth);
		int y = roundf(dy / tileHeight);

		Geo::Rectangle::fillRect((x * tileWidth + camX) * zoom, (y * tileHeight + camY) * zoom, tileWidth * zoom, tileHeight * zoom, 0, 1, 0, 1);

	}

	// camera controls
	if (window.keyIsDown(GLFW_KEY_W)) camY -= camSpeed / zoom;
	if (window.keyIsDown(GLFW_KEY_A)) camX += camSpeed / zoom;
	if (window.keyIsDown(GLFW_KEY_S)) camY += camSpeed / zoom;
	if (window.keyIsDown(GLFW_KEY_D)) camX -= camSpeed / zoom;

	if (window.keyIsDown(GLFW_KEY_Q)) zoom *= 0.98f;
	if (window.keyIsDown(GLFW_KEY_E)) zoom *= 1.02f;

}

void GameLogicInterface::cleanup()
{
}

void GameLogicInterface::mouseMoveCallback(double xPos, double yPos)
{

}

void GameLogicInterface::mouseButtonCallback(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		float dx = (window.getMouseX() / zoom) - camX;
		float dy = (window.getMouseY() / zoom) - camY;
		int tileX = roundf(dx / tileWidth);
		int tileY = roundf(dy / tileHeight);

		auto p = cells.find({ tileX, tileY });
		if (p == cells.end()) {
			cells.insert({ tileX, tileY });
		}
		else {
			cells.erase(p);
		}
	}
	
}

void GameLogicInterface::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		paused = !paused;

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
		applyRules();

	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS && paused) {
		cells.clear();
	}
}

void GameLogicInterface::characterCallback(unsigned int codepoint)
{
}


bool GameLogicInterface::getTile(int currentX, int currentY, int deltaX, int deltaY, std::unordered_set<std::pair<int, int>>& tiles)
{
	return tiles.find({ currentX + deltaX, currentY + deltaY }) != tiles.end();
}

int GameLogicInterface::countNeighbours(int x, int y, std::unordered_set<std::pair<int, int>>& cells)
{
	int neighborCount = 0;
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (!(dx == 0 && dy == 0)) {
				if (getTile(x, y, dx, dy, cells)) {
					neighborCount++;
				}
			}
		}
	}
	
	return neighborCount;
}

void GameLogicInterface::applyRules()
{
	std::list<std::array<int, 2>> newSet;

	for (auto cellIt = cells.begin(); cellIt != cells.end(); ++cellIt) {
		std::pair<int, int> cell = *cellIt;

		//int neighborCount = countNeighbours(cell[0], cell[1], cells);
		int neighborCount = countNeighbours(cell.first, cell.second, cells);

		// any dead cell with three live neighbors becomes a live cell
		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				if (!(dx == 0 && dy == 0)) {

					int nCount = countNeighbours(cell.first + dx, cell.second + dy, cells);
					if (nCount == 3) {
						newSet.push_back({ cell.first + dx, cell.second + dy });
					}

				}
			}
		}
		
		// any live cell with two or three live neighbors survives
		if (neighborCount == 2 || neighborCount == 3) {
			newSet.push_back({ cell.first, cell.second });
			continue;
		}

		// all other live cells die, dead cells stay dead
		// just not pushing other cells into list

	}

	cells.clear();
	for (auto& c : newSet) {
		cells.insert({ c[0], c[1] });
	}

}
