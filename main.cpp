#include <iostream>
#include <cmath>
#include "threepp/threepp.hpp"
#include "threepp/geometries/geometries.hpp"
#include "threepp/geometries/BoxGeometry.hpp"
#include "threepp/materials/MeshBasicMaterial.hpp"
#include "threepp/objects/Mesh.hpp"
#include "threepp/scenes/Scene.hpp"
#include "threepp/cameras/PerspectiveCamera.hpp"
#include "threepp/math/Color.hpp"
#include "threepp/materials/Material.hpp"




using namespace threepp;


// Function to calculate normalized device coordinates (NDC) from screen coordinates
Vector2 screenToNDC(float x, float y, float screenWidth, float screenHeight) {
    double ndcX = (x / screenWidth) * 2 - 1;
    double ndcY = -(y / screenHeight) * 2 + 1;
    return threepp::Vector2(static_cast<float>(ndcX), static_cast<float>(ndcY));

}

class Canvas {
private:
    int mouseX_;
    int mouseY_;
    bool mouseDown_; // Variable to store the mouse down state

public:
    Canvas() {
        mouseX_ = 0;
        mouseY_ = 0;
        mouseDown_ = false; // Initialize mouseDown_ to false
    }

    int getMouseX() const {
        return mouseX_;
    }

    int getMouseY() const {
        return mouseY_;
    }

    bool getMouseDown() const {
        return mouseDown_;
    }

    void setSize(int width, int height) {
        // Implementation of setSize function
        // ...
    }

    void animate(std::function<void(double)> callback) {
        // Implementation of animate function
        // ...
    }

    void render(Scene::Ptr scene, PerspectiveCamera::Ptr camera) {
        // Implementation of render function
        // ...
    }

    void onMouseDown(std::function<void(int, int, int)> callback) {
        // Implementation of onMouseDown function
        // ...
    }

    void onMouseUp(std::function<void(int, int, int)> callback) {
        // Implementation of onMouseUp function
        // ...
    }
};

class Ptr;// Cube class
class Cube {
public:
    using Ptr = std::shared_ptr<Cube>;

    Cube() {
        auto geometry_ = BoxGeometry::create(1, 1, 1);
        auto material_ = MeshBasicMaterial::create();
        auto mesh_ = Mesh::create(geometry_, material_);
        auto scene_ = Scene::create();
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
        // Get mouse cursor position
        Vector2 mouseNDC = screenToNDC(mouseX, mouseY, screenWidth, screenHeight);

        // Convert mouse cursor position into coordinates
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
    BoxGeometry::Ptr geometry_;
    MeshBasicMaterial::Ptr material_;
    Mesh::Ptr mesh_;
    Scene::Ptr scene_;
    PerspectiveCamera::Ptr camera_;

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
    threepp::Canvas canvas("MyCanvas");
    canvas.setSize(screenWidth, screenHeight);

    // Create a cube
    Cube cube;

    // Start rendering loop
    canvas.animate([&](double deltaTime) {
        cube.update(deltaTime, canvas.getMouseX(), canvas.getMouseY(), screenWidth, screenHeight);
        canvas.render(cube.scene_, cube.camera_);
    });

    // Mouse event handling
    canvas.onMouseDown([&](int button, int x, int y) {
        float normalizedX = static_cast<float>(x) / screenWidth * 2 - 1;
        float normalizedY = static_cast<float>(y) / screenHeight * -2 + 1;
        threepp::Raycaster raycaster;
        raycaster.setFromCamera(cube.camera_, normalizedX, normalizedY);
        std::vector<threepp::Intersection> intersects = raycaster.intersectObjects(cube.scene_->children());

        if (!intersects.empty()) {
            threepp::Intersection& intersection = intersects[0];
            intersection.object->material->color = threepp::Color::grey();
        }
    });

    canvas.onMouseDown([&](int button, int x, int y) {
        if (button == 0) { // Left mouse button
            cube.stopDrag();
        }
    });
    return 0;
}
