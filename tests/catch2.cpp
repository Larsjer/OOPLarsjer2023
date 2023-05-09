#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <cmath>
#include "threepp/threepp.hpp"

using namespace threepp;

// Define a small threshold for floating point comparison
const double EPSILON = 1e-6;

// Unit test for Cube class
TEST_CASE("Cube class tests", "[cube]") {
// Create a cube
Cube cube;

// Test initial position
Vector3 initialPos = cube.mesh_->position();
REQUIRE(initialPos.x() == Approx(0.0).epsilon(EPSILON));
REQUIRE(initialPos.y() == Approx(0.0).epsilon(EPSILON));
REQUIRE(initialPos.z() == Approx(0.0).epsilon(EPSILON));

// Test update function
double deltaTime = 0.016;  // 60 FPS
double mouseX = 400;
double mouseY = 300;
int screenWidth = 800;
int screenHeight = 600;
cube.update(deltaTime, mouseX, mouseY, screenWidth, screenHeight);
Vector3 newPos = cube.mesh_->position();
REQUIRE(newPos.x() != Approx(initialPos.x()).epsilon(EPSILON));
REQUIRE(newPos.y() != Approx(initialPos.y()).epsilon(EPSILON));
REQUIRE(newPos.z() != Approx(initialPos.z()).epsilon(EPSILON));
}
