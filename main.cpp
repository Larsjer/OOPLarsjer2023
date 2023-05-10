#include <iostream>
#include <cmath>
#include "threepp/threepp.hpp"

using namespace threepp;

// Function to calculate normalized device coordinates (NDC) from screen coordinates
Vector2 screenToNDC(double x, double y, double screenWidth, double screenHeight) {
    double ndcX = (x / screenWidth) * 2 - 1;
    double ndcY = -(y / screenHeight) * 2 + 1;
    return {ndcX, ndcY};
}

// Cube class
class Cube {
public:
    Cube() {
        // Create geometry and material
        geometry_ = BoxGeometry::create(1, 1, 1);
        material_ = MeshBasicMaterial::create(Color(0xff0000));

        // Create mesh and add to scene
        mesh_ = Mesh::create(geometry_, material_);
        scene_->add(mesh_);

        // Set initial position and trailing position
        mesh_->position().set(0, 0, 0);
        trailingPos_ = mesh_->position().clone();
        initialPos_ = mesh_->position().clone();
        isDragging_ = false;
        isInsideLoop_ = false;
        roundCounter_ = 0;
    }

    // Update function to be called every frame
    void update(double deltaTime, double mouseX, double mouseY, int screenWidth, int screenHeight) {
        // Get mouse cursor position in normalized device coordinates (NDC)
        Vector2 mouseNDC = screenToNDC(mouseX, mouseY, screenWidth, screenHeight);

        // Convert NDC to world coordinates
        Raycaster raycaster;
        raycaster.setFromCamera(mouseNDC, camera_);
        auto intersects = raycaster.intersectObjects({mesh_});

        if (!intersects.empty()) {
            // Update the trailing position towards the cursor with a lerp-based approach
            Vector3 cursorPos = intersects[0].point;

            if (isDragging_) {
                trailingPos_.lerp(cursorPos, deltaTime * 3);  // Lerp for smooth trailing effect
                mesh_->position().copy(trailingPos_);
            }

            // Check if cube is inside the loop
            if (!isInsideLoop_ && checkLoopCollision()) {
                isInsideLoop_ = true;
                roundCounter_++;
                std::cout << "Completed Round: " << roundCounter_ << std::endl;
            }
        } else {
            isDragging_ = false;
        }
    }

    void startDrag() {
        isDragging_ = true;
    }

    void stopDrag() {
        isDragging_ = false;
    }

private:
    // Three.js objects
    Geometry::Ptr geometry_;
    Material::Ptr material_;
    Mesh::Ptr mesh_;
    Scene::Ptr scene_ = Scene::create();
    PerspectiveCamera::Ptr camera_ = PerspectiveCamera::create(70, 1.0, 0.1, 1000);

    // Trailing position of the cube
    Vector3 trailingPos_;
    Vector3 initialPos_;
    bool isDragging_;
    bool isInsideLoop_;
    int roundCounter_;

    bool checkLoopCollision() {
        // Check if the cube is inside the loop based on its position
        double x = mesh_->position().x();
        double y = mesh_->position().y();
        double radius = 3.0;

        double distance = std::sqrt(x * x + y * y);
        return distance <= radius;
    }
};

int main() {
    // Create a canvas
    int screenWidth = 800;
    int screenHeight = 600;
    Canvas canvas(screenWidth, screenHeight);

    // Create a cube
    Cube cube;

    // Start rendering loop
    canvas.animate([&](double deltaTime) {
        cube.update(deltaTime, canvas.getMouseX(), canvas.getMouseY(), screenWidth, screenHeight);
        canvas.render(cube.scene_, cube.camera_);
    });

    // Mouse event handling
    canvas.onMouseDown([&](int button, int x, int y) {
        if (button == 0) { // Left mouse button
            Vector2 mouseNDC = screenToNDC(x, y, screenWidth, screenHeight);
            Raycaster raycaster;
            raycaster.setFromCamera(mouseNDC, cube.camera_);
            auto intersects = raycaster.intersectObjects({cube.mesh_});
            if (!intersects.empty()) {
                cube.startDrag();
            }
        }
    });

    canvas.onMouseUp([&](int button, int x, int y) {
        if (button == 0) { // Left mouse button
            cube.stopDrag();
        }
    });

    return 0;
}
