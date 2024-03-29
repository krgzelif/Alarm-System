
// HOME SECURITY ALARM SYSTEM

#include <LiquidCrystal.h> // for LCD we need to include this library
#include <Keypad.h> // for Keypad we need to include

// i define the pins of buzzer and ultrasonic sensor
#define buzzer 10
#define trigPin 11 //The part of the sensor that sends the sound wave.
#define echoPin 12 //The part of the sensor that receivers the sound wave.


//define some necessary variables for the program
long duration;
int distance, initialDistance, currentDistance, i;
int screenOffMsg =0;                 //screen of message
String password="1234";
String tempPassword;                 // for change password
boolean activated = false;           // Status of the alarm
boolean isActivated;
boolean activateAlarm = false;        // status of alarm activated
boolean alarmActivated = false;      
boolean enteredPassword;              // State of the entered password to stop the alarm
boolean passChangeMode = false;
boolean passChanged = false;


//for keypad
const byte ROWS = 4;         //four rows
const byte COLS = 4;         //four columns
char keypressed;
                             //define the cymbols on the buttons of the keypads
char keyMap[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //Row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //Column pinouts of the keypad

Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, ROWS, COLS); 
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 


// sets buzzer and ultrasonic
void setup() { 
  lcd.begin(16,2); //We also start the work with the Define the number of columns and rows of the LCD screen and lcd begin command
  pinMode(buzzer, OUTPUT); // Set buzzer as an output
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}


// Countdown code to activate alarm in 9 seconds
 //and print the countdown to the screen
void loop() {
  if (activateAlarm) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarm will be");
    lcd.setCursor(0,1);
    lcd.print("activated in");
   
    int countdown = 9; //we can change how many seconds the countdown is
    while (countdown != 0) {
      lcd.setCursor(13,1);
      lcd.print(countdown);
      countdown--;     //reduces the countdown
      tone(buzzer, 700, 100);    //beeps once every second
      delay(1000);
    }

     //When the countdown is over, a text informing that the alarm is active will appear on the screen.
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarm Activated!");
    initialDistance = getDistance();  //We measure the first distance to the objects in front of us with our alarm device.
    activateAlarm = false;            //activateAlarm boolean holds false value
    alarmActivated = true;            //alarmActivated boolean holds true value
  }
 // ultrasonic distance sensor constantly checks whether the measured distance is smaller than the initial distance, corrected with a value of 10 cm.
 //and if true it is enabled. The tone() function will activate the buzzer and the enterPassword() custom function will be called.
  if (alarmActivated == true){
      currentDistance = getDistance() + 10;
      if ( currentDistance < initialDistance) {
        tone(buzzer, 1000); //will activate the buzzer
        lcd.clear();
        enterPassword(); //The enterPassword custom function will be called.
      }
    }
//In the loop part, I first check if the alarm is active.
//If the alarm is not active, we will open the main screen of the program on the LCD, which offers two options, A to activate the alarm and B to change the password.
  if (!alarmActivated) {  
    if (screenOffMsg == 0 ){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("A - Activate");
      lcd.setCursor(0,1);
      lcd.print("B - Change Pass");
      screenOffMsg = 1;
    }
    keypressed = myKeypad.getKey();
     if (keypressed =='A'){        //If A is pressed, activate the alarm
      tone(buzzer, 1000, 200);
      activateAlarm = true;            
    }
    
    else if (keypressed =='B') {  // ıf B is pressed, change the password
      lcd.clear();
      int i=1;
      tone(buzzer, 2000, 100); // keypad button tone
      tempPassword = "";
      lcd.setCursor(0,0);
      lcd.print("Current Password"); //it will ask us to enter our current password
      lcd.setCursor(0,1);
      lcd.print(">");
      passChangeMode = true; //password change status becomes true
      passChanged = true;   
      while(passChanged) {      
      keypressed = myKeypad.getKey(); /// The code here will run whenever there is a key pressed
      if (keypressed != NO_KEY){  // The code here will run whenever there is a key pressed
        if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
            keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
            keypressed == '8' || keypressed == '9' ) {
         tempPassword += keypressed; //will print the pressed keys to the value we defined as the temporary password.
         lcd.setCursor(i,1);
         lcd.print("*");//It is printed as * because we do not want the entered numbers to appear.
         i++;
         tone(buzzer, 2000, 100);
        }
      }
       //If more than 5 keys are pressed or # key is pressed, the entered values will be reset and we will re-enter the password,
      //in short, it will clear the screen.
      if (i > 5 || keypressed == '#') { //If we enter more than 4 digits or press the # button, we can delete previously entered digits.
        tempPassword = "";
        i=1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Current Password");
        lcd.setCursor(0,1);
        lcd.print(">"); 
      }
       //If the * key is pressed, it will be checked whether the current password is the correct one.
      if ( keypressed == '*') { // password complate
        i=1;
        tone(buzzer, 2000, 100);
        if (password == tempPassword) {  //if the password is temporary password
          tempPassword="";
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Set New Password");  //A text will appear on the screen asking us to enter a new password.
          lcd.setCursor(0,1);
          lcd.print(">");
          while(passChangeMode) {
            keypressed = myKeypad.getKey();
            if (keypressed != NO_KEY){
              if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
                  keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
                  keypressed == '8' || keypressed == '9' ) {
                tempPassword += keypressed;
                lcd.setCursor(i,1);
                lcd.print("*");
                i++;
                tone(buzzer, 2000, 100);
              }
            }
            if (i > 5 || keypressed == '#') {
              tempPassword = "";
              i=1;
              tone(buzzer, 2000, 100);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Set New Password");
              lcd.setCursor(0,1);
              lcd.print(">");
            }
            if ( keypressed == '*') {
              i=1;
              tone(buzzer, 2000, 100);
              password = tempPassword;
              passChangeMode = false; //password change will complete so both booleans will go to false
              passChanged = false;
              screenOffMsg = 0;
            }            
          }
        }
      }
    }
   }
 }
}
//we will write the special function about entering password
//It gets added to the tempPassword variable every time a key is pressed.
void enterPassword() { 
  

  int k=5; // because 1234* (5 digit)
  tempPassword = "";
  activated = true;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" * ALARM * ");
  lcd.setCursor(0,1);
  lcd.print("Pass>");
      while(activated) {
      keypressed = myKeypad.getKey();
      if (keypressed != NO_KEY){
        if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
            keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
            keypressed == '8' || keypressed == '9' ) {
          tempPassword += keypressed;
          lcd.setCursor(k,1);
          lcd.print("*");
          k++;
        }
      }
      if (k > 9 || keypressed == '#') { 
       
        tempPassword = "";
        k=5;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" * ALARM * ");
        lcd.setCursor(0,1);
        lcd.print("Pass>");
      }
      if ( keypressed == '*') { //If we press the * button, we check if the password currently entered is the same as the password originally set.
        if ( tempPassword == password ) {
          activated = false; //When the password is entered correctly, the activation status of the alarm becomes false.
          alarmActivated = false;
          noTone(buzzer);
          screenOffMsg = 0; 
        }
           //When the password is entered incorrectly, 
        //a screen appears on the screen stating that the password is incorrect and re-entering it.
        else if (tempPassword != password) { 
          lcd.setCursor(0,1);
          lcd.print("Wrong! Try Again");
          delay(2000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(" * ALARM * ");
          lcd.setCursor(0,1);
          lcd.print("Pass>");
        }
      }    
    }
}
// Custom function for the Ultrasonic sensor
//for distance 
long getDistance(){
  //int i=10; (distance variable)
  
  //while( i<=10 ) {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration*0.034/2;
  //sumDistance += distance;
  //}
  //int averageDistance= sumDistance/10;

  //delay(2000); ==> Waiting time required so that the ultrasonic sensor does not ring immediately when it detects motion       
  return distance;

}





       const int waterSens= 13;
        int waterValue;
        void setup(){
        pinMode(waterSens, INPUT);
        }

         void loop() {
           waterValue= analogRead(waterSens);
           Serial.println(waterValue);
           if(waterValue<=100){
           digitalWrite(buzzer, LOW);

            }
          else {
          if (screenOffMsg == 0 ){
          digitalWrite(buzzer, HIGH);
   
            }
