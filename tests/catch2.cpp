#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <cmath>
#include "threepp/threepp.hpp"

using namespace threepp;

TEST_CASE("Cube behavior", "[Cube]") {
int screenWidth = 800;
int screenHeight = 600;

// Create a cube
Cube cube;

SECTION("Initial state") {
// Verify initial state of the cube
REQUIRE(cube.scene_ != nullptr);
REQUIRE(cube.mesh_ != nullptr);
REQUIRE(cube.roundCounter_ == 0);
REQUIRE(cube.isInsideLoop_ == false);
REQUIRE(cube.isDragging_ == false);
}

SECTION("Mouse interaction") {
// Simulate mouse interaction
double deltaTime = 0.016; // Example delta time value
double mouseX = 400; // Example mouse X coordinate
double mouseY = 300; // Example mouse Y coordinate

// Simulate mouse click inside the cube
cube.startDrag();
cube.update(deltaTime, mouseX, mouseY, screenWidth, screenHeight);

// Verify the cube is being dragged
REQUIRE(cube.isDragging_ == true);

// Simulate mouse release
cube.stopDrag();
cube.update(deltaTime, mouseX, mouseY, screenWidth, screenHeight);

// Verify the cube is no longer being dragged
REQUIRE(cube.isDragging_ == false);
}

SECTION("Loop collision") {
// Simulate cube positions inside and outside the loop
cube.mesh_->position().set(0, 0, 0); // Position inside the loop
REQUIRE(cube.checkLoopCollision() == true);

cube.mesh_->position().set(5, 0, 0); // Position outside the loop
REQUIRE(cube.checkLoopCollision() == false);
}
}
