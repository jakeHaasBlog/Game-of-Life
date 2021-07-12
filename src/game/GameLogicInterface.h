#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "engine/Window.h"
#include "engine/Texture.h"
#include "engine/TexturedQuad.h"
#include "engine/Geometry.h"
#include "engine/BatchQuads.h"
#include "engine/BatchLines.h"
#include "engine/ViewportManager.h"
#include "engine/ExistentialDictionary.h"

#include <memory>
#include <thread>

#define tilesWide 150
#define tilesTall 100

namespace GameLogicInterface {
	void init();
	void update(float deltaTime);
	void cleanup();

	void mouseMoveCallback(double xPos, double yPos);
	void mouseButtonCallback(int button, int action, int mods);
	void keyCallback(int key, int scancode, int action, int mods);
	void characterCallback(unsigned int codepoint);

	int getTile(int current, int deltaX, int deltaY, std::array<bool, tilesWide* tilesTall>* tiles);
	int countNeighbours(int index, std::array<bool, tilesWide* tilesTall>* tiles);
	void applyRules();
};