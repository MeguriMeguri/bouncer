#pragma once

#include <raylib.h>



#define MAX(a, b) ((a)>(b)? (a) : (b)) //Used for virtual screen
#define MIN(a, b) ((a)<(b)? (a) : (b))

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define VSCREEN_WIDTH 320
#define VSCREEN_HEIGHT 180


// Class declarations for Ball and ObstacleBlocks
class Ball {
public:
    float x, y;
    float speedY;
    int radius;
    bool isJumping;
    bool isGameOver;
    bool hasWon;

    Ball();
    void reset();
    void balldraw();
    void ballupdate(int screen_height, float groundHeight);
    bool checkWinCondition(int screen_width);
};

class ObstacleBlocks {
public:
    Rectangle obstacles[6];
    Rectangle ground;
    float obstacleSpeed;
    Texture2D groundTexture;
    Texture2D blockTexture;
    Texture2D holeTexture;
    Texture2D backgroundTexture;

    ObstacleBlocks();
    void reset();
    void loadTextures();
    void drawBlocks();
    void updateBlocks();
    bool checkCollisionWithBall(Ball& ball);
    bool checkFallIntoHole(Ball& ball);
    bool checkBallBelowGround(Ball& ball);
    void unloadTextures();
};

// Function declarations
void GameInitialize();
void GameUpdate();
void GameDraw();
void GameDeInitialize();
