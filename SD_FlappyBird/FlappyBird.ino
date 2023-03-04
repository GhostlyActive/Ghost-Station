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

const int birdSize = 8;
const uint8_t bird[] PROGMEM = {
  B00011000,
  B00111100,
  B01111110,
  B11111111,
  B11111111,
  B01111110,
  B00111100,
  B00011000
};
int birdX = 20;
int birdY = SCREEN_HEIGHT / 2 - birdSize / 2;
int previousBirdY = birdY;
int buttonPin = 2;
const int gravity = 2;
const float jumpSpeed = 5;

const int obstacleWidth = 10;
int obstacleX = 128;
int obstacleY = 64;
const int obstacleGap = 40;
const float obstacleSpeed = 3;

void setup() {
  Serial.begin(9600);
  display.begin();
  display.fillScreen(BLACK);
  randomSeed(analogRead(A0));
  pinMode(buttonPin, INPUT_PULLUP);
}

void drawBird() {
  // delete previous bird track for performance optimization
  display.fillRect(birdX, previousBirdY, birdSize, birdSize, BLACK);

  // draw bird
  display.drawBitmap(birdX, birdY, bird, birdSize, birdSize, YELLOW);

  // previous position of the bird
  previousBirdY = birdY;
}

void drawObstacle() {
  // delete area of the screen occupied by the obstacle
  display.fillRect(obstacleX - obstacleSpeed, 0, obstacleWidth + obstacleSpeed * 2, 128, BLACK);
  
  // draw obstacles
  display.fillRect(obstacleX, 0, obstacleWidth, obstacleY, WHITE);
  display.fillRect(obstacleX, obstacleY + obstacleGap, obstacleWidth, 128 - obstacleY - obstacleGap, WHITE);
}

void gameOver() {
  display.fillScreen(BLACK);
  display.setCursor(20, 50);
  display.setTextColor(WHITE);
  display.print("Game Over!");
  while (true) {}
}

void loop() {
  // delete bird track
  display.fillRect(birdX, previousBirdY, birdSize, birdSize, BLACK);

  // move bird
  if (digitalRead(buttonPin) == LOW) { //if low -> fly bird
    previousBirdY = birdY;
    birdY -= jumpSpeed;
  } else {
    birdY += gravity;
  }

  // check collision with upper or lower edge of the screen
  if (birdY < 0 || birdY + birdSize > 128) {
    gameOver();
  }

  // move obstacles
  obstacleX -= obstacleSpeed;

  // check if the bird has touched the obstacle
  if (obstacleX < birdX + birdSize && obstacleX + obstacleWidth > birdX && (birdY < obstacleY || birdY + birdSize > obstacleY + obstacleGap)) {
    gameOver();
  }

  // check if the obstacle has left the screen
  if (obstacleX < -obstacleWidth) {
    // create new obstacles
    obstacleX = 128;
    obstacleY = random(20, 108 - obstacleGap);
  }

  // draw bird
  drawBird();

  // draw obstacles
  drawObstacle();

  // small delay
  delay(20);
}

