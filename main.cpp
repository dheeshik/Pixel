/*******************************************************************************************
 *
 *   raylib [core] example - Smooth Pixel-perfect camera
 *
 *   Example originally created with raylib 3.7, last time updated with
 *raylib 4.0
 *
 *   Example contributed by Giancamillo Alessandroni (@NotManyIdeasDev) and
 *   reviewed by Ramon Santamaria (@raysan5)
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an
 *OSI-certified, BSD-like license that allows static linking with closed source
 *software
 *
 *   Copyright (c) 2021-2024 Giancamillo Alessandroni (@NotManyIdeasDev) and
 *Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "include/raylib.h"

#include <cmath>
#include <math.h> // Required for: sinf(), cosf()

void DrawTile(Vector2 position, int tilex, int tiley, Texture2D tileset);
void DrawTileset(Texture2D tileset);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 600;
  const int screenHeight = 540;

  const int virtualScreenWidth = 160;
  const int virtualScreenHeight = 144;

  const float virtualRatio = (float)screenWidth / (float)virtualScreenWidth;

  InitWindow(screenWidth, screenHeight,
             "raylib [core] example - smooth pixel-perfect camera");

  Camera2D worldSpaceCamera = {0}; // Game world camera
  worldSpaceCamera.zoom = 0.5f;

  Camera2D screenSpaceCamera = {0}; // Smoothing camera
  screenSpaceCamera.zoom = 1.0f;

  RenderTexture2D target = LoadRenderTexture(
      virtualScreenWidth,
      virtualScreenHeight); // This is where we'll draw all our objects.

  Texture2D grassTiles = LoadTexture("assets/tileset/TX Tileset Grass.png");
  Texture2D wallTiles = LoadTexture("assets/tileset/TX Tileset Wall.png");

  Rectangle rec01 = {70.0f, 35.0f, 20.0f, 20.0f};
  Rectangle rec02 = {90.0f, 55.0f, 30.0f, 10.0f};
  Rectangle rec03 = {80.0f, 65.0f, 15.0f, 25.0f};

  // The target's height is flipped (in the source Rectangle), due to OpenGL
  // reasons
  Rectangle sourceRec = {0.0f, 0.0f, (float)target.texture.width,
                         -(float)target.texture.height};
  Rectangle destRec = {-virtualRatio, -virtualRatio,
                       screenWidth + (virtualRatio * 2),
                       screenHeight + (virtualRatio * 2)};

  Vector2 origin = {0.0f, 0.0f};

  float rotation = 0.0f;

  float cameraX = 0.0f;
  float cameraY = 0.0f;

  Vector2 playerPos = {0.f, 0.f};

  SetTargetFPS(0);

  //--------------------------------------------------------------------------------------
  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    rotation +=
        60.0f * GetFrameTime(); // Rotate the rectangles, 60 degrees per second

    // Make the camera move to demonstrate the effect
    if (IsKeyDown(KEY_RIGHT))
      playerPos.x += 2.0f;
    if (IsKeyDown(KEY_LEFT))
      playerPos.x -= 2.0f;
    if (IsKeyDown(KEY_UP))
      playerPos.y -= 2.0f;
    if (IsKeyDown(KEY_DOWN))
      playerPos.y += 2.0f;

    // Set the camera's target to the values computed above
    // screenSpaceCamera.target = (Vector2){cameraX, cameraY};
    screenSpaceCamera.target = playerPos;

    // Round worldSpace coordinates, keep decimals into screenSpace coordinates
    worldSpaceCamera.target.x = (int)screenSpaceCamera.target.x;
    screenSpaceCamera.target.x -= worldSpaceCamera.target.x;
    screenSpaceCamera.target.x *= virtualRatio;

    worldSpaceCamera.target.y = (int)screenSpaceCamera.target.y;
    screenSpaceCamera.target.y -= worldSpaceCamera.target.y;
    screenSpaceCamera.target.y *= virtualRatio;
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginTextureMode(target);
    ClearBackground((Color){31, 16, 42, 255});

    BeginMode2D(worldSpaceCamera);
    DrawTileset(grassTiles);
    DrawTileset(wallTiles);
    DrawRectanglePro(rec01, origin, rotation, BLACK);
    DrawRectanglePro(rec02, origin, -rotation, RED);
    DrawRectanglePro(rec03, origin, rotation + 45.0f, BLUE);
    EndMode2D();
    EndTextureMode();

    BeginDrawing();
    ClearBackground(RED);

    BeginMode2D(screenSpaceCamera);
    DrawTexturePro(target.texture, sourceRec, destRec, origin, 0.0f, WHITE);
    EndMode2D();

    DrawText(TextFormat("Screen resolution: %ix%i", screenWidth, screenHeight),
             10, 10, 20, DARKBLUE);
    DrawText(TextFormat("World resolution: %ix%i", virtualScreenWidth,
                        virtualScreenHeight),
             10, 40, 20, DARKGREEN);
    DrawFPS(GetScreenWidth() - 95, 10);
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  UnloadRenderTexture(target); // Unload render texture
  UnloadTexture(grassTiles);

  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
void DrawTile(Vector2 position, int tilex, int tiley, Texture2D tileset) {
  DrawTextureRec(tileset, {tilex * 32.f, tiley * 32.f, 32.f, 32.f}, position,
                 (Color){255, 255, 255, 255});
}

void DrawTileset(Texture2D tileset) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      DrawTile({32.0f * i, 32.f * j}, i, j, tileset);
    }
  }
}
