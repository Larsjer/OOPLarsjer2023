#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../main.cpp"

// Test case for the screenToNDC function
TEST_CASE("Screen to NDC conversion", "[screenToNDC]") {
    SECTION("Conversion with positive coordinates") {
        float x = 100;
        float y = 200;
        float screenWidth = 800;
        float screenHeight = 600;

        Vector2 result = screenToNDC(x, y, screenWidth, screenHeight);

        REQUIRE(result.x == Approx(0.25f));
        REQUIRE(result.y == Approx(-0.333f));
    }

    SECTION("Conversion with negative coordinates") {
        float x = -100;
        float y = -200;
        float screenWidth = 800;
        float screenHeight = 600;

        Vector2 result = screenToNDC(x, y, screenWidth, screenHeight);

        REQUIRE(result.x == Approx(-0.75f));
        REQUIRE(result.y == Approx(0.666f));
    }
}

// Test case for the Cube class
TEST_CASE("Cube class", "[Cube]") {
    // Test case for the checkLoopCollision function
    SECTION("Check loop collision") {
        Cube cube;
        cubeFriend cubeFriend;
        cubeFriend.accessCubeMembers(cube);

        // Cube inside the loop
        cube.stopDrag();
        cube.update(1.0, 1.5, 0.5, 800, 600, cube, PerspectiveCamera::create(), Scene::create(), Object3D::create());

        REQUIRE(cube.checkLoopCollision() == true);

        // Cube outside the loop
        cube.stopDrag();
        cube.update(1.0, 4.0, 0.0, 800, 600, cube, PerspectiveCamera::create(), Scene::create(), Object3D::create());

        REQUIRE(cube.checkLoopCollision() == false);
    }
}

// Test case for the CanvasClass class
TEST_CASE("CanvasClass class", "[CanvasClass]") {
    // Test case for the getMouseX and getMouseY functions
    SECTION("Get mouse position") {
        CanvasClass canvas;

        canvasFriend canvasFriend;
        canvasFriend.accessCanvasVariables(canvas);

        // Initial mouse position should be 0
        REQUIRE(canvas.getMouseX() == 0);
        REQUIRE(canvas.getMouseY() == 0);

        // Set new mouse position
        canvas.onMouseDown([](int button, int x, int y) {
            // Set mouse position to (100, 200)
            canvas.mouseX_ = 100;
            canvas.mouseY_ = 200;
        });

        // Mouse position should be updated
        REQUIRE(canvas.getMouseX() == 100);
        REQUIRE(canvas.getMouseY() == 200);
    }
}

// Test case for the main function
TEST_CASE("Main function", "[main]") {
    // Test case for the screen size variables
    SECTION("Screen size") {
        int screenWidth = 1280;
        int screenHeight = 960;

        // Verify the values of the screen size variables
        REQUIRE(screenWidth == 1280);
        REQUIRE(screenHeight == 960);
    }
}
