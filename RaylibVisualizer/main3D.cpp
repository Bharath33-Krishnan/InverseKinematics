#include <raylib/raylib.h>
#include <raylib/raymath.h>
#include <glm/glm.hpp>

#include "./IK_Solver/Debug.h"
bool IK::Debug::print_new_line_after_log = true;

#include "IK_Solver/IK_Solver.h"
#include "IK_Solver/IK_Algo_FABRIK.h"
#include "IK_Solver/Link.h"

// Converts glm::vec3 to Raylib Vector3
inline Vector3 ToRaylibVec(const glm::vec3& v) {
    return { v.x, v.y, v.z };
}

// Converts mouse X/Z to a 3D glm::vec3 target at fixed Y level
inline glm::vec3 MouseXZToWorld3D(Camera camera) {
    Vector2 mousePos = GetMousePosition();
    Ray ray = GetMouseRay(mousePos, camera);
    // Intersect with y = 0 plane
    float t = -ray.position.y / ray.direction.y;
    Vector3 point = Vector3Add(ray.position, Vector3Scale(ray.direction, t));
    return glm::vec3(point.x, 0.0f, point.z);
}

float num_links = 25.0f; // Each link is 1 unit in logic, scaled to 50px in screen

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "FABRIK IK Solver 3D - Raylib + GLM");

    Camera3D camera = { 0 };
    camera.position = { 10.0f, 10.0f, 10.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    using VecT = glm::vec3;
	IK::IK_Algo<VecT>* fabrik = new IK::IK_Algo_FABRIK<VecT>();
    IK::IK_Solver<VecT> solver(num_links, fabrik);

    // Create chain starting at origin along +X axis
    VecT base = { 0.0f, 0.0f, 0.0f };
    for (int i = 0; i < num_links; ++i)
        solver.addLink(base + VecT(i * 1.0f, 0.0f, 0.0f), base + VecT((i + 1) * 1.0f, 0.0f, 0.0f));

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        glm::vec3 target = MouseXZToWorld3D(camera);
        solver.solve(base, target);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);

        // Draw ground
        DrawGrid(20, 1.0f);

        // Draw chain
        const auto& links = solver.getLinksVector();
        for (int i = 0; i < num_links; ++i) {
            auto* link = links[i].get();
            DrawLine3D(ToRaylibVec(link->getStartPos()), ToRaylibVec(link->getEndPos()), DARKBLUE);
            DrawSphere(ToRaylibVec(link->getStartPos()), 0.1f, SKYBLUE);
        }

        // Target marker
        DrawSphere(ToRaylibVec(target), 0.15f, RED);

        EndMode3D();
        DrawText("FABRIK IK 3D - Move mouse to target | WASD + Mouse to move camera", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
