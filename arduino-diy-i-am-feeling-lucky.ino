const int led1Pin = 7; //Red
const int led2Pin = 8; //Green
const int buttonPin = 6; //Button to ground

const long interval_normal = 900000; //how long it takes to go green
const long interval_win = 120000; //when win happens go green in this many seconds
const int luck_normal = 200; //what are the chances when button is green to win
const int luck_win = 3; //what are the chances when button is green to win after you had a win
const int win_duration = 3; //How long does the streak last after the win

unsigned long interval = interval_normal; 
unsigned long previousMillis = 0;
unsigned int luck = luck_normal;
unsigned int duration = 0;
bool ledsBlinking = false;
bool buttonPressed = false;

void setup() {
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  randomSeed(analogRead(0)); // Initialize random seed
  previousMillis = millis();
}

void loop() {
  unsigned long currentMillis = millis();
  if (!ledsBlinking) {  //Red light on
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, LOW);
    if (currentMillis - previousMillis >= interval) { // After time passed allow to press button
      previousMillis = currentMillis;
      ledsBlinking = true; 
    }
    if (digitalRead(buttonPin) != LOW) { // Prevent cheating by button being held down
      buttonPressed = false;
    } else {
      previousMillis = currentMillis; 
    }
  } else { //Green light, you can try your  luck
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, HIGH);
    if (digitalRead(buttonPin) == LOW && !buttonPressed) {  // Check if the button is pressed

      int randomValue = random(luck); // Generate a random number 
      if (randomValue == 0) { // Did you win?

        if (duration < win_duration) { //Are we on a winning streak
          duration = duration + 1;
          luck = luck_win;
          interval = interval_win;
        } else { //Winning streak is over, back to normal.
          duration = 0;
          luck = luck_normal;
          interval = interval_normal;
        }
        for (int i = 0; i < 20; i++) { //Flash for the win
          digitalWrite(led1Pin, LOW);
          digitalWrite(led2Pin, LOW);
          delay(100); 
          digitalWrite(led2Pin, HIGH);
          delay(100); 
          digitalWrite(led1Pin, HIGH);
          delay(100); 
        }
      } else {
        digitalWrite(led2Pin, LOW); 
        for (int i = 0; i < 6; i++) { //Flash for the loss
          digitalWrite(led1Pin, LOW);
          delay(100); 
          digitalWrite(led1Pin, HIGH);
          delay(100);
        }
      }
      ledsBlinking = false; //Go back to Red light
      previousMillis = millis();
      buttonPressed = true; // Button pressed, prevent further presses for 10 minutes
    }
  }
}
