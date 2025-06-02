//Timing game

const int DELAY = 200;

//For 8-bit shift register
const int LATCH_PIN = 11;      
const int CLOCK_PIN = 9;     
const int DATA_PIN = 12;     
byte leds = 0;


int selectedLED;
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 3, 4, 5, 6);
//          lcd(RS, E, D4, D5, D6, D7)
const int BUTTON_PIN = 13;

void setup() 
{

// set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  // lcd.print("Welcome");

  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);  
  pinMode(CLOCK_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  leds = 0;
  updateShiftRegister();

  Serial.begin(9600);

  Serial.print("PROGRAM BEGIN");

  
   

  instructions();
}


void loop() 
{
  //Pre-loop tasks
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GAME START");
  delay(1000);
  int totalScore = 0;
  for(int a = 0; a < 3; a++){
    //three spins
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ROUND ");
    lcd.print(a+1);


    totalScore += play();
  }
  lcd.clear();

  //Output Final Results
  finalResults(totalScore);
  
  delay(1000);


  //Start a new game
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press button to");
  lcd.setCursor(0, 1);
  lcd.print("play again...");

  while(digitalRead(BUTTON_PIN) == HIGH){
    //wait for input
  }
}

void updateShiftRegister(){
   digitalWrite(LATCH_PIN, LOW);
   shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, leds);
   digitalWrite(LATCH_PIN, HIGH);}
void instructions(){
  unsigned long waitTime;

  lcd.setCursor(0, 0);
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("the Cyclone");

  waitTime = millis();

  while(!(millis() - waitTime >= 2000)){
    //waiting...
    if(digitalRead(BUTTON_PIN) == LOW){
      lcd.clear();
      return; //exit early
    }
  }
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Aim for the");
  lcd.setCursor(0, 1);
  lcd.print("middle");

  waitTime = millis();

  while(!(millis() - waitTime >= 1500)){
    //waiting...
    if(digitalRead(BUTTON_PIN) == LOW){
      lcd.clear();
      return; //exit early
    }
  }
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("There are 3");
  lcd.setCursor(0, 1);
  lcd.print("rounds.");

  waitTime = millis();

  while(!(millis() - waitTime >= 1500)){
    //waiting...
    if(digitalRead(BUTTON_PIN) == LOW){
      lcd.clear();
      return; //exit early
    }
  }
  lcd.clear();



  lcd.setCursor(0, 0);
  lcd.print("Press the button");
  lcd.setCursor(0, 1);
  lcd.print("to start!");
  
  while(digitalRead(BUTTON_PIN) == HIGH){
      //wait for player to press button
  }
  lcd.clear();
  delay(500); //allow buffers to empty
  }

int play(){
   unsigned long onTime;

  int roundScore = 0;
  const int RED = 10;
  const int YELLOW = 30;
  const int GREEN = 50;

  const int SPEED = 75; //the lower this is, the harder the game is
  
  Serial.println("PLAYING");

  int i = 0;  
  while(true){ //scroll thru LED's infinitely
    if(!bitRead(leds,i)){//onlyif LED is off
      bitSet(leds, i); 
      updateShiftRegister();  //LED is on

      onTime = millis();//get time once led turned on
    } 

    if(digitalRead(BUTTON_PIN) == LOW){ //scrolling stops once player inputs
      // Serial.print("PRESS");
            selectedLED = i;
            break;
    }

    if(millis() - onTime >= SPEED){ //current time - led turn on time = time led was on
          // Serial.println("off");
          bitClear(leds, i);
          updateShiftRegister();
      //once led is off, move to next led
            i++; //next led
          if(i == 7){ //last led is 6 so repeat cycle
            i = 0;
          }
    }

  }
  Serial.println(selectedLED);

    //only highlight LED that player selected
    leds = 0;
    bitSet(leds, selectedLED);
    updateShiftRegister(); 
  
    // output which color it was
    lcd.setCursor(0, 0);
    lcd.print("You got ");
    switch(selectedLED){
      case 0:
      case 1:
      case 5:
      case 6: 
          lcd.print("red!");
          roundScore = RED;
          break;
      case 2:
      case 4:
          lcd.print("yellow!");
          roundScore = YELLOW;
          break;
      case 3:
          lcd.print("green!");
          roundScore = GREEN;
          break;
      default:
          lcd.print("XXXX");
          break;
    }

    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print('+');
    lcd.print(roundScore);
    lcd.print(" points!");
    delay(1500);


    leds = 0; //clear LEDS
    updateShiftRegister();
    return roundScore;
}


void finalResults(int totalScore){
   lcd.setCursor(0, 0);
    lcd.print("Final Score: ");
    lcd.print(totalScore);
    delay(500);
    lcd.setCursor(0, 1);

    if(totalScore == 150){
      lcd.print("A perfect game!");
      int strobe = 75;

      //make a nice flashing effect
      for(int i = 0; i < 20; i++){
        leds = 255; //all on
        updateShiftRegister();
        delay(strobe);
        leds = 0; // all off
        updateShiftRegister();
        delay(strobe);
      }
      //total time is (2*strobe) * 20 = 3000ms

    }
    else if(110 <= totalScore && totalScore < 150){
      lcd.print("Not too bad");
      int strobe = 120;

      //make a nice flashing effect
      for(int i = 0; i < 12; i++){
        leds = 0b01110111; //yellows on
        updateShiftRegister();
        delay(strobe);
        leds = 0; // all off
        updateShiftRegister();
        delay(strobe);
      }
      //total time is (2*strobe) * 12 = 2880ms
      
    }
    else if(50 <= totalScore && totalScore < 110){
      lcd.print("Shoddy work");

      int strobe = 160;
      for(int i = 0; i < 7; i++){
        leds = 0b01100011; //all reds on
        updateShiftRegister();
        delay(strobe);
        leds = 0; // all off
        updateShiftRegister();
        delay(strobe);
      }
      //total time is (2*strobe) * 9 = 2880ms
    }
    else{
      lcd.print("You need help :(");
      int strobe = 390;
      for(int i = 0; i < 4; i++){
        leds = 0b01000001; //outer reds on
        updateShiftRegister();
        delay(strobe);
        leds = 0; // all off
        updateShiftRegister();
        delay(strobe);
      }
      //total time is (2*strobe) * 4 = 3120ms
    } 
}
