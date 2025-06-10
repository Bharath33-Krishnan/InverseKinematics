#include <raylib/raylib.h>
#include <glm/glm.hpp>

#include "./IK_Solver/Debug.h"

bool IK::Debug::print_new_line_after_log = true;

#include "./IK_Solver/IK_Algo_FABRIK.h"
#include "./IK_Solver/IK_Solver.h"

int screenWidth = 800;
int screenHeight = 600;
float unitLength = 25.0f; // Each link is 1 unit in logic, scaled to 50px in screen
float num_links = 25.0f; // Each link is 1 unit in logic, scaled to 50px in screen

// Converts glm::vec2 to Raylib's Vector2
inline Vector2 ToRaylibVec(const glm::vec2& v) {
    return { v.x * unitLength + screenWidth / 2.0f, screenHeight / 2.0f - v.y * unitLength };
}

// Converts screen mouse position to glm world position (centered, scaled)
inline glm::vec2 MousePosToGLM() {
    Vector2 m = GetMousePosition();
    return glm::vec2(
        (m.x - screenWidth / 2.0f) / unitLength,
        (screenHeight / 2.0f - m.y) / unitLength
    );
}

int main() {
    InitWindow(screenWidth, screenHeight, "FABRIK IK Visualizer - Raylib + GLM");
    SetTargetFPS(60);

    using VecT = glm::vec2;
	IK::IK_Algo<VecT>* fabrik = new IK::IK_Algo_FABRIK<VecT>();
    IK::IK_Solver<VecT> solver(num_links, fabrik);

    VecT base(0.0f, 0.0f);
    for (int i = 0; i < num_links; ++i)
        solver.addLink(base + VecT(i * 1.0f, 0.0f), base + VecT((i + 1) * 1.0f, 0.0f));

    while (!WindowShouldClose()) {
        VecT target = MousePosToGLM();
        solver.solve(base, target);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        const auto& links = solver.getLinksVector();
        for (int i = 0; i < num_links; ++i) {
            auto* link = links[i].get();
            DrawLineEx(
                ToRaylibVec(link->getStartPos()),
                ToRaylibVec(link->getEndPos()),
                4.0f,
                DARKBLUE
            );
        }

        DrawCircleV(GetMousePosition(), 6, RED);
        DrawText("FABRIK Inverse Kinematics", 10, 10, 20, GRAY);
        DrawText("Move mouse to change end effector target", 10, 40, 10, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
