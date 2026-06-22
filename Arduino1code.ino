// Arduino 1 code (Motor 1, LCD and buttons)

#include <LiquidCrystal.h>
#include <Stepper.h>

/*
Setting up Pins for LCD, Motor 1 and buttons
*/
LiquidCrystal lcd(9, 8, 7, 6, 5, 4); // RS=9, EN=8, D4=7, D5=6, D6=5, D7=4

const int STEPS_PER_REV = 1700;  // change this depending on how much to turn to dispense 1tsp
const int MOTOR_RPM = 10;        // Speed of motor
Stepper myStepper(STEPS_PER_REV, 10, 12, 11, 13);

const int buttonA = 2;  // cycle button
const int buttonB = 3;  // confirm button

/*
Communication pins
*/
#define MOTOR_2_PIN A0  // signal pin to tell Arduino 2 to spin Motor 2
#define MOTOR_3_PIN A1  // signal pin to tell Arduino 2 to spin Motor 3


/*
Setting up variables needed throughout whole code 
*/
int programState = 0;          // 0 = spice selection, 1 = amount selection, 2 = dispensing.
int currentDisplayMode = 0;    // 0 = Spice 1, 1 = Spice 2, 2 = Spice 3. Used to switch between what motor is being used. 
int amount = 1;                // amout = number of teaspoons, set to 1 teaspoon per rotation.
double teaspoons = 1.0;        // amount of teaspoons, displayed to the user.
String selectedSpice = "Spice"; // Initializing slectedSpice as "Spice", First choice.

// 60 seconds / 10 RPM = 6 seconds per rev = 6000ms, this is used for delaying signals sent to arduino 2. 
const long TIME_PER_REV = 60000 / MOTOR_RPM; 

/*
LCD screen code
*/
void displayWelcome() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome :)"); // Welcome screen
  lcd.setCursor(0, 1);
  lcd.print("Choose a spice");
}

void displaySpice(int mode) {
  lcd.clear();
  switch (mode) {
    case 0:
      lcd.print("Spice 1");
      lcd.setCursor(0, 1);
      lcd.print("Dill Weed");
      selectedSpice = "Dill Weed"; 
      break;
    case 1:
      lcd.print("Spice 2");
      lcd.setCursor(0, 1);
      lcd.print("Stevia"); 
      selectedSpice = "Stevia"; 
      break;
    case 2:
      lcd.print("Spice 3");
      lcd.setCursor(0, 1);
      lcd.print("Sage"); 
      selectedSpice = "Sage"; 
      break;
  }
}
/*
spice slection function ------------------------------------------------------------
*/
void spiceSelection() {
  // this is for the confirmation
  if (digitalRead(buttonB)==LOW) { // if button b pressed, moves onto next stage
      lcd.clear();
      programState = 1; 
      delay(200);
      return; // 
  }

  if (digitalRead(buttonA) == LOW) {// if button a pressed, cycles through options
    delay(250); // debounce
    currentDisplayMode = (currentDisplayMode + 1) % 3; // Cycles 0, 1, 2, cycles through spices.
    displaySpice(currentDisplayMode);
  }
}

/*
amount selection Function ------------------------------------------------------------
*/
void amountSelection() {
  // Display current amount
  lcd.setCursor(0, 0);
  lcd.print("Amount       ");
  lcd.setCursor(0, 1);
  lcd.print(teaspoons); // teaspoons is initialized as 1, so min value dispensed is 1 tsp.
  lcd.print(" tsp       ");

  // checks if ONLY button A is being pressed. 
  if (digitalRead(buttonA) == LOW && digitalRead(buttonB)!=LOW) {
    delay(250); // Debounce / delay to check for a hold

    if (digitalRead(buttonA) == LOW) { // if button is still being pressed after 250ms, then assumes user is holding button. Manual dispensing
      // manual dispensing logic
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Manual Spin...");
      while (digitalRead(buttonA) == LOW) {
        // logic to determine which motor is spun.
        switch(currentDisplayMode) {
          case 0: // Motor 1
            myStepper.step(40); // small step ONLY if button is held
            break;
          case 1: // Motor 2
            digitalWrite(MOTOR_2_PIN, HIGH); // sends signal to arduino 2 to spin motor 2/3
            delay(50); // signal is on for 50ms
            digitalWrite(MOTOR_2_PIN, LOW); // turns signal off. 
            delay(50); 
            break;
          case 2: // Motor 3
            digitalWrite(MOTOR_3_PIN, HIGH); 
            delay(50); 
            digitalWrite(MOTOR_3_PIN, LOW);
            delay(50);
            break;
        }
      }
      
    } else { //else, if the button is not held down, it is tapped.
      // short press = increase amount
      amount++;
      teaspoons = amount;
    }
    
    // after tap or hold, clear and re-display
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Amount");
    lcd.setCursor(0, 1);
    lcd.print(teaspoons);
    lcd.print(" tsp");
  }

  if (teaspoons >= 0.5){ // if teaspoons is not less than 0, prevents decreasing to a negative teaspoon value.
    if (digitalRead(buttonB) == LOW && digitalRead(buttonA) != LOW){ // if button B pressed, decreases amount.
      delay(250);
      amount--;
      teaspoons = amount;
      lcd.clear(); // prints screen again with updated info.
      lcd.setCursor(0, 0);
      lcd.print("Amount");
      lcd.setCursor(0, 1);
      lcd.print(teaspoons);
      lcd.print(" tsp");
    }
  }
  
  // if button B held down, confirm, moves to next stage (dispensing)
 if (digitalRead(buttonB) == LOW ){
    delay(300);
    if (digitalRead(buttonB) == LOW ){    
      delay(250); // debounce
      lcd.clear(); // clears LCD for next stage in code.
      programState = 2; // moves to next stage of code, amount selection
    }
  }
}

/*
Dispensing funtion ------------------------------------------------------------
*/
void dispensing(int spins) { // 'spins' is the 'amount', number of teaspoons
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispensing...");
  lcd.setCursor(0, 1);
  lcd.print(teaspoons);
  lcd.print(" tsp ");
  lcd.print(selectedSpice);

  int stepsPerTsp = STEPS_PER_REV; 

  for (int i = 1; i <= spins; i++) { // until i is equal to the number of spins
    
    // Logic to choose which motor to spin.
    switch (currentDisplayMode) {
      case 0: 
        myStepper.step(stepsPerTsp);
        break;
        
      case 1: // Spice 2: Send signal for Motor 2
        digitalWrite(MOTOR_2_PIN, HIGH);
        delay(100); // Signal pulse
        digitalWrite(MOTOR_2_PIN, LOW);
        delay(TIME_PER_REV); // delay the next signal from the analog pins for TIME_PER_REV millisenconds (6000ms = 6s). Prevents signal from being sent while motor is spinning.
        break;
        
      case 2: // Spice 3: Send signal for Motor 3
        digitalWrite(MOTOR_3_PIN, HIGH);
        delay(100);                     
        digitalWrite(MOTOR_3_PIN, LOW);
        delay(TIME_PER_REV);
        break;
    }
    
    delay(500); // 0.5s pause between spins, indicates when a teaspoon has been dispensed and prevents overheating
  }

  lcd.clear();
  lcd.print("Done!");
  delay(1500);
  lcd.clear();

  // Resets all variables for next round
  amount = 1;
  teaspoons = 1;
  programState = 0;
  displaySpice(currentDisplayMode); // Start on the last-used spice
}


void setup() {
  myStepper.setSpeed(MOTOR_RPM); // RPM for Motor 1
  Serial.begin(9600);
  
  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);

  // set up analog pins as outputs (sends signals to arduino 2)
  pinMode(MOTOR_2_PIN, OUTPUT);
  pinMode(MOTOR_3_PIN, OUTPUT);
  // making sure pins begin as LOW, so that motors do not randomly go off.
  digitalWrite(MOTOR_2_PIN, LOW);
  digitalWrite(MOTOR_3_PIN, LOW);

  lcd.begin(16, 2);
  displayWelcome(); // calling welcome screen
  delay(3000); // displays welcome screen for 3 sec
  displaySpice(0);
}

// -------------------- MAIN LOOP --------------------
void loop() {
  switch (programState) {
    case 0:
      spiceSelection();
      break;
    case 1:
      amountSelection();
      break;
    case 2:
      dispensing(amount);
      break;
  }
}
