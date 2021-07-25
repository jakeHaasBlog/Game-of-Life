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

#include <array>
#include <unordered_set>
#include <algorithm>

#define tilesWide 150
#define tilesTall 100

namespace std {
	template <> struct std::hash<std::pair<int, int>> {
		inline size_t operator()(const std::pair<int, int>& i) const {
			int a = i.first;
			int b = i.second;
			int A = a >= 0 ? 2 * a : -2 * a - 1;
			int B = b >= 0 ? 2 * b : -2 * b - 1;
			int C = (A >= B ? A * A + A + B : A + B * B) / 2;
			a < 0 && b < 0 || a >= 0 && b >= 0 ? C : -C - 1;
			
			return C;
		}
	};
}

namespace GameLogicInterface {
	void init();
	void update(float deltaTime);
	void cleanup();

	void mouseMoveCallback(double xPos, double yPos);
	void mouseButtonCallback(int button, int action, int mods);
	void keyCallback(int key, int scancode, int action, int mods);
	void characterCallback(unsigned int codepoint);

	bool getTile(int currentX, int currentY, int deltaX, int deltaY, std::unordered_set<std::pair<int, int>>& tiles);
	int countNeighbours(int x, int y, std::unordered_set<std::pair<int, int>>& cells);
	void applyRules();
};