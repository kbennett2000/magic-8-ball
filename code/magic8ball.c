#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
const int buttonPin = 2;
bool buttonPressed = false;
unsigned long answerStartTime = 0;
bool initialLoad = true;

const char* answers[] = {
    "Yes", 
    "No", 
    "Maybe", 
    "Why are you     asking me?",
    "Are you always  this boring?",
    "Oh my God!      Who cares???",
    "Are you fucking CRAZY???",
    "That's a stupid question!",
    "Eh, why not?",
    "What do I look  like a magic 8 ball?",
    "How would I     know?",
    "For $100 I can  make it happen...",
    "Fuck if I know  ask again later",
    "Your mom's      opinion is: Yes",
    "Shit, no way,   try again",
    "Eh, who gives a fuck?",
    "Balls to the    wall, why not?",
    "Doubt it, but   what do I care?",
    "Hell yeah, or   maybe not",
    "Piss off, you   figure it out",
    "Sure, if pigs   could fly",
    "Nope, but keep  dreaming",
    "Maybe if I gave a shit",
    "Yes, in another dimension",
    "Fuck no!",
    "Signs point to  go fuck yourself",
    "Try again, or   don't, whatever",
    "Ask your balls, they might know",
    "You're fucked,  good luck",
    "Not in this     lifetime, pal",
    "Only if you     stop asking",
    "Ask me when I   care",
    "Nope, but your  mom says yes",
    "Better luck next time, loser",
    "Fuck around and find out",
    "Yeah, right     after hell freezes"   
};

int numAnswers = sizeof(answers) / sizeof(answers[0]);

const unsigned char eightBall [] PROGMEM = {
  0x07, 0xE0,
  0x1F, 0xF8,
  0x3F, 0xFC,
  0x7F, 0xFE,
  0x7F, 0xFE,
  0x7B, 0xDE,
  0x7B, 0xDE,
  0x7B, 0xDE,
  0x7B, 0xDE,
  0x7B, 0xDE,
  0x7B, 0xDE,
  0x7F, 0xFE,
  0x7F, 0xFE,
  0x3F, 0xFC,
  0x1F, 0xF8,
  0x07, 0xE0
};

const unsigned char ballRoll1 [] PROGMEM = {
  0x00, 0x07, 0xE0, 0x00,
  0x00, 0x1F, 0xF8, 0x00,
  0x00, 0x3F, 0xFC, 0x00,
  0x00, 0x7F, 0xFE, 0x00,
  0x00, 0xFF, 0xFF, 0x00,
  0x01, 0xFF, 0xFF, 0x80,
  0x03, 0xFF, 0xFF, 0xC0,
  0x07, 0xFF, 0xFF, 0xE0,
  0x0F, 0xFF, 0xFF, 0xF0,
  0x0F, 0xE0, 0x07, 0xF0,
  0x1F, 0xC0, 0x03, 0xF8,
  0x1F, 0xC7, 0xE3, 0xF8,
  0x1F, 0xCF, 0xF3, 0xF8,
  0x1F, 0xCF, 0xF3, 0xF8,
  0x1F, 0xCF, 0xF3, 0xF8,
  0x1F, 0xCF, 0xF3, 0xF8,
  0x1F, 0xC7, 0xE3, 0xF8,
  0x1F, 0xC0, 0x03, 0xF8,
  0x1F, 0xE0, 0x07, 0xF8,
  0x0F, 0xFF, 0xFF, 0xF0,
  0x0F, 0xFF, 0xFF, 0xF0,
  0x07, 0xFF, 0xFF, 0xE0,
  0x03, 0xFF, 0xFF, 0xC0,
  0x01, 0xFF, 0xFF, 0x80,
  0x00, 0xFF, 0xFF, 0x00,
  0x00, 0x7F, 0xFE, 0x00,
  0x00, 0x3F, 0xFC, 0x00,
  0x00, 0x1F, 0xF8, 0x00,
  0x00, 0x07, 0xE0, 0x00
};

const unsigned char ballRoll2 [] PROGMEM = {
  0x00, 0x07, 0xE0, 0x00,
  0x00, 0x1F, 0xF8, 0x00,
  0x00, 0x3F, 0xFC, 0x00,
  0x00, 0x7F, 0xFE, 0x00,
  0x00, 0xFF, 0xFF, 0x00,
  0x01, 0xFF, 0xFF, 0x80,
  0x03, 0xFF, 0xFF, 0xC0,
  0x07, 0xE0, 0x07, 0xE0,
  0x0F, 0xC0, 0x03, 0xF0,
  0x0F, 0xC7, 0xE3, 0xF0,
  0x1F, 0xCF, 0xF3, 0xF8,
  0x1F, 0xCF, 0xF3, 0xF8,
  0x1F, 0xCF, 0xF3, 0xF8,
  0x1F, 0xCF, 0xF3, 0xF8,
  0x1F, 0xC7, 0xE3, 0xF8,
  0x1F, 0xC0, 0x03, 0xF8,
  0x1F, 0xE0, 0x07, 0xF8,
  0x0F, 0xFF, 0xFF, 0xF0,
  0x0F, 0xFF, 0xFF, 0xF0,
  0x07, 0xFF, 0xFF, 0xE0,
  0x03, 0xFF, 0xFF, 0xC0,
  0x01, 0xFF, 0xFF, 0x80,
  0x00, 0xFF, 0xFF, 0x00,
  0x00, 0x7F, 0xFE, 0x00,
  0x00, 0x3F, 0xFC, 0x00,
  0x00, 0x1F, 0xF8, 0x00,
  0x00, 0x07, 0xE0, 0x00
};

const unsigned char ballRoll3 [] PROGMEM = {
  0x00, 0x07, 0xE0, 0x00,
  0x00, 0x1F, 0xF8, 0x00,
  0x00, 0x3F, 0xFC, 0x00,
  0x00, 0x7F, 0xFE, 0x00,
  0x00, 0xFF, 0xFF, 0x00,
  0x01, 0xE0, 0x07, 0x80,
  0x03, 0xC0, 0x03, 0xC0,
  0x07, 0xC7, 0xE3, 0xE0,
  0x0F, 0xCF, 0xF3, 0xF0,
  0x0F, 0xCF, 0xF3, 0xF0,
  0x1F, 0xCF, 0xF3, 0xF8,
  0x1F, 0xCF, 0xF3, 0xF8,
  0x1F, 0xC7, 0xE3, 0xF8,
  0x1F, 0xC0, 0x03, 0xF8,
  0x1F, 0xE0, 0x07, 0xF8,
  0x0F, 0xFF, 0xFF, 0xF0,
  0x0F, 0xFF, 0xFF, 0xF0,
  0x07, 0xFF, 0xFF, 0xE0,
  0x03, 0xFF, 0xFF, 0xC0,
  0x01, 0xFF, 0xFF, 0x80,
  0x00, 0xFF, 0xFF, 0x00,
  0x00, 0x7F, 0xFE, 0x00,
  0x00, 0x3F, 0xFC, 0x00,
  0x00, 0x1F, 0xF8, 0x00,
  0x00, 0x07, 0xE0, 0x00
};

const unsigned char ballRoll4 [] PROGMEM = {
  0x00, 0x07, 0xE0, 0x00,
  0x00, 0x1F, 0xF8, 0x00,
  0x00, 0x3F, 0xFC, 0x00,
  0x00, 0x7E, 0x7E, 0x00,
  0x00, 0xFC, 0x3F, 0x00,
  0x01, 0xFC, 0x3F, 0x80,
  0x03, 0xFC, 0x3F, 0xC0,
  0x07, 0xFC, 0x3F, 0xE0,
  0x0F, 0xFC, 0x3F, 0xF0,
  0x0F, 0xFC, 0x3F, 0xF0,
  0x1F, 0xF8, 0x1F, 0xF8,
  0x1F, 0xF0, 0x0F, 0xF8,
  0x1F, 0xE0, 0x07, 0xF8,
  0x1F, 0xE0, 0x07, 0xF8,
  0x1F, 0xFF, 0xFF, 0xF8,
  0x0F, 0xFF, 0xFF, 0xF0,
  0x0F, 0xFF, 0xFF, 0xF0,
  0x07, 0xFF, 0xFF, 0xE0,
  0x03, 0xFF, 0xFF, 0xC0,
  0x01, 0xFF, 0xFF, 0x80,
  0x00, 0xFF, 0xFF, 0x00,
  0x00, 0x7F, 0xFE, 0x00,
  0x00, 0x3F, 0xFC, 0x00,
  0x00, 0x1F, 0xF8, 0x00,
  0x00, 0x07, 0xE0, 0x00
};

void screenTransition(bool fadeIn) {
  int steps = 16;
  for(int i = 0; i < steps; i++) {
    display.ssd1306_command(SSD1306_SETCONTRAST);
    if (fadeIn) {
      display.ssd1306_command(i * (255/steps));
    } else {
      display.ssd1306_command(255 - (i * (255/steps)));
    }
    delay(100);
  }
  if (fadeIn) {
    display.ssd1306_command(SSD1306_SETCONTRAST);
    display.ssd1306_command(255);
  }
}

void slideTransition(bool slideIn) {
  int steps = 24;  // Reduced from 32 for faster animation
  for(int i = 0; i <= steps; i++) {
    display.clearDisplay();
    if (slideIn) {
      drawEightBall(6 - (steps-i)*5, 8);  // Increased movement per step from 4 to 5
    } else {
      drawEightBall(6 + i*5, 8);
    }
    display.display();
    delay(30);  // Reduced from 50ms for faster animation
  }
}

void showInitialScreen() {
  if (initialLoad) {
    // Add a ground line for the ball to roll on
    uint8_t frame = 0;
    
    // Rolling animation
    for(int x = -32; x <= SCREEN_WIDTH; x += 2) {
      display.clearDisplay();
      
      // Draw ground line
      display.drawFastHLine(0, 31, SCREEN_WIDTH, SSD1306_WHITE);
      
      // Draw rolling ball
      drawRollingBall(x, 2, frame);
      
      display.display();
      frame++;
      delay(30);
    }
    
    delay(500);
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(1, 1);
    display.println(F("  Magic 8    Ball"));  
    display.display();
    delay(2000);
    screenTransition(true);
    initialLoad = false;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(24, 4);
  display.println(F(" --Magic 8 Ball--"));  
  display.println(F("     Ask me anything"));
  display.println(F("    Then press button"));
  drawEightBall(6, 8);
  display.display();
  screenTransition(true);
}

void drawEightBall(int16_t x, int16_t y) {
  display.drawBitmap(x, y, eightBall, 16, 16, SSD1306_WHITE);
}

void shakeAnimation() {
  const int shakeFrames = 30;
  const int shakeAmount = 6;
  
  for(int i = 0; i < shakeFrames; i++) {
    display.clearDisplay();
    int16_t offsetX = random(-shakeAmount, shakeAmount);
    int16_t offsetY = random(-shakeAmount, shakeAmount);
    drawEightBall(8 + offsetX, 8 + offsetY);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(24, 4);
    display.println(F("   Shaking..."));  
    display.display();
    delay(50);
  }
}

void showAnswer(const char* answer, bool withTransition = true) {
  if (withTransition) {
    slideTransition(true);
  }
  
  display.clearDisplay();
  drawEightBall(6, 8);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(28, 4);
  display.println(F("The answer is:"));
  display.setCursor(28, 16);
  display.println(answer);
  display.display();
  
  if (withTransition) {
    screenTransition(true);
  }
  
  answerStartTime = millis();
}

void generateNewAnswer() {
  randomSeed(analogRead(A0) * millis());
  
  display.setTextSize(1);
  shakeAnimation();
  
  int randomIndex = random(0, numAnswers);
  showAnswer(answers[randomIndex]);
}

void drawRollingBall(int16_t x, int16_t y, uint8_t frame) {
  const unsigned char* ball;
  switch(frame % 4) {
    case 0:
      ball = ballRoll1;
      break;
    case 1:
      ball = ballRoll2;
      break;
    case 2:
      ball = ballRoll3;
      break;
    case 3:
      ball = ballRoll4;
      break;
  }
  display.drawBitmap(x, y, ball, 32, 29, SSD1306_WHITE);
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  
  long randomSeedValue = 0;
  for(int i = 0; i < 8; i++) {
    randomSeedValue = randomSeedValue * 4 + analogRead(A0);
    delay(1);
  }
  randomSeed(randomSeedValue);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {    
    for(;;);
  }
  
  showInitialScreen();
}

void loop() {
  unsigned long currentTime = millis();
  
  if (digitalRead(buttonPin) == LOW) {
    if (!buttonPressed) {
      buttonPressed = true;
      generateNewAnswer();  // Always generate new answer when button is pressed
    }
  } else {
    buttonPressed = false;
  }
  
  // Only auto-return to initial screen if no button press interrupted it
  if (answerStartTime > 0 && currentTime - answerStartTime >= 10000 && !buttonPressed) {
    screenTransition(false);
    slideTransition(false);
    showInitialScreen();
    answerStartTime = 0;
  }
}
