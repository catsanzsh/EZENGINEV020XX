// Minimal Peach's Castle Scene in C++ (raylib)
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <cmath>

// Simple block structure
struct Block {
    Vector3 position;
    Vector3 scale;
    Color color;
};

// Player structure
struct Player {
    Vector3 position;
    Vector3 velocity;
    float speed;
    float jumpPower;
    bool grounded;
    bool gravityEnabled;
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Peach's Castle - B3313 Test Map");
    Camera camera = { 0 };
    camera.position = {0.0f, 5.0f, -15.0f};
    camera.target = {0.0f, 0.5f, 4.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    Player player = {{0, 0.5f, 4}, {0,0,0}, 6.0f, 8.0f, true, false};

    // Generate Castle Blocks
    std::vector<Block> blocks;

    // Courtyard
    blocks.push_back({{0, -0.5f, 0}, {30, 1, 30}, GREEN});

    // Castle base front wall
    for (int x = -4; x <= 4; x++)
        for (int y = 0; y < 5; y++)
            blocks.push_back({{(float)x, (float)y, 5}, {1,1,1}, LIGHTGRAY});

    // Castle sides
    for (int z = 4; z <= 8; z++)
        for (int y = 0; y < 5; y++) {
            blocks.push_back({{-4, (float)y, (float)z}, {1,1,1}, GRAY});
            blocks.push_back({{4, (float)y, (float)z}, {1,1,1}, GRAY});
        }

    // Towers
    for (int y = 5; y < 8; y++) {
        blocks.push_back({{-4, (float)y, 8}, {1,1,1}, GRAY});
        blocks.push_back({{4, (float)y, 8}, {1,1,1}, GRAY});
    }

    // Castle door
    blocks.push_back({{0, 0, 4}, {2,2,0.2f}, BROWN});

    // Floating star block
    blocks.push_back({{0, 2, 6}, {0.7f,0.7f,0.7f}, YELLOW});

    while (!WindowShouldClose()) {
        // Player movement
        Vector3 moveDir = {0};
        if (IsKeyDown(KEY_W)) moveDir.z += 1;
        if (IsKeyDown(KEY_S)) moveDir.z -= 1;
        if (IsKeyDown(KEY_A)) moveDir.x += 1;
        if (IsKeyDown(KEY_D)) moveDir.x -= 1;

        // Normalize direction
        if (Vector3Length(moveDir) > 0)
            moveDir = Vector3Normalize(moveDir);

        player.position.x += moveDir.x * player.speed * GetFrameTime();
        player.position.z += moveDir.z * player.speed * GetFrameTime();

        // Gravity toggle
        if (IsKeyPressed(KEY_G)) player.gravityEnabled = !player.gravityEnabled;

        // Simple gravity & jumping
        if (player.gravityEnabled) {
            player.velocity.y -= 20 * GetFrameTime();
            player.position.y += player.velocity.y * GetFrameTime();

            if (player.position.y <= 0.5f) {
                player.position.y = 0.5f;
                player.velocity.y = 0;
                player.grounded = true;
            }
        }

        if (IsKeyPressed(KEY_SPACE) && player.grounded && player.gravityEnabled) {
            player.velocity.y = player.jumpPower;
            player.grounded = false;
        }

        // Camera smoothly follows player
        camera.target = Vector3Lerp(camera.target, player.position, 4 * GetFrameTime());
        camera.position = Vector3Add(camera.target, {0,5,-15});

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);
        // Draw Blocks
        for (auto &b : blocks)
            DrawCube(b.position, b.scale.x, b.scale.y, b.scale.z, b.color);

        // Draw player
        DrawCube(player.position, 0.5f, 1.0f, 0.5f, WHITE);

        EndMode3D();

        DrawText("Press G to toggle gravity. WASD to move. SPACE to jump.", 10, 10, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
