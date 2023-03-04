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


#define BUTTON_LEFT_PIN 2
#define BUTTON_RIGHT_PIN 3
#define BUTTON_FORWARD_PIN 4
#define BUTTON_BACKWARD_PIN 5
#define BUTTON_UP_PIN 6
#define BUTTON_DOWN_PIN 7



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
Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_CS, OLED_DC, OLED_RESET); 



const int MAP_WIDTH = 8;
const int MAP_HEIGHT = 8;

int gamefield[MAP_WIDTH][MAP_HEIGHT] = {
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 0, 0, 0, 1},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 1, 1, 1, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1}
};

// player position and direction
float playerX = 3.5;
float playerY = 3.5;
float playerAngle = 0.0;


void setup() {
  tft.begin();
  tft.fillScreen(BLACK);
}

void loop() {

  // begin writing to the screen
  tft.startWrite();

  // draw the walls of the game world
  for (int x = 0; x < 128; x++) {
    float rayAngle = playerAngle - 30.0 + (float)x / 128.0 * 60.0;
    float rayX = playerX;
    float rayY = playerY;
    float rayDistance = 0.0;
    bool hitWall = false;
    while (!hitWall && rayDistance < 10.0) {
      int mapX = (int)rayX;
      int mapY = (int)rayY;
      if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
        hitWall = true;
        rayDistance = 10.0;
      } else if (gamefield[mapX][mapY] == 1) {
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
    int wallColor = gamefield[(int)rayX][(int)rayY] == 1 ? BLUE : GREEN;
    tft.writeFastVLine(x, wallTop, wallHeight, wallColor);
  }

  // end writing to the screen
  tft.endWrite();

  // read input from joystick or other input device
  int joystickX = analogRead(A0);
  int joystickY = analogRead(A1);

  // update player position and angle based on input
  playerAngle += (float)(joystickX - 512) / 1024.0 * 5.0;
  playerX += cos(playerAngle * PI / 180.0) * (float)(joystickY - 512) / 1024.0 * 0.1;
  playerY += sin(playerAngle * PI / 180.0) * (float)(joystickY - 512) / 1024.0 * 0.1;

  // ensure player does not move through walls
  if (gamefield[(int)playerX][(int)playerY] == 1) {
    playerX -= cos(playerAngle * PI / 180.0) * (float)(joystickY - 512) / 1024.0 * 0.1;
    playerY -= sin(playerAngle * PI / 180.0) * (float)(joystickY - 512) / 1024.0 * 0.1;
  }

  // delay to control the frame rate
  delay(20);
}