#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

// OLED spi
#define SCLK_PIN 2
#define MOSI_PIN 3
#define OLED_CS    6
#define OLED_RESET 7
#define OLED_DC    5

// screen dimensions
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 

// control
#define BUTTON_LEFT_PIN 2
#define BUTTON_RIGHT_PIN 3
#define BUTTON_FORWARD_PIN 4
#define BUTTON_BACKWARD_PIN 5
#define BUTTON_UP_PIN 6
#define BUTTON_DOWN_PIN 7

// map size
#define MAP_WIDTH 8
#define MAP_HEIGHT 8

// color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

// Adafruit_SSD1351 init
Adafruit_SSD1351 display = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_CS, OLED_DC, OLED_RESET); 


class GameField {
  public:
    int gfield[MAP_WIDTH][MAP_HEIGHT] = {
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 0, 0, 0, 0, 0, 0, 1},
      {1, 0, 1, 1, 0, 0, 0, 1},
      {1, 0, 1, 0, 0, 1, 0, 1},
      {1, 0, 1, 0, 0, 1, 0, 1},
      {1, 0, 0, 0, 0, 0, 0, 1},
      {1, 0, 0, 1, 1, 1, 0, 1},
      {1, 1, 1, 1, 1, 1, 1, 1}
    };      
};


class Player {
  public:
    float playerX = 3.5;
    float playerY = 3.5;
    float playerAngle = 0.0;
    
    void updatePosition(GameField field);
    float getPlayerPosition(float& x, float& y, float& a);    
};

    void Player::updatePosition(GameField field) {
      // read input from joystick or other input device
      int joystickX = analogRead(A0);
      int joystickY = analogRead(A1);

      // update player position and angle based on input
      playerAngle += (float)(joystickX - 512) / 1024.0 * 5.0;
      playerX += cos(playerAngle * PI / 180.0) * (float)(joystickY - 512) / 1024.0 * 0.1;
      playerY += sin(playerAngle * PI / 180.0) * (float)(joystickY - 512) / 1024.0 * 0.1;


      // ensure player does not move through walls
      if (field.gfield[(int)playerX][(int)playerY] == 1) {
        playerX -= cos(playerAngle * PI / 180.0) * (float)(joystickY - 512) / 1024.0 * 0.1;
        playerY -= sin(playerAngle * PI / 180.0) * (float)(joystickY - 512) / 1024.0 * 0.1;
      }      
    }

    float Player::getPlayerPosition(float& x, float& y, float& a)
    {
      x = playerX;
      y = playerY;
      a = playerAngle;
    }

     /*outsourced from GameField class because forward declaration was not possible in one page script*/
    void drawWalls(Player play, GameField field) {
      for (int x = 0; x < 128; x++) {
        float rayX, rayY, rayAngle;

        play.getPlayerPosition(rayX, rayY, rayAngle);
        rayAngle = rayAngle - 30.0 + (float)x / 128.0 * 60.0;
        float rayDistance = 0.0;
        bool hitWall = false;
        while (!hitWall && rayDistance < 10.0) {
          int mapX = (int)rayX;
          int mapY = (int)rayY;
          if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
            hitWall = true;
            rayDistance = 10.0;
          } else if (field.gfield[mapX][mapY] == 1) {
            hitWall = true;
          } else {
            rayX += cos(rayAngle * PI / 180.0) * 0.1;
            rayY += sin(rayAngle * PI / 180.0) * 0.1;
            rayDistance += 0.1;
          }
        }
        int wallHeight = (int)(128.0 / rayDistance);
        int wallTop = (128 - wallHeight) / 2;
        int wallBottom = 128 - wallTop;
        int wallColor = field.gfield[(int)rayX][(int)rayY] == 1 ? BLUE : GREEN;
        display.writeFastVLine(x, wallTop, wallHeight, wallColor);
      }
    }

class GameIntro
{ 
  public:
    unsigned long FirstTriangles() {
      unsigned long start;
      int           n, i, cx = display.width()  / 2 - 1,
                          cy = display.height() / 2 - 1;

      display.fillScreen(BLACK);
      n     = min(cx, cy);
      start = micros();
      for(i=0; i<n; i+=5) {
        display.drawTriangle(
          cx    , cy - i, // peak
          cx - i, cy + i, // bottom left
          cx + i, cy + i, // bottom right
          display.color565(i, i, i));
      }
  
      return micros() - start;
    } 

    unsigned long FilledTriangles() {
      unsigned long start, t = 0;
      int           i, cx = display.width()  / 2 - 1,
                    cy = display.height() / 2 - 1;

    start = micros();
    for(i=min(cx,cy); i>10; i-=5) {
      start = micros();
      display.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      display.color565(i*1, i*1, i*1));
      t += micros() - start;
      display.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      display.color565(i*2, i*2, i*2));
       delay(100);
      yield();
    }
 
    return t;
  }

    unsigned long IntroText()
    { 
      unsigned long start = micros();
      display.setCursor(0, 40);
      display.setTextColor(WHITE);    
      display.setTextSize(3);
      display.println(" Ghost");

      display.setCursor(0, 70);
      display.setTextColor(WHITE);    
      display.setTextSize(3);
      display.println("Station");

      return micros() - start;
    }
};

 // create the game field and player objects
  GameField gamefield;
  Player player;
  GameIntro intro;


void setup() {
  display.begin();
  display.fillScreen(BLACK);

   // intro animation with text at the end
  intro.FirstTriangles();
  delay(500);

  intro.FilledTriangles();
  delay(500);

  intro.IntroText();
  delay(1000);
  display.fillScreen(BLACK);
}

void loop() {

  // begin writing to the screen
  display.startWrite();


  // draw the walls of the game world
  drawWalls(player, gamefield);


  // update player position and angle based on input
  player.updatePosition(gamefield);

  // end writing to the screen
  display.endWrite();

  // delay to control the frame rate
  delay(20);
}

