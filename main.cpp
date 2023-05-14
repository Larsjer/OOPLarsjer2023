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
    return Vector2(static_cast<float>(ndcX), static_cast<float>(ndcY));

}

class CanvasClass {
private:
    friend class canvasFriend;
    int mouseX_;
    int mouseY_;
    bool mouseDown_;

public:

    CanvasClass() {
        mouseX_ = 0;
        mouseY_ = 0;
        mouseDown_ = false;
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

    void animate(std::function<void(double)> callback) {
        // Implementation of animate function
    }

    void render(int Scene, int Camera) {
        // Implementation of render function
    }

    void onMouseDown(std::function<void(int, int, int)> callback) {
        // Update mouseX_ and mouseY_ on mouse down event
        callback(0, mouseX_, mouseY_);
        mouseDown_ = true;
    }

    void onMouseUp(std::function<void(int, int, int)> callback) {
        // Implementation of onMouseUp function
    }
};



class canvasFriend{
public:
    void accessCanvasVariables(CanvasClass& CanvasClass){
        CanvasClass.mouseX_ = 0;
        CanvasClass.mouseY_ = 0;
    }
};

class Ptr;// Cube class
class Cube {
public:
    using Ptr = std::shared_ptr<Cube>;
    friend class cubeFriend;

    Cube() {


        auto geometry_ = BoxGeometry::create(1, 1, 1);
        auto material_ = MeshBasicMaterial::create();
        auto mesh_ = Mesh::create(geometry_, material_);

        // Set initial position and trailing position
        mesh_->position.set(0, 0, 0);
        trailingPos_ = mesh_->position.clone();
        initialPos_ = mesh_->position.clone();
        isDragging_ = false;
        isInsideLoop_ = false;
        roundCounter_ = 0;
    }

    // Update function to be called every frame
    void update(double deltaTime, double mouseX, double mouseY, int screenWidth, int screenHeight, Cube& cube, const PerspectiveCamera& camera, const Scene& scene, const Object3D& object3D) {
        // Get mouse cursor position
        Vector2 mouseNDC = screenToNDC(mouseX, mouseY, screenWidth, screenHeight);

        // Convert mouse cursor position into coordinates
        Raycaster raycaster;
        raycaster.setFromCamera(mouseNDC,  camera);
        auto intersects = raycaster.intersectObject(object3D, false);

        if (!intersects.empty()) {
            // Update the trailing position towards the cursor with a lerp-based approach
            Vector3 cursorPos = intersects[0].point;

            if (isDragging_) {
                trailingPos_.lerp(cursorPos, deltaTime * 3);  // Lerp for smooth trailing effect
                mesh_->position.copy(trailingPos_);
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

    friend class CubeTest;

private:
    // Trailing position of the cube
    Vector3 trailingPos_;
    Vector3 initialPos_;
    bool isDragging_;
    bool isInsideLoop_;
    int roundCounter_;

    bool checkLoopCollision() {
        // Check if the cube is inside the loop based on its position
        double x = mesh_->position.x;
        double y = mesh_->position.y;
        double radius = 3.0;

        double distance = std::sqrt(x * x + y * y);
        return distance <= radius;
    }
};

class cubeFriend{
public:
    void accessCubeMembers(Cube& cube) {
        // Access and modify private members of the Cube class
        cube.trailingPos_.x = 10;
        cube.isDragging_ = true;
        cube.checkLoopCollision() = true;

    }
};

int main() {
    //Make variables for screen size
    int screenWidth = 1280;
    int screenHeight = 960;

    auto object3d = Object3D::create();
    CanvasClass CanvasClass;
    Canvas canvas;
    canvas.setSize(WindowSize{screenWidth, screenHeight});
    auto scene_ = Scene::create();
    auto camera_ = PerspectiveCamera::create(75, canvas.getAspect(), 0.1f, 100);
    camera_->position.z = 5;


    //auto camera = PerspectiveCamera::create();
    //camera->position.z = 5;

    // Create a cube
    Cube cube;

    // Start rendering loop
    CanvasClass.animate([&](double deltaTime) {
        cube.update(deltaTime, CanvasClass.getMouseX(), CanvasClass.getMouseY(), screenWidth, screenHeight, cube, camera_, scene_, object3D);

        CanvasClass.render(*scene_, *camera_);
    });

    // Mouse event handling
    CanvasClass.onMouseDown([&](int button, int x, int y) {
        float normalizedX = static_cast<float>(x) / screenWidth * 2 - 1;
        float normalizedY = static_cast<float>(y) / screenHeight * -2 + 1;
        Vector2 setFromCamera(float x, float y);
        threepp::Raycaster raycaster;
        raycaster.setFromCamera(setFromCamera(normalizedX,normalizedY), camera_);
        std::vector<threepp::Intersection> intersects = raycaster.intersectObjects(*object3d, false);

        if (!intersects.empty()) {
            threepp::Intersection& intersection = intersects[0];
            intersection.object->material()->colorWrite = Color::green;
        }
    });

    CanvasClass.onMouseDown([&](int button, int x, int y) {
        if (button == 0) { // Left mouse button
            cube.stopDrag();
        }
    });
    return 0;
}
