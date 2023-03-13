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

class Bird {
  public:
    Bird();

    void draw();
    void update();
    void setButtonPin(int pin);
    int getButtonPin();
    void setGravity(int gravity);
    void setJumpSpeed(float jumpSpeed);
    bool isOutOfBounds();

  private:
    int buttonPin;
    const uint8_t shape[8] PROGMEM = {
      B00011000,
      B00111100,
      B01111110,
      B11111111,
      B11111111,
      B01111110,
      B00111100,
      B00011000
    };
    int gravity;
    float jumpSpeed;

  public:
    int x;
    int y;
    int previousY;
    const int size = 8;

};

Bird::Bird() {
  buttonPin = 2;
  gravity = 2;
  jumpSpeed = 5;
  x = 20;
  y = SCREEN_HEIGHT / 2 - 8 / 2;
  previousY = y;
}

void Bird::setButtonPin(int pin) {
  buttonPin = pin;
}

int Bird::getButtonPin() {
  return buttonPin;
}

void Bird::setGravity(int gravity) {
  gravity = gravity;
}

void Bird::setJumpSpeed(float jumpSpeed) {
  jumpSpeed = jumpSpeed;
}

void Bird::draw() {
  // delete previous bird track for performance optimization
  display.fillRect(x, previousY, size, size, BLACK);

  // draw bird
  display.drawBitmap(x, y, shape, size, size, YELLOW);

  // previous position of the bird
  previousY = y;
}

void Bird::update() {
  // move bird
  if (digitalRead(buttonPin) == LOW) { //if low -> fly bird
    previousY = y;
    y -= jumpSpeed;
  } else {
    y += gravity;
  }
}

bool Bird::isOutOfBounds() {
  return y < 0 || y + size > 128;
}

class Obstacle {
  public:
    Obstacle();

    void draw();
    void update();
    void setSpeed(float speed);
    bool isOutOfBounds();
    void setXY(int x, int y);
    int getX();
    int getY();

  private:
    int x;
    int y;

  public:
    const int width = 10;
    const int gap = 40;
    float speed;
};

Obstacle::Obstacle() {
  speed = 3;
  x = 128;
  y = 64;
}

void Obstacle::setSpeed(float speed) {
  this->speed = speed;
}

void Obstacle::setXY(int x, int y)
{
  this->x = x;
  this->y = y;
}



void Obstacle::draw() {
  // delete area of the screen occupied by the obstacle
  display.fillRect(x - speed, 0, width + speed * 2, 128, BLACK);
  
  // draw obstacles
  display.fillRect(x, 0, width, y, WHITE);
  display.fillRect(x, y + gap, width, 128 - y - gap, WHITE);
}

void Obstacle::update() {
  x -= speed;
}

bool Obstacle::isOutOfBounds() {
  return x < -width;
}

int Obstacle::getX()
{
  return x;
}

int Obstacle::getY()
{
  return y;
}


class Game{

  public:
    Game();
    void gameOver();

};

    Game::Game() {

}

void Game::gameOver() {
  display.fillScreen(BLACK);
  display.setCursor(35, SCREEN_HEIGHT/2);
  display.setTextColor(WHITE);
  display.print("Game Over!");
  while (true) {}
}

Bird bird;
Obstacle obstacle;
Game game;

void setup() {
  Serial.begin(9600);
  display.begin();
  display.fillScreen(BLACK);
  randomSeed(analogRead(A0));
  pinMode(bird.getButtonPin(), INPUT_PULLUP);
  
}


void loop() {
  // delete bird track
  display.fillRect(bird.x, bird.previousY, bird.size, bird.size, BLACK);

  // update bird
  bird.update();

  // check collision with upper or lower edge of the screen
  if (bird.isOutOfBounds()) {
    game.gameOver();
  }

  // move obstacles
  obstacle.update();

  // check if the bird has touched the obstacle
  if (obstacle.getX() < bird.x + bird.size && obstacle.getX() + obstacle.width > bird.x && (bird.y < obstacle.getY() || bird.y + bird.size > obstacle.getY() + obstacle.gap)) {
    game.gameOver();
  }

  // check if the obstacle has left the screen
  if (obstacle.isOutOfBounds()) {
    // create new obstacles
    obstacle.setXY(128, random(20, 108 - obstacle.gap));
    
  }

  // draw bird
  bird.draw();

  // draw obstacles
  obstacle.draw();

  // small delay
  delay(20);
}