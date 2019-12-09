#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "bitmaps.h"
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
#define BACKGROUND_WIDTH    9
#define BACKGROUND_HEIGHT   4
#define TILE_SIZE           16
// make an ArdBitmap instance that will use the given the screen buffer and dimensions
#define ARDBITMAP_SBUF arduboy.getBuffer()
#include <ArdBitmap.h>
ArdBitmap<WIDTH, HEIGHT> ardbitmap;


//Program variables
int ground = 32;
bool playerMoveEnable = true;
bool enemy0MoveEnable = true;
bool enemy1MoveEnable = true;
bool enemy2MoveEnable = true;
bool enemy3MoveEnable = true;
bool enemy4MoveEnable = true;
bool dog0MoveEnable = true;
bool dog1MoveEnable = true;
bool dog2MoveEnable = true;
bool dog3MoveEnable = true;
//Player position variables
int playerX = 64;
int playerY = 32;
//Health variables
float health = 100;
float enemyAttack = 0.5;
//enemy position variables
int enemySpawnFreq;
int enemySpawnRightX = 134;
int enemySpawnLeftX = -16;
int enemy0X = enemySpawnRightX;
int enemy0Y = ground;
int enemy1X = enemySpawnLeftX;
int enemy1Y = ground;
int enemy2X = enemySpawnRightX;
int enemy2Y = ground;
int enemy3X = enemySpawnLeftX;
int enemy3Y = ground;
int enemy4X = enemySpawnRightX;
int enemy4Y = ground;
int dog0X = enemySpawnRightX;
int dog0Y = ground + 8;
int dog1X = enemySpawnRightX;
int dog1Y = ground + 8;
int dog2X = enemySpawnRightX;
int dog2Y = ground + 8;
int dog3X = enemySpawnRightX;
int dog3Y = ground + 8;

//Background position variables
int bgX = 0;
int bgY = 0;
int spawnX = 0; //Used for spawning AND bg stuff
int levelProg = 0;//used to determine when to draw exit
int middleHighTileY = 16;
int middleLowTileY = 32;
int floorTileY = 48;
int ceilingTileX = 0;
int ceilingTileY = 0;

int playerSpeed = 2;
int gameState = 0;
int level = 1;

//Spawn points
int enemy0Spawn;
int enemy1Spawn;
int enemy2Spawn;
int enemy3Spawn;
int enemy4Spawn;
int dog0Spawn = -2000;
int dog1Spawn = -2000;
int dog2Spawn = -2000;
int dog3Spawn = -2000;

//Animation variables
int runAnimFrame = 0;
int runJumpAnimFrame = 0;
int jumpUpAnimFrame = 0;
int punchAnimFrame = 0;
int kickAnimFrame = 0;
int enemy0RunAnimFrame = 0;
int enemy1RunAnimFrame = 0;
int enemy2RunAnimFrame = 0;
int enemy3RunAnimFrame = 0;
int enemy4RunAnimFrame = 0;
int dog0RunAnimFrame = 0;
int dog1RunAnimFrame = 0;
int dog2RunAnimFrame = 0;
int dog3RunAnimFrame = 0;
bool fallingRight = false;
bool fallingLeft = false;
bool playerRunLeft = false;
bool playerRunRight = false;
bool playerFaceRight = true;
bool playerFlyKickRight = false;
bool playerFlyKickLeft = false;
bool playerKickRight = false;
bool playerKickLeft = false;
bool playerCrouchRight = false;
bool playerCrouchLeft = false;
bool playerCrouchPunchRight = false;
bool playerCrouchPunchLeft = false;
// Enemy existence variables
bool enemy0Exist = false;
bool enemy0Dying = false;
bool enemy1Exist = false;
bool enemy1Dying = false;
bool enemy2Exist = false;
bool enemy2Dying = false;
bool enemy3Exist = false;
bool enemy3Dying = false;
bool enemy4Exist = false;
bool enemy4Dying = false;
bool dog0Exist = false;
bool dog0Dying = false;
bool dog1Exist = false;
bool dog1Dying = false;
bool dog2Exist = false;
bool dog2Dying = false;
bool dog3Exist = false;
bool dog3Dying = false;

// Enemy animation variables
int enemy0DyingAnimFrame = 0;
int enemy1DyingAnimFrame = 0;
int enemy2DyingAnimFrame = 0;
int enemy3DyingAnimFrame = 0;
int enemy4DyingAnimFrame = 0;
int dog0DyingAnimFrame = 0;
int dog1DyingAnimFrame = 0;
int dog2DyingAnimFrame = 0;
int dog3DyingAnimFrame = 0;

//Collision detection variables
Rect playerRect;
Rect enemy0Rect;
Rect enemy1Rect;
Rect enemy2Rect;
Rect enemy3Rect;
Rect enemy4Rect;
Rect dog0Rect;
Rect dog1Rect;
Rect dog2Rect;
Rect dog3Rect;
Rect playerKickRect;

//Game state variables
#define GAME_TITLE        0
#define LEVEL_SCREEN      1
#define GAME_PLAY         2
#define GAME_OVER         3
#define GAME_HIGH         4
#define GAME_FINISH       5


void setup() 
{
  arduboy.setTextSize(1);
  //Serial.begin(9600);
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(20);
  arduboy.clear();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  if (!(arduboy.nextFrame()))
  {
    return;
  }
  arduboy.clear();

  switch(gameState)
  {
    case GAME_TITLE:
            //Title screen stuff
            Titlescreen();
            break;
    case LEVEL_SCREEN:
            //Level screen
            LevelScreen();
            break;
    case GAME_PLAY: 
            //Gameplay stuff
            GamePlay();
            break;
    case GAME_OVER: 
            //Game over screen
            GameOverScreen(false);
            break;
    case GAME_FINISH:
            //Game finished screen
            GameOverScreen(true);
   }
  //Serial.println(dog0X);
  arduboy.display();
}

void Titlescreen()
{ 
 ardbitmap.drawCompressed(WIDTH / 2, HEIGHT / 2, StickmenTitle, WHITE, ALIGN_CENTER, MIRROR_NONE);
  if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON))
  {
    arduboy.audio.on();
    sound.tones(Kung_Fu);
    gameState = LEVEL_SCREEN;
    delay(1000);
    ResetVariables();
  }
}

void DrawInitialScreen()
{
   //Draw initial screen
    for (int x = 0; x< 9; x++)
    {
      displayFloorTiles[x] = dungeonTileMap[30];
      displayMiddleLowTiles[x] = dungeonTileMap[11];
      displayMiddleHighTiles[x] = dungeonTileMap[11];
      displayCeilingTiles[x] = dungeonTileMap[30];
    }
}

void LevelScreen()
{
  arduboy.audio.on();
  if (level == 1)
  {
    arduboy.setCursor(45, 20);
    arduboy.print("Dungeon");
    enemy0Spawn = -20;
    enemy1Spawn = -90;
    enemy2Spawn = -142;
    enemy3Spawn = -228;
    enemy4Spawn = -314;
    enemySpawnFreq = 400;
    ResetVariables(); 
    DrawInitialScreen();
  }
    if (level == 2)
    {
      arduboy.setCursor(45, 20);
      arduboy.print("Garden");
      enemy0Spawn = -10;
      enemy1Spawn = -70;
      enemy2Spawn = -102;
      enemy3Spawn = -188;
      enemy4Spawn = -188;
      dog0Spawn -200;
      enemySpawnFreq = 300;
      ResetVariables();
      DrawInitialScreen();
    }

    if (level == 3)
    {
      arduboy.setCursor(30, 20);
      arduboy.print("First Floor");
      enemy0Spawn = -10;
      enemy1Spawn = -50;
      enemy2Spawn = -92;
      enemy3Spawn = -130;
      enemy4Spawn = -100;
      dog0Spawn = -100;
      dog1Spawn = -200;
      enemySpawnFreq = 250;
      ResetVariables();
      DrawInitialScreen();
    }

    if (level == 4)
    {
      arduboy.setCursor(30, 20);
      arduboy.print("Second Floor");
      enemy0Spawn = -10;
      enemy1Spawn = -25;
      enemy2Spawn = -50;
      enemy3Spawn = -100;
      enemy4Spawn = -100;
      dog0Spawn = -50;
      dog1Spawn = -100;
      dog2Spawn = -200;
      enemySpawnFreq = 150;
      ResetVariables();
      DrawInitialScreen();
    }

    if (level == 5)
    {
      arduboy.setCursor(45, 20);
      arduboy.print("Rooftop");
      enemy0Spawn = 0;
      enemy1Spawn = -10;
      enemy2Spawn = -20;
      enemy3Spawn = -30;
      enemy4Spawn = -40;
      dog0Spawn = -50;
      dog1Spawn = -100;
      dog2Spawn = -150;
      dog3Spawn = -250;
      enemySpawnFreq = 100;
      ResetVariables();
      DrawInitialScreen();
    }

    if (level == 6)
    {
      gameState = GAME_FINISH;
    }

    for (int i = 0; i < 9 ; i++)    //Copy levelTile array to the displayTile array for the DrawLevel() function
    {
      displayTiles[i] = levelTilePointer[level-1][i];
    }
  
  if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON))
  {
    gameState = GAME_PLAY;
    delay(1000);
  }
}

void ResetVariables()
{
  playerMoveEnable = true;
  enemy0MoveEnable = true;
  enemy1MoveEnable = true;
  enemy2MoveEnable = true;
  enemy3MoveEnable = true;
  enemy4MoveEnable = true;
  dog0MoveEnable = true;
  dog1MoveEnable = true;
  dog2MoveEnable = true;
  dog3MoveEnable = true;
  //Reset variables
  health = 100;
  enemy0X = enemySpawnRightX;
  enemy0Y = ground;
  enemy1X = enemySpawnLeftX;
  enemy1Y = ground;
  enemy2X = enemySpawnRightX;
  enemy2Y = ground;
  enemy3X = enemySpawnLeftX;
  enemy3Y = ground;
  enemy4X = enemySpawnRightX;
  enemy4Y = ground;
  dog0X = enemySpawnRightX;
  dog0Y = ground + 8;
  dog1X = enemySpawnLeftX;
  dog1Y = ground + 8;
  dog2X = enemySpawnRightX;
  dog2Y = ground + 8;
  dog3X = enemySpawnLeftX;
  dog3Y = ground + 8;
  bgX = 0;
  spawnX = 0;
  enemy0Exist = false;
  enemy1Exist = false;
  enemy2Exist = false;
  enemy3Exist = false;
  enemy4Exist = false;
  dog0Exist = false;
  dog1Exist = false;
  dog2Exist = false;
  dog3Exist = false;
  enemy0Rect = {0,0,0,0};
  enemy1Rect = {0,0,0,0};
  enemy2Rect = {0,0,0,0};
  enemy3Rect = {0,0,0,0};
  enemy4Rect = {0,0,0,0};
  dog0Rect = {0,0,0,0};
  dog1Rect = {0,0,0,0};
  dog2Rect = {0,0,0,0};
  dog3Rect = {0,0,0,0};
  levelProg = 0;
}

void HealthBar()
{
  arduboy.fillRect(0, 0,(health*WIDTH/100), 2); //Health bar drawn on top of screen according to health percentage
  if (health < 0)
  {
    gameState = GAME_OVER;
    sound.tones(GameOver);
  }
}

void GameOverScreen(bool finished)
{
  if (finished == false)
  {
    arduboy.setCursor(30, 20);
    arduboy.print("Game Over!");
  }
  else 
  {
     ardbitmap.drawCompressed(WIDTH / 2, HEIGHT / 2, End_Screen, WHITE, ALIGN_CENTER, MIRROR_NONE);
  }
  sound.tones(GameOver);
  delay(1000);
  arduboy.audio.off();
  if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON))
  {
    level = 1;
    gameState = GAME_TITLE;
    delay(300);
  }    
}

void GamePlay()
{
  Movement();
  DrawPlayer();
  ScreenLimit();
  DrawLevel();
  EnemyControl();
  CollisionDetection();
  SpawnControl();
  HealthBar();
  LevelControl();
  //Serial.println(playerMoveEnable);
}

void LevelControl()
{
  if (playerX == 92 && spawnX == -1024)   //When player walks enough to the right, Level changes to next level.
  {
    level++;
    gameState = LEVEL_SCREEN;
  }
}

void SpawnControl()
{
  //This function handles all the enemy spawning triggers
  if (level > 0)
  {
    if (enemy0Exist == false && spawnX < enemy0Spawn)
    {
      enemy0Exist = true;
    }
    
    if (enemy1Exist == false && spawnX < enemy1Spawn)
    {
      enemy1Exist = true;
    }
    
    if (enemy2Exist == false && spawnX < enemy2Spawn)
    {
      enemy2Exist = true;
    }
    
    if (enemy3Exist == false && spawnX < enemy3Spawn)
    {
      enemy3Exist = true;
    }
    
    if (enemy4Exist == false && spawnX < enemy4Spawn)
    {
      enemy4Exist = true;
    }
    
    if (dog0Exist == false && spawnX < dog0Spawn)
    {
      dog0Exist = true;
    }
    if (dog1Exist == false && spawnX < dog1Spawn)
    {
      dog1Exist = true;
    }
    if (dog2Exist == false && spawnX < dog2Spawn)
    {
      dog2Exist = true;
    }
    if (dog3Exist == false && spawnX < dog3Spawn)
    {
      dog3Exist = true;
    }
  }
}



void EnemyControl()
{
  //Enemy 0 Controls
  if (enemy0Exist == true)    //Check for existence
  {  
    enemy0Rect = {enemy0X, enemy0Y-9, 4, 24};      //Create collision rect
    if (enemy0MoveEnable == true)
    {
      ardbitmap.drawCompressed(enemy0X, enemy0Y, runAnim[enemy0RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);    //Draw enemy
      enemy0RunAnimFrame++;
      enemy0X -= playerSpeed;//Runs left
      if (enemy0RunAnimFrame > 7)
      {
        enemy0RunAnimFrame = 0;
      }
    }
  }
  //Enemy 1 controls
  if (enemy1Exist == true)    //Check for existence
  {
    enemy1Rect = {enemy1X-4, enemy1Y-9, 4, 24};//Create collision rect
    if (enemy1MoveEnable == true)
    {
      ardbitmap.drawCompressed(enemy1X, enemy1Y, runAnim[enemy1RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE);    //Draw enemy
      enemy1RunAnimFrame++;
      enemy1X += playerSpeed;//Runs Right
      if (enemy1RunAnimFrame > 7)
      {
        enemy1RunAnimFrame = 0;
      }
    }
  }
  //Enemy 2 controls
  if (enemy2Exist == true)    //Check for existence
  {
    enemy2Rect = {enemy2X, enemy2Y-9, 4, 24};      //Create collision rect
    if (enemy2MoveEnable == true)
    {
      ardbitmap.drawCompressed(enemy2X, enemy2Y, runAnim[enemy2RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);    //Draw enemy
      enemy2RunAnimFrame++;
      enemy2X -= playerSpeed;//Runs Left
      if (enemy2RunAnimFrame > 7)
      {
        enemy2RunAnimFrame = 0;
      }
    }
  }
  //Enemy 3 controls
  if (enemy3Exist == true)    //Check for existence
  {
    enemy3Rect = {enemy3X-4, enemy3Y-9, 4, 24};      //Create collision rect
    if (enemy3MoveEnable == true)
    {
      ardbitmap.drawCompressed(enemy3X, enemy3Y, runAnim[enemy3RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE);    //Draw enemy
      enemy3RunAnimFrame++;
      enemy3X += playerSpeed;//Runs Right
      if (enemy3RunAnimFrame > 7)
      {
        enemy3RunAnimFrame = 0;
      }
    }
  }
  //Enemy 4 controls
  if (enemy4Exist == true)    //Check for existence
  {
    enemy4Rect = {enemy4X, enemy4Y-9, 4, 24};      //Create collision rect
    if (enemy4MoveEnable == true)
    {
      ardbitmap.drawCompressed(enemy4X, enemy4Y, runAnim[enemy4RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);    //Draw enemy
      enemy4RunAnimFrame++;
      enemy4X -= playerSpeed;//Runs left
      if (enemy4RunAnimFrame > 7)
      {
        enemy4RunAnimFrame = 0;
      }
    }
  }

  //Enemy 0 death animation
  if (enemy0Dying == true)//Checks to see if enemy is dying
  {
    sound.tone(175, 25);
    if (playerX < enemy0X)
    {
      ardbitmap.drawCompressed(enemy0X, enemy0Y, dyingAnim[enemy0DyingAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE); //Plays dying animation
      enemy0X+=2;
    }
    if (playerX > enemy0X)
    {
      ardbitmap.drawCompressed(enemy0X, enemy0Y, dyingAnim[enemy0DyingAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); //Plays dying animation
      enemy0X-=2;
    }
      enemy0DyingAnimFrame++;
      
      enemy0Y+=2;
    if (enemy0DyingAnimFrame > 7)   //Resets animation counter after playing
    {
      enemy0DyingAnimFrame = 0;
      enemy0Dying = false;      //Enemy no longer dying
      enemy0Y = ground;         //Reset x and y variables
      enemy0X = enemySpawnRightX;
    }
  }

  //Enemy 1 death animation
  if (enemy1Dying == true)//Checks to see if enemy is dying
  {
    sound.tone(175, 25);
    if (playerX < enemy1X)
    {
      ardbitmap.drawCompressed(enemy1X, enemy1Y, dyingAnim[enemy1DyingAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE); //Plays dying animation
      enemy0X+=2;
    }
    if (playerX > enemy1X)
    {
      ardbitmap.drawCompressed(enemy1X, enemy1Y, dyingAnim[enemy1DyingAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); //Plays dying animation
      enemy1X-=2;
    }
      enemy1DyingAnimFrame++;
      enemy1Y+=2;
    if (enemy1DyingAnimFrame > 7)   //Resets animation counter after playing
    {
      enemy1DyingAnimFrame = 0;
      enemy1Dying = false;      //Enemy no longer dying
      enemy1Y = ground;         //Reset x and y variables
      enemy1X = enemySpawnLeftX;
    }
  }

  //Enemy 2 death animation
  if (enemy2Dying == true)//Checks to see if enemy is dying
  {
    sound.tone(175, 25);
    if (playerX < enemy2X)
    {
      ardbitmap.drawCompressed(enemy2X, enemy2Y, dyingAnim[enemy2DyingAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE); //Plays dying animation
      enemy2X+=2;
    }
    if (playerX > enemy2X)
    {
      ardbitmap.drawCompressed(enemy2X, enemy2Y, dyingAnim[enemy2DyingAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); //Plays dying animation
      enemy2X-=2;
    }
      enemy2DyingAnimFrame++;
      
      enemy2Y+=2;
    if (enemy2DyingAnimFrame > 7)   //Resets animation counter after playing
    {
      enemy2DyingAnimFrame = 0;
      enemy2Dying = false;      //Enemy no longer dying
      enemy2Y = ground;         //Reset x and y variables
      enemy2X = enemySpawnRightX;
    }
  }

  //Enemy 3 death animation
  if (enemy3Dying == true)//Checks to see if enemy is dying
  {
    sound.tone(175, 25);
    if (playerX < enemy3X)
    {
      ardbitmap.drawCompressed(enemy3X, enemy3Y, dyingAnim[enemy3DyingAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE); //Plays dying animation
      enemy3X+=2;
    }
    if (playerX > enemy3X)
    {
      ardbitmap.drawCompressed(enemy3X, enemy3Y, dyingAnim[enemy3DyingAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); //Plays dying animation
      enemy3X-=2;
    }
      enemy3DyingAnimFrame++;
      
      enemy3Y+=2;
    if (enemy3DyingAnimFrame > 7)   //Resets animation counter after playing
    {
      enemy3DyingAnimFrame = 0;
      enemy3Dying = false;      //Enemy no longer dying
      enemy3Y = ground;         //Reset x and y variables
      enemy3X = enemySpawnLeftX;
    }
  }

  //Enemy 4 death animation
    if (enemy4Dying == true)//Checks to see if enemy is dying
  {
    sound.tone(175, 25);
    if (playerX < enemy4X)
    {
      ardbitmap.drawCompressed(enemy4X, enemy4Y, dyingAnim[enemy4DyingAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE); //Plays dying animation
      enemy4X+=2;
    }
    if (playerX > enemy4X)
    {
      ardbitmap.drawCompressed(enemy4X, enemy4Y, dyingAnim[enemy4DyingAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); //Plays dying animation
      enemy4X-=2;
    }
      enemy4DyingAnimFrame++;
      
      enemy4Y+=2;
    if (enemy4DyingAnimFrame > 7)   //Resets animation counter after playing
    {
      enemy4DyingAnimFrame = 0;
      enemy4Dying = false;      //Enemy no longer dying
      enemy4Y = ground;         //Reset x and y variables
      enemy4X = enemySpawnRightX;
    }
  }


  //Dog controls
  
    //Dog 0 controls
  if (dog0Exist == true)    //Check for existence
  {
    dog0Rect = {dog0X-8, dog0Y-2, 14, 8};      //Create collision rect
    //arduboy.drawRect(dog0X-8,dog0Y-2, 14, 8);
    if (dog0MoveEnable == true)
    {
      ardbitmap.drawCompressed(dog0X, dog0Y, dogRunAnim[dog0RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);    //Draw enemy
      dog0RunAnimFrame++;
      dog0X -= playerSpeed;//Runs Left
      if (dog0RunAnimFrame > 6)
      {
        dog0RunAnimFrame = 0;
      }
    }
  }

      //Dog 1 controls
  if (dog1Exist == true)    //Check for existence
  {
    dog1Rect = {dog1X-8, dog1Y-2, 14, 8};      //Create collision rect
    //arduboy.drawRect(dog1X-8,dog1Y-2, 14, 8);
    if (dog1MoveEnable == true)
    {
      ardbitmap.drawCompressed(dog1X, dog1Y, dogRunAnim[dog1RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE);    //Draw enemy
      dog1RunAnimFrame++;
      dog1X += playerSpeed;//Runs Right
      if (dog1RunAnimFrame > 6)
      {
        dog1RunAnimFrame = 0;
      }
    }
  }
      //Dog 2 controls
  if (dog2Exist == true)    //Check for existence
  {
    dog2Rect = {dog2X-8, dog2Y-2, 14, 8};      //Create collision rect
    //arduboy.drawRect(dog2X-8,dog2Y-2, 14, 8);
    if (dog2MoveEnable == true)
    {
      ardbitmap.drawCompressed(dog2X, dog2Y, dogRunAnim[dog2RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);    //Draw enemy
      dog2RunAnimFrame++;
      dog2X -= playerSpeed;//Runs Left
      if (dog2RunAnimFrame > 6)
      {
        dog2RunAnimFrame = 0;
      }
    }
  }

        //Dog 3 controls
  if (dog3Exist == true)    //Check for existence
  {
    dog3Rect = {dog3X-8, dog3Y-2, 14, 8};      //Create collision rect
    //arduboy.drawRect(dog3X-8,dog3Y-2, 14, 8);
    if (dog3MoveEnable == true)
    {
      ardbitmap.drawCompressed(dog3X, dog3Y, dogRunAnim[dog3RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE);    //Draw enemy
      dog3RunAnimFrame++;
      dog3X += playerSpeed;//Runs Right
      if (dog3RunAnimFrame > 6)
      {
        dog3RunAnimFrame = 0;
      }
    }
  }

   //Dog 0 death animation
    if (dog0Dying == true)//Checks to see if enemy is dying
  {
    sound.tone(175, 25);
    if (playerX < dog0X)
    {
      ardbitmap.drawCompressed(dog0X, dog0Y, dogRunAnim[dog0RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_VERTICAL); //Plays dying animation
      dog0X+=2;
    }
    if (playerX > dog0X)
    {
      ardbitmap.drawCompressed(dog0X, dog0Y, dogRunAnim[dog0RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HOR_VER); //Plays dying animation
      dog0X-=2;
    }
      dog0RunAnimFrame++;
      
      dog0Y+=2;
    if (dog0RunAnimFrame > 6)   //Resets animation counter after playing
    {
      dog0RunAnimFrame = 0;
      dog0Dying = false;      //Enemy no longer dying
      dog0Y = ground + 8;         //Reset x and y variables
      dog0X = enemySpawnRightX;
    }
  }


   //Dog 1 death animation
    if (dog1Dying == true)//Checks to see if enemy is dying
  {
    sound.tone(175, 25);
    if (playerX < dog1X)
    {
      ardbitmap.drawCompressed(dog1X, dog1Y, dogRunAnim[dog1RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_VERTICAL); //Plays dying animation
      dog0X+=2;
    }
    if (playerX > dog1X)
    {
      ardbitmap.drawCompressed(dog1X, dog1Y, dogRunAnim[dog1RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HOR_VER); //Plays dying animation
      dog1X-=2;
    }
      dog1RunAnimFrame++;
      
      dog1Y+=2;
    if (dog1RunAnimFrame > 6)   //Resets animation counter after playing
    {
      dog1RunAnimFrame = 0;
      dog1Dying = false;      //Enemy no longer dying
      dog1Y = ground + 8;         //Reset x and y variables
      dog1X = enemySpawnLeftX;
    }
  }


   //Dog 2 death animation
    if (dog2Dying == true)//Checks to see if enemy is dying
  {
    sound.tone(175, 25);
    if (playerX < dog2X)
    {
      ardbitmap.drawCompressed(dog2X, dog2Y, dogRunAnim[dog2RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_VERTICAL); //Plays dying animation
      dog2X+=2;
    }
    if (playerX > dog2X)
    {
      ardbitmap.drawCompressed(dog2X, dog2Y, dogRunAnim[dog2RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HOR_VER); //Plays dying animation
      dog2X-=2;
    }
      dog2RunAnimFrame++;
      
      dog2Y+=2;
    if (dog2RunAnimFrame > 6)   //Resets animation counter after playing
    {
      dog2RunAnimFrame = 0;
      dog2Dying = false;      //Enemy no longer dying
      dog2Y = ground + 8;         //Reset x and y variables
      dog2X = enemySpawnRightX;
    }
  }


   //Dog 3 death animation
    if (dog3Dying == true)//Checks to see if enemy is dying
  {
    sound.tone(175, 25);
    if (playerX < dog3X)
    {
      ardbitmap.drawCompressed(dog3X, dog3Y, dogRunAnim[dog3RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_VERTICAL); //Plays dying animation
      dog3X+=2;
    }
    if (playerX > dog3X)
    {
      ardbitmap.drawCompressed(dog3X, dog3Y, dogRunAnim[dog3RunAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HOR_VER); //Plays dying animation
      dog3X-=2;
    }
      dog3RunAnimFrame++;
      
      dog3Y+=2;
    if (dog3RunAnimFrame > 6)   //Resets animation counter after playing
    {
      dog3RunAnimFrame = 0;
      dog3Dying = false;      //Enemy no longer dying
      dog3Y = ground + 8;         //Reset x and y variables
      dog3X = enemySpawnLeftX;
    }
  }
}


void CollisionDetection()
{
  //update player Collision Rect 
  if (playerFaceRight == false)       //Rect position slightly changed dependging on which way player facing. To cover sprite properly.
  {
    playerRect = {playerX-3, playerY-9, 9, 24};
    //arduboy.fillRect(playerX-3, playerY-9, 9, 24); //This was used to show the player rect
    if (playerKickLeft == true || playerFlyKickLeft == true)   //Enabling kick rect
      {
        playerKickRect = {playerX-18,playerY-4, 18, 10};
        //arduboy.drawRect(playerX-18,playerY-4, 18, 10);
      }
      if (playerCrouchPunchLeft == true)
      {
        playerKickRect = {playerX-10, playerY+5, 10, 10};
        //arduboy.drawRect(playerX-10, playerY+5, 10, 10);
      } 
  }
  
  if (playerFaceRight == true)      //Rect position slightly changed dependging on which way player facing. To cover sprite properly.
  {
    playerRect = {playerX-6, playerY-9, 9, 24};
    //arduboy.fillRect(playerX-6, playerY-9, 9, 24); //This was used to show the player rect
    if (playerKickRight == true || playerFlyKickRight == true)    //Enabling kick rect
    {
      playerKickRect = {playerX,playerY-4, 18, 10};
      //arduboy.drawRect(playerX,playerY-4, 18, 10);
    }
    if (playerCrouchPunchRight == true)
    {
      playerKickRect = {playerX, playerY+5, 10, 10};
      //arduboy.drawRect(playerX, playerY+5, 10, 10);
    }
  }
  if (playerKickRight == false && playerKickLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerCrouchPunchRight == false && playerCrouchPunchLeft == false)  //If player is not attacking then turn kick rect off
  {
    playerKickRect = {0,0,0,0};
    //arduboy.fillRect(0,0,0,0);
  }

  //Checking for collisions
if (arduboy.collide(playerRect, dog0Rect))              // Check for collision and if enemy hitting player then freeze both. Need to add health sub here
  {
    playerMoveEnable = false; //Freeze player
    dog0MoveEnable = false;  //Freeze enemy
    if (playerX > dog0X)                                  //Check which side player on
    {
      ardbitmap.drawCompressed(dog0X, dog0Y, Dog3, WHITE, ALIGN_CENTER, MIRROR_NONE); 
    }
    else
    {
      ardbitmap.drawCompressed(dog0X, dog0Y, Dog3, WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); 
    }
    health -= enemyAttack;
  }
  else                                                      //Otherwise enemy is unfrozen
  {
    dog0MoveEnable = true;
  }

if (arduboy.collide(playerRect, dog1Rect))              // Check for collision and if enemy hitting player then freeze both. Need to add health sub here
  {
    playerMoveEnable = false; //Freeze player
    dog1MoveEnable = false;  //Freeze enemy
    if (playerX > dog1X)                                  //Check which side player on
    {
      ardbitmap.drawCompressed(dog1X, dog1Y, Dog3, WHITE, ALIGN_CENTER, MIRROR_NONE); 
    }
    else
    {
      ardbitmap.drawCompressed(dog1X, dog1Y, Dog3, WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); 
    }
    health -= enemyAttack;
  }
  else                                                      //Otherwise enemy is unfrozen
  {
    dog1MoveEnable = true;
  }

if (arduboy.collide(playerRect, dog2Rect))              // Check for collision and if enemy hitting player then freeze both. Need to add health sub here
  {
    playerMoveEnable = false; //Freeze player
    dog2MoveEnable = false;  //Freeze enemy
    if (playerX > dog2X)                                  //Check which side player on
    {
      ardbitmap.drawCompressed(dog2X, dog2Y, Dog3, WHITE, ALIGN_CENTER, MIRROR_NONE); 
    }
    else
    {
      ardbitmap.drawCompressed(dog2X, dog2Y, Dog3, WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); 
    }
    health -= enemyAttack;
  }
  else                                                      //Otherwise enemy is unfrozen
  {
    dog2MoveEnable = true;
  }

if (arduboy.collide(playerRect, dog3Rect))              // Check for collision and if enemy hitting player then freeze both. Need to add health sub here
  {
    playerMoveEnable = false; //Freeze player
    dog3MoveEnable = false;  //Freeze enemy
    if (playerX > dog3X)                                  //Check which side player on
    {
      ardbitmap.drawCompressed(dog3X, dog3Y, Dog3, WHITE, ALIGN_CENTER, MIRROR_NONE); 
    }
    else
    {
      ardbitmap.drawCompressed(dog3X, dog3Y, Dog3, WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); 
    }
    health -= enemyAttack;
  }
  else                                                      //Otherwise enemy is unfrozen
  {
    dog3MoveEnable = true;
  }

  
  if (arduboy.collide(playerRect, enemy0Rect))              // Check for collision and if enemy hitting player then freeze both. Need to add health sub here
  {
    playerMoveEnable = false; //Freeze player
    enemy0MoveEnable = false;  //Freeze enemy
    if (playerX > enemy0X)                                  //Check which side player on
    {
      ardbitmap.drawCompressed(enemy0X, enemy0Y, EnemyGrab0, WHITE, ALIGN_CENTER, MIRROR_NONE); 
    }
    else
    {
      ardbitmap.drawCompressed(enemy0X, enemy0Y, EnemyGrab0, WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); 
    }
    health -= enemyAttack;
  }
  else                                                      //Otherwise enemy is unfrozen
  {
    enemy0MoveEnable = true;
  }

  if (arduboy.collide(playerRect, enemy1Rect))
  {
    playerMoveEnable = false; //Freeze player
    enemy1MoveEnable = false;  //Freeze enemy
    if (playerX > enemy1X)
    {
      ardbitmap.drawCompressed(enemy1X, enemy1Y, EnemyGrab0, WHITE, ALIGN_CENTER, MIRROR_NONE); 
    }
    else
    {
      ardbitmap.drawCompressed(enemy1X, enemy1Y, EnemyGrab0, WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); 
    }
    health -= enemyAttack;
  }
  else
  {
    enemy1MoveEnable = true;
  }

  if (arduboy.collide(playerRect, enemy2Rect))
  {
    playerMoveEnable = false; //Freeze player
    enemy2MoveEnable = false;  //Freeze enemy
    if (playerX > enemy2X)
    {
      ardbitmap.drawCompressed(enemy2X, enemy2Y, EnemyGrab0, WHITE, ALIGN_CENTER, MIRROR_NONE); 
    }
    else
    {
      ardbitmap.drawCompressed(enemy2X, enemy2Y, EnemyGrab0, WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); 
    }
    health -= enemyAttack;
  }
  else
  {
    enemy2MoveEnable = true;
  }

  if (arduboy.collide(playerRect, enemy3Rect))
  {
    playerMoveEnable = false; //Freeze player
    enemy3MoveEnable = false;  //Freeze enemy
    if (playerX > enemy3X)
    {
      ardbitmap.drawCompressed(enemy3X, enemy3Y, EnemyGrab0, WHITE, ALIGN_CENTER, MIRROR_NONE); 
    }
    else
    {
      ardbitmap.drawCompressed(enemy3X, enemy3Y, EnemyGrab0, WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); 
    }
    health -= enemyAttack;
  }
  else
  {
    enemy3MoveEnable = true;
  }

  if (arduboy.collide(playerRect, enemy4Rect))
  {
    playerMoveEnable = false; //Freeze player
    enemy4MoveEnable = false;  //Freeze enemy
    if (playerX > enemy4X)
    {
      ardbitmap.drawCompressed(enemy4X, enemy4Y, EnemyGrab0, WHITE, ALIGN_CENTER, MIRROR_NONE); 
    }
    else
    {
      ardbitmap.drawCompressed(enemy4X, enemy4Y, EnemyGrab0, WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); 
    }
    health -= enemyAttack;
  }
  else
  {
    enemy4MoveEnable = true;
  }


  if (!arduboy.collide(playerRect, enemy0Rect) && !arduboy.collide(playerRect, enemy1Rect) && !arduboy.collide(playerRect, enemy2Rect) && !arduboy.collide(playerRect, enemy3Rect) && !arduboy.collide(playerRect, enemy4Rect) && !arduboy.collide(playerRect, dog0Rect))
  {
    playerMoveEnable = true; //Player free to move    
  }
  
  if (arduboy.collide(playerKickRect, enemy0Rect))  // Checking enemy 0
  {
    enemy0Exist = false;
    enemy0Dying = true;
    enemy0Spawn = enemy0Spawn - enemySpawnFreq;   //enemySpawn is tied to bgX value
    enemy0Rect = {0,0,0,0};
  }
  
  if (arduboy.collide(playerKickRect, enemy1Rect))  // Checking enemy 1
  {
    enemy1Exist = false;
    enemy1Dying = true;
    enemy1Spawn = enemy1Spawn - enemySpawnFreq;
    enemy1Rect = {0,0,0,0};
  }
  
  if (arduboy.collide(playerKickRect, enemy2Rect))  // Checking enemy 2
  {
    enemy2Exist = false;
    enemy2Dying = true;
    enemy2Spawn = enemy2Spawn - enemySpawnFreq;
    enemy2Rect = {0,0,0,0};
  }
  
  if (arduboy.collide(playerKickRect, enemy3Rect))  // Checking enemy 3
  {
    enemy3Exist = false;
    enemy3Dying = true;
    enemy3Spawn = enemy3Spawn - enemySpawnFreq;
    enemy3Rect = {0,0,0,0};
  }
  
  if (arduboy.collide(playerKickRect, enemy4Rect))  // Checking enemy 4
  {
    enemy4Exist = false;
    enemy4Dying = true;
    enemy4Spawn = enemy4Spawn - enemySpawnFreq;
    enemy4Rect = {0,0,0,0};
  }
  
  if (arduboy.collide(playerKickRect, dog0Rect))  // Checking dog 0
  {
    dog0Exist = false;
    dog0Dying = true;
    dog0Spawn = dog0Spawn - enemySpawnFreq;
    dog0Rect = {0,0,0,0};
  }
  
  if (arduboy.collide(playerKickRect, dog1Rect))  // Checking dog 1
  {
    dog1Exist = false;
    dog1Dying = true;
    dog1Spawn = dog1Spawn - enemySpawnFreq;
    dog1Rect = {0,0,0,0};
  }
  
  if (arduboy.collide(playerKickRect, dog2Rect))  // Checking dog 2
  {
    dog2Exist = false;
    dog2Dying = true;
    dog2Spawn = dog2Spawn - enemySpawnFreq;
    dog2Rect = {0,0,0,0};
  }
  
  if (arduboy.collide(playerKickRect, dog3Rect))  // Checking dog 3
  {
    dog3Exist = false;
    dog3Dying = true;
    dog3Spawn = dog3Spawn - enemySpawnFreq;
    dog3Rect = {0,0,0,0};
  }
  
}

void DrawLevel()
{ 
  
  // When screen has scrolled one tile over
    if (bgX < -15)
    { 
      levelProg++;
      for (int x = 0; x < 8; x++) //copy all the tiles one over in the array
      {
        displayFloorTiles[x] = displayFloorTiles[x+1];
        displayMiddleLowTiles[x] = displayMiddleLowTiles[x+1];
        displayMiddleHighTiles[x] = displayMiddleHighTiles[x+1];
        displayCeilingTiles[x] = displayCeilingTiles[x+1];
      }
  
        // Draw upcomming tiles
        displayFloorTiles[8] = dungeonTileMap[random(displayTiles[0], displayTiles[1])];
        displayCeilingTiles[8] = dungeonTileMap[random(displayTiles[6], displayTiles[7])];
  
        if (spawnX % -64 == 0)          //Spacing out the decorative tiles using modulus
        {
          displayMiddleHighTiles[8] = dungeonTileMap[displayTiles[5]];       
        }
        else
        {
          displayMiddleHighTiles[8] = dungeonTileMap[displayTiles[4]];     
        }
        
        if (spawnX % displayTiles[8] == 0)          //Spacing out the decorative tiles using modulus
        {
          displayMiddleLowTiles[8] = dungeonTileMap[random(displayTiles[2], displayTiles[3])];
        }
        else
        {
          displayMiddleLowTiles[8] = dungeonTileMap[displayTiles[4]];
        }
        
      if (levelProg > 59)   //once level nears end draw exit
      {
        if ( level < 5)
        {
          DrawExit(1);
        }
        else 
        {
          DrawExit(2);
        }
      }
     
      bgX = 0; //Reset the scroll variable
   }

    
    //Draw screen
    for (int x = 0; x< 9; x++)
    {
      ardbitmap.drawCompressed(x * TILE_SIZE + bgX, floorTileY, displayFloorTiles[x], WHITE, ALIGN_H_LEFT, MIRROR_NONE);       //Draw floor tiles
      ardbitmap.drawCompressed(x * TILE_SIZE + bgX, middleHighTileY, displayMiddleHighTiles[x], WHITE, ALIGN_H_LEFT, MIRROR_NONE);     //Draw middle high tiles
      ardbitmap.drawCompressed(x * TILE_SIZE + bgX, middleLowTileY, displayMiddleLowTiles[x], WHITE, ALIGN_H_LEFT, MIRROR_NONE);     //Draw middle low tiles
      ardbitmap.drawCompressed(x * TILE_SIZE + bgX, ceilingTileY, displayCeilingTiles[x], WHITE, ALIGN_H_LEFT, MIRROR_NONE);   //Draw ceiling tiles
    }
}

void DrawExit(int x)
{
  if (x == 1)
  {
    if (levelProg == 60)
    {
       displayFloorTiles[8] = dungeonTileMap[36];
       displayMiddleLowTiles[8] = dungeonTileMap[30];    
       displayMiddleHighTiles[8] = dungeonTileMap[30];    
       displayCeilingTiles[8] = dungeonTileMap[30];
    }
    if (levelProg == 61) //if near end of level then start drawing exit
    {
       displayFloorTiles[8] = dungeonTileMap[36];
       displayMiddleLowTiles[8] = dungeonTileMap[15];    
       displayMiddleHighTiles[8] = dungeonTileMap[13];    
       displayCeilingTiles[8] = dungeonTileMap[30];
    }
    if (levelProg == 62) //if near end of level then start drawing exit
    {
       displayFloorTiles[8] = dungeonTileMap[36];
       displayMiddleLowTiles[8] = dungeonTileMap[16];    
       displayMiddleHighTiles[8] = dungeonTileMap[14];    
       displayCeilingTiles[8] = dungeonTileMap[30];
    }
    if (levelProg > 62)
    {
       displayFloorTiles[8] = dungeonTileMap[36];
       displayMiddleLowTiles[8] = dungeonTileMap[30];    
       displayMiddleHighTiles[8] = dungeonTileMap[30];    
       displayCeilingTiles[8] = dungeonTileMap[30];
    }
  }
  else if (x == 2)
  {
    if (levelProg == 60)
    {
       displayFloorTiles[8] = dungeonTileMap[37];
       displayMiddleLowTiles[8] = dungeonTileMap[11];    
       displayMiddleHighTiles[8] = dungeonTileMap[11];    
       displayCeilingTiles[8] = dungeonTileMap[random(31, 36)];
    }
    if (levelProg == 61) //if near end of level then start drawing exit
    {
       displayFloorTiles[8] = dungeonTileMap[37];
       displayMiddleLowTiles[8] = dungeonTileMap[11];    
       displayMiddleHighTiles[8] = dungeonTileMap[11];    
       displayCeilingTiles[8] = dungeonTileMap[random(31, 36)];
    }
    if (levelProg == 62) //if near end of level then start drawing exit
    {
       displayFloorTiles[8] = dungeonTileMap[37];
       displayMiddleLowTiles[8] = dungeonTileMap[38];    
       displayMiddleHighTiles[8] = dungeonTileMap[38];    
       displayCeilingTiles[8] = dungeonTileMap[38];
    }
    if (levelProg > 62)
    {
       displayFloorTiles[8] = dungeonTileMap[37];
       displayMiddleLowTiles[8] = dungeonTileMap[11];    
       displayMiddleHighTiles[8] = dungeonTileMap[11];    
       displayCeilingTiles[8] = dungeonTileMap[random(31, 36)];
    }
  }
}

void DrawPlayer()
{
  //Idle animation
  if (playerRunRight == false && playerRunLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && playerCrouchLeft == false && playerCrouchRight == false && playerCrouchPunchLeft == false && playerCrouchPunchRight == false)  
  { 
    if (playerFaceRight == true)
     {
        ardbitmap.drawCompressed(playerX, playerY, Idle0, WHITE, ALIGN_CENTER, MIRROR_NONE);
     }
    else
     {
        ardbitmap.drawCompressed(playerX, playerY, Idle0, WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);
     }
   }
  
  if (playerRunRight == true)
  {
    ardbitmap.drawCompressed(playerX, playerY, runAnim[runAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE);
    runAnimFrame++;
    if (runAnimFrame > 7)
    {
      runAnimFrame = 0;
    }
  }
        
  if (playerRunLeft == true)
  {
    ardbitmap.drawCompressed(playerX, playerY, runAnim[runAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);
    runAnimFrame++;
    if (runAnimFrame > 7)
    {
      runAnimFrame = 0;
    }
  }

 if (playerFlyKickRight == true)
 {
    ardbitmap.drawCompressed(playerX, playerY, flyKick[punchAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE);
    punchAnimFrame++;
    playerX += playerSpeed;
    if (punchAnimFrame > 9)
    {
      punchAnimFrame = 0;
      playerFlyKickRight = false;
    }
 }

 if (playerFlyKickLeft == true)
 {
    ardbitmap.drawCompressed(playerX, playerY, flyKick[punchAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);
    punchAnimFrame++;
    playerX -= playerSpeed;
    if (punchAnimFrame > 9)
    {
      punchAnimFrame = 0;
      playerFlyKickLeft = false;
    }
 }

 if (playerKickRight == true)
 {
    ardbitmap.drawCompressed(playerX, playerY, kickAnim[kickAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE);
    kickAnimFrame++;
    if (kickAnimFrame > 7)
    {
      kickAnimFrame = 0;
      playerKickRight = false;
    }
 }

  if (playerKickLeft == true)
 {
    ardbitmap.drawCompressed(playerX, playerY, kickAnim[kickAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);
    kickAnimFrame++;
    if (kickAnimFrame > 7)
    {
      kickAnimFrame = 0;
      playerKickLeft = false;
    }
 }

  if (playerCrouchRight == true)
  {
     ardbitmap.drawCompressed(playerX, playerY, Crouch0, WHITE, ALIGN_CENTER, MIRROR_NONE);
  }

  if (playerCrouchLeft == true)
  {
     ardbitmap.drawCompressed(playerX, playerY, Crouch0, WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);
  }

    if (playerCrouchPunchRight == true)
  {
     ardbitmap.drawCompressed(playerX, playerY, crouchPunchAnim[kickAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE);
     kickAnimFrame++;
     if (kickAnimFrame > 6)
     {
        kickAnimFrame = 0;
        playerCrouchPunchRight = false;
     }
  }

     if (playerCrouchPunchLeft == true)
  {
     ardbitmap.drawCompressed(playerX, playerY, crouchPunchAnim[kickAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);
     kickAnimFrame++;
     if (kickAnimFrame > 6)
     {
        kickAnimFrame = 0;
        playerCrouchPunchLeft = false;
     }
  }
 
}

void Movement()
{
  //Setting up movement buttons
  //run left
    if (arduboy.pressed(LEFT_BUTTON) && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false)
    {
      if (playerMoveEnable == true)
      {
        playerX = playerX - playerSpeed;
      }
      playerRunLeft = true;
      playerRunRight = false;
      playerFaceRight = false;
      playerCrouchPunchRight = false;
      playerCrouchPunchLeft = false;
      
    }
    // run right
    if (arduboy.pressed(RIGHT_BUTTON) && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false)
    {
      if (playerMoveEnable == true)
      {
        playerX = playerX + playerSpeed;
      }
      playerRunRight = true;
      playerRunLeft = false;
      playerFaceRight = true;
      playerCrouchPunchRight = false;
      playerCrouchPunchLeft = false;
      
    }
  
  // idle
  if (!arduboy.pressed(LEFT_BUTTON) && !arduboy.pressed(RIGHT_BUTTON) && !arduboy.pressed(DOWN_BUTTON))
  {
    playerRunRight = false;
    playerRunLeft = false;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = false;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }
  //Crouch right
  if (arduboy.pressed(DOWN_BUTTON) && playerFaceRight == true && playerRunRight == false && playerRunLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && playerCrouchRight == false && playerCrouchLeft == false && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false)
  {
    playerCrouchRight = true;
    playerCrouchLeft = false;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = false;
  }

  //Crouch left
  if (arduboy.pressed(DOWN_BUTTON) && playerFaceRight == false && playerRunRight == false && playerRunLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && playerCrouchRight == false && playerCrouchLeft == false && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false)
  {
    playerCrouchRight = false;
    playerCrouchLeft = true;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = false;
  }
  
  //Setting up attack buttons
  //Flykick right
  if (arduboy.pressed(A_BUTTON) && playerFaceRight == true && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && !arduboy.collide(playerRect, dog0Rect) && !arduboy.collide(playerRect, dog1Rect) && !arduboy.collide(playerRect, dog2Rect) && !arduboy.collide(playerRect, dog3Rect) && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false && playerMoveEnable == true)  
  {
    playerFlyKickRight = true;
    playerFlyKickLeft = false;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = false;
    playerKickLeft = false;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = false;
    playerCrouchRight = false;
    playerCrouchLeft = false;
    
  }
  //Flykick left
    if (arduboy.pressed(A_BUTTON) && playerFaceRight == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && !arduboy.collide(playerRect, dog0Rect) && !arduboy.collide(playerRect, dog1Rect) && !arduboy.collide(playerRect, dog2Rect) && !arduboy.collide(playerRect, dog3Rect) && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false && playerMoveEnable == true)
  {
    playerFlyKickRight = false;
    playerFlyKickLeft = true;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = false;
    playerKickLeft = false;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = false;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }

    //kick right
  if (arduboy.pressed(B_BUTTON) && playerFaceRight == true && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false)
  {
    playerFlyKickRight = false;
    playerFlyKickLeft = false;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = true;
    playerKickLeft = false;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = false;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }
    //kick left
    if (arduboy.pressed(B_BUTTON) && playerFaceRight == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false)
  {
    playerFlyKickRight = false;
    playerFlyKickLeft = false;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = false;
    playerKickLeft = true;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = false;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }

  //Crouch punch right
  if (arduboy.pressed(B_BUTTON) && arduboy.pressed(DOWN_BUTTON) && playerFaceRight == true && playerRunRight == false && playerRunLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false)
  {
    playerFlyKickRight = false;
    playerFlyKickLeft = false;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = false;
    playerKickLeft = false;
    playerCrouchPunchRight = true;
    playerCrouchPunchLeft = false;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }

  //Crouch punch left
  if (arduboy.pressed(B_BUTTON) &&  arduboy.pressed(DOWN_BUTTON) && playerFaceRight == false && playerRunRight == false && playerRunLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false)
  {
    playerFlyKickRight = false;
    playerFlyKickLeft = false;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = false;
    playerKickLeft = false;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = true;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }

  //Crouch punch right
  if (arduboy.pressed(A_BUTTON) && arduboy.pressed(DOWN_BUTTON) && playerFaceRight == true && playerRunRight == false && playerRunLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false)
  {
    playerFlyKickRight = false;
    playerFlyKickLeft = false;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = false;
    playerKickLeft = false;
    playerCrouchPunchRight = true;
    playerCrouchPunchLeft = false;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }

  //Crouch punch left
  if (arduboy.pressed(A_BUTTON) &&  arduboy.pressed(DOWN_BUTTON) && playerFaceRight == false && playerRunRight == false && playerRunLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false)
  {
    playerFlyKickRight = false;
    playerFlyKickLeft = false;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = false;
    playerKickLeft = false;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = true;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }
}

void ScreenLimit()
{
  //setting boundires for screen
 /*
  if (bgX < 0)
  {
    if (playerX < 60)   //If player goes to left limit
    {
      playerX = 60;     //Limit the player to that x coord
      bgX = bgX + playerSpeed;  //Then scroll the background x value
      spawnX += playerSpeed;    //Increment spawn X by player speed. This is used for middle tiles and spawning
      if (enemy0Exist == true)
      {
         enemy0X = enemy0X + playerSpeed; //scroll enemies
      }
      if (enemy1Exist == true)
      {
         enemy1X = enemy1X + playerSpeed; //scroll enemies
      }
      if (enemy2Exist == true)
      {
         enemy2X = enemy2X + playerSpeed; //scroll enemies
      }
      if (enemy3Exist == true)
      {
         enemy3X = enemy3X + playerSpeed; //scroll enemies
      }
      if (enemy4Exist == true)
      {
         enemy4X = enemy4X + playerSpeed; //scroll enemies
      }
      if (enemy5Exist == true)
      {
         enemy5X = enemy5X + playerSpeed; //scroll enemies
      }
    }   
  }
  */
  if (spawnX > -1024)
  { 
      if (playerX > 70)    
      {
        playerX = 70;
        bgX = bgX - playerSpeed;
        spawnX -= playerSpeed;
        if (enemy0Exist == true)
        {
          enemy0X = enemy0X - playerSpeed + 1;
        }
        if (enemy1Exist == true)
        {
          enemy1X = enemy1X - playerSpeed + 1;
        }
        if (enemy2Exist == true)
        {
          enemy2X = enemy2X - playerSpeed + 1;
        }
        if (enemy3Exist == true)
        {
          enemy3X = enemy3X - playerSpeed + 1;
        }
        if (enemy4Exist == true)
        {
          enemy4X = enemy4X - playerSpeed + 1;
        }
        if (dog0Exist == true)
        {
          dog0X = dog0X - playerSpeed + 1;
        }
         if (dog1Exist == true)
        {
          dog1X = dog1X - playerSpeed + 1;
        }
         if (dog2Exist == true)
        {
          dog2X = dog2X - playerSpeed + 1;
        }
         if (dog3Exist == true)
        {
          dog3X = dog3X - playerSpeed + 1;
        }
      }
  }
   if (playerX < 4)   //This stops the player walking off the screen after the background stops scrolling
      {
     playerX = 4;
   }
   if (playerX > 96)
   {
     playerX = 96;
   }
  
  
 // Serial.println(bgX);
  //Serial.println(playerX);
}
