// C++ code
//
#include <LiquidCrystal_I2C.h>
int DIST_MAX = 100;
int DIST_MIN = 10;
int CURRENT_DIST = 0;
int CURRENT_FREQ = 0;
int UPDATE_FREQ_BUZZER = 200;
int UPDATE_FREQ_LCD = 200;
int BUZZER_LED_FREQ_MAX = 1000;
int BUZZER_LED_FREQ_MIN = 10;
int buzzerSoundFreq = 200;
int trigPin = 8;
int echoPin = 9;
int buzzerPin = 2;
int ledPin = 3;
int duration = 0;

long begintimeUpdateBuzzer;
long begintimeFreqBuzzer;
long begintimeUpdateLCD;

LiquidCrystal_I2C lcd (0x27, 16, 2);

int isOn = 0;

void setup()
{
  Serial.begin(9600);
  lcd.init();
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  begintimeUpdateBuzzer = millis();
  begintimeUpdateLCD = millis();
  lcd.setCursor(0, 0);
  lcd.print("DISTANCE : ");
  lcd.setCursor(0, 1);
  lcd.print("FREQUENCY:");
}

void loop()
{
  
  if(millis() - begintimeUpdateBuzzer >= UPDATE_FREQ_BUZZER){
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
    CURRENT_DIST = duration / 58;
    CURRENT_FREQ = calculateFreq();
	begintimeUpdateBuzzer = millis();
  }
  if(millis() - begintimeUpdateLCD >= UPDATE_FREQ_LCD){
    lcd.setCursor(11, 0);
    lcd.print(CURRENT_DIST);
    lcd.print("cm");
    lcd.setCursor(11, 1);
    lcd.print(CURRENT_FREQ);
    lcd.print("ms");
    begintimeUpdateLCD = millis();
  }
  if(CURRENT_DIST > 100 || CURRENT_DIST < 10){
      digitalWrite(ledPin, LOW);
      noTone(buzzerPin);
      isOn = 0;
  }
  else{
    if(millis() - begintimeFreqBuzzer >= CURRENT_FREQ){
      if(isOn){
        digitalWrite(ledPin, LOW);
        noTone(buzzerPin);

        isOn = 0;
      }
      else{
        digitalWrite(ledPin, HIGH);
        tone(buzzerPin, buzzerSoundFreq, CURRENT_FREQ);
        begintimeFreqBuzzer = millis();
        isOn = 1;
      }
      begintimeFreqBuzzer = millis();
    }
  
  }
  
}

int calculateFreq(){
    float ratio = (long)(CURRENT_DIST - DIST_MIN) / (float)(DIST_MAX - DIST_MIN);
    return (int)(ratio * (BUZZER_LED_FREQ_MAX - BUZZER_LED_FREQ_MIN)) + BUZZER_LED_FREQ_MIN;
}