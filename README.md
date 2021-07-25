# game-of-life
This program is a recreation of John Conway's game of life simulation. This basic implementaion is written entirely in C++ and makes use of the free-to-use GLFW and GLEW libraries to handle windowing and linking to OpenGL.

user guide:
Upon launching, the 150x100 board is filled randomly with living cells and the simulation begins. The user can pause the simulation at any time by presssing the spacebar. While the simulation is paused, the user can see a green square over any cell that they hover their cursor over. The highlighted cell will toggle between living and non-living upon being left-clicked. The user can also press backspace while the simulation is paused to clear the board completely. The simulation can be resumed by pressing the spacebar.
