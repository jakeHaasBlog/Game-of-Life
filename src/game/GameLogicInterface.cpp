#include "game/GameLogicInterface.h"

#include "engine/AnimatedSprite.h"

#include <thread>
#include <memory>

// this stops the variables declared here from becoming globaly accessable
namespace {

	const float tilesX = -1.5f;
	const float tilesY = -1.0f;
	const float tileWidth = 3.0f / tilesWide;
	const float tileHeight = 2.0f / tilesTall;

	std::array<bool, tilesWide* tilesTall> tiles[2];

	bool paused = false;
}

void GameLogicInterface::init() {

	window.setResolution(1920, 1080);
	window.setTitle("Game of Life");
	
	srand(time(NULL));

	for (bool& b : tiles[0])
		b = rand() % 2;

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
	for (int i = 0; i < tiles[0].size(); i++) {
		int x = i % tilesWide;
		int y = i / tilesWide;

		if (tiles[0][i]) {
			//Geo::Rectangle::fillRect(tilesX + x * tileWidth, tilesY + y * tileHeight, tileWidth, tileHeight, 1, 1, 1, 1);
			quads.addQuad(tilesX + x * tileWidth, tilesY + y * tileHeight, tileWidth, tileHeight, 1, 1, 1, 1);
		}
		else {
			//Geo::Rectangle::fillRect(tilesX + x * tileWidth, tilesY + y * tileHeight, tileWidth, tileHeight, 0, 0, 0, 1);
			quads.addQuad(tilesX + x * tileWidth, tilesY + y * tileHeight, tileWidth, tileHeight, 0, 0, 0, 1);
		}
	}
	quads.renderAll();

	if (paused) {
		float dx = window.getMouseX() - tilesX;
		float dy = window.getMouseY() - tilesY;
		int x = dx / tileWidth;
		int y = dy / tileHeight;

		Geo::Rectangle::fillRect(tilesX + x * tileWidth, tilesY + y * tileHeight, tileWidth, tileHeight, 0, 1, 0, 1);

	}

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
		float dx = window.getMouseX() - tilesX;
		float dy = window.getMouseY() - tilesY;
		int tileX = dx / tileWidth;
		int tileY = dy / tileHeight;

		int index = tileX + tileY * tilesWide;

		if (index > 0 && index < tiles[0].size()) {
			tiles[0][index] = !tiles[0][index];
		}
	}
	
}

void GameLogicInterface::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		paused = !paused;

	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS && paused) {
		for (bool& b : tiles[0]) {
			b = false;
		}
	}
}

void GameLogicInterface::characterCallback(unsigned int codepoint)
{
}


int GameLogicInterface::getTile(int current, int deltaX, int deltaY, std::array<bool, tilesWide* tilesTall>* tiles)
{
	int x = current % tilesWide;
	int y = current / tilesWide;
	x += deltaX;
	y += deltaY;

	int index = x + y * tilesWide;
	if (x < 0 || x >= tilesWide) return -1;
	if (y < 0 || y >= tilesTall) return -1;

	return index;
}

int GameLogicInterface::countNeighbours(int index, std::array<bool, tilesWide * tilesTall>* tiles)
{
	int neighborCount = 0;
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (!(dx == 0 && dy == 0)) {
				int dIndex = getTile(index, dx, dy, tiles);
				if (dIndex != -1) {
					if ((*tiles)[dIndex]) {
						neighborCount++;
					}
				}
			}
		}
	}
	
	return neighborCount;
}

void GameLogicInterface::applyRules()
{

	for (int i = 0; i < tiles[0].size(); i++) {
		tiles[1] = tiles[0];
	}


	for (int i = 0; i < tiles[0].size(); i++) {
		int neighborCount = countNeighbours(i, tiles + 1);
		
		// any live cell with two or three live neighbors survives
		if ((tiles[1])[i]) {
			if (neighborCount == 2 || neighborCount == 3) {
				tiles[0][i] = true;
				continue;
			}
		}

		// any dead cell with three live neighbors becomes a live cell
		else {
			if (neighborCount == 3) {
				tiles[0][i] = true;
				continue;
			}
		}

		// all other live cells die, dead cells stay dead
		(tiles[0])[i] = false;
	}

}
