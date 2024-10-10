#include "game.h"
#include <iostream>
#include <raylib.h>

// Local objects
Ball ball;
ObstacleBlocks obstacles;
int framesCounter = 0;
const int maxTime = 1800; // 30 seconds = 1800 frames at 60FPS



Ball::Ball() {
    reset();
}

void Ball::reset() {
    x = 100;  // Set ball's initial x position
    y = 350;  // Set ball's initial y position to sit on the platform
    speedY = 0;
    radius = 30;
    isJumping = false;
    isGameOver = false;
    hasWon = false;
}

void Ball::balldraw() {
    DrawCircle(x, y, radius, RED);
}

void Ball::ballupdate(int screen_height, float groundHeight) {
    if (isGameOver || hasWon) return;

    const float gravity = 0.5;
    const float jumpStrength = -12.f;

    if (y + radius < groundHeight) {
        speedY += gravity;
    }
    else {
        speedY = 0;
        y = groundHeight - radius;
        isJumping = false;
    }

    if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_SPACE)) && !isJumping) {
        speedY = jumpStrength;
        isJumping = true;
    }

    y += speedY;
}

bool Ball::checkWinCondition(int screen_width) {
    if (x >= screen_width) {
        hasWon = true;
        return true;
    }
    return false;
}

ObstacleBlocks::ObstacleBlocks() {
    reset();
}

void ObstacleBlocks::reset() {
    ground = { 0, 400, 1280, 320 };
    obstacleSpeed = 6;

    obstacles[0] = { 1600, 380, 100, 600 };  // Hole 1 (ground level)
    obstacles[1] = { 1200, 300, 40, 100 };   // Block 1
}

void ObstacleBlocks::loadTextures() {
    groundTexture = LoadTexture("Graphics/TILES.png");
    blockTexture = LoadTexture("Graphics/boxblock5.png");
    holeTexture = LoadTexture("Graphics/Transparen.png");
    backgroundTexture = LoadTexture("Graphics/backgroundball.png");
}

void ObstacleBlocks::drawBlocks() {
    DrawTexture(backgroundTexture, 0, -80, WHITE);
    DrawTexture(groundTexture, 0, 400, WHITE);
    DrawTexture(holeTexture, obstacles[0].x, obstacles[0].y, WHITE);
    DrawTexture(blockTexture, obstacles[1].x, obstacles[1].y, WHITE);
}

void ObstacleBlocks::updateBlocks() {
    for (int i = 0; i < 6; i++) {
        obstacles[i].x -= obstacleSpeed;
        if (obstacles[i].x + obstacles[i].width < 0) {
            obstacles[i].x = 1180 + GetRandomValue(450, 2000); // Reset position off-screen
        }
    }
}

bool ObstacleBlocks::checkCollisionWithBall(Ball& ball) {
    if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, ground)) {
        ball.y = ground.y - ball.radius - 24;
        ball.speedY = 0;
        ball.isJumping = false;
        return false;
    }

    if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, obstacles[1])) {
        ball.isGameOver = true;
        return true;
    }

    return false;
}

bool ObstacleBlocks::checkFallIntoHole(Ball& ball) {
    return CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, obstacles[0]);
}

bool ObstacleBlocks::checkBallBelowGround(Ball& ball) {
    return (ball.y + ball.radius > ground.y);
}

void ObstacleBlocks::unloadTextures() {
    UnloadTexture(groundTexture);
    UnloadTexture(blockTexture);
    UnloadTexture(holeTexture);
    UnloadTexture(backgroundTexture);
}




ObstacleBlocks Class;

// Game initialization function
void GameInitialize() {
    obstacles.loadTextures();
    ball.reset();
    obstacles.reset();
    framesCounter = 0;
}

// Game update function
void GameUpdate() {
    if (ball.isGameOver || ball.hasWon) {
        if (IsKeyPressed(KEY_ENTER)) {
            ball.reset();
            obstacles.reset();
            framesCounter = 0; // Reset timer
        }
    }
    else {
        // Update game difficulty
        if (framesCounter >= 600) {
            obstacles.obstacleSpeed = 10; // Increase speed after 10 seconds
        }
        if (framesCounter >= 1200) {
            obstacles.obstacleSpeed = 15; // Increase speed after 20 seconds
        }

        ball.ballupdate(VSCREEN_HEIGHT, obstacles.ground.y);
        obstacles.updateBlocks();

        // Collision checks
        if (obstacles.checkCollisionWithBall(ball)) {
            if (obstacles.checkBallBelowGround(ball)) {
                ball.isGameOver = true; // Game over due to collision below ground
            }
        }

        if (obstacles.checkFallIntoHole(ball)) {
            ball.isGameOver = true; // Game over due to falling into a hole
        }

        framesCounter++;
        if (framesCounter >= maxTime) {
            ball.hasWon = true; // Win condition after 30 seconds
        }
    }
}

// Game draw function
void GameDraw() {
    
    ClearBackground(RAYWHITE);

    if (ball.isGameOver) {
        DrawText("GAME OVER", VSCREEN_WIDTH / 2 - 150, VSCREEN_HEIGHT / 2 - 30, 50, RED);
        DrawText("Press ENTER to Restart", VSCREEN_WIDTH / 2 - 200, VSCREEN_HEIGHT / 2 + 30, 30, WHITE);
    }
    else if (ball.hasWon) {
        DrawText("YOU WIN!", VSCREEN_WIDTH / 2 - 150, VSCREEN_HEIGHT / 2 - 30, 50, GREEN);
        DrawText("Press ENTER to Restart", VSCREEN_WIDTH / 2 - 200, VSCREEN_HEIGHT / 2 + 30, 30, WHITE);
    }
    else {
        obstacles.drawBlocks();
        ball.balldraw();
        DrawText(TextFormat("Time: %02i", framesCounter / 60), 20, 20, 20, BLACK);
    }

    
}

// Game deinitialization function
void GameDeInitialize() {
    obstacles.unloadTextures();
    CloseWindow();
}
