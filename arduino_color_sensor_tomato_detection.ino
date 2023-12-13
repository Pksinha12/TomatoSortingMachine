#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define SENSOR_OUT 8
#define BUZZER 9
#define BELT_CLK 10
#define BELT_ACLK 11
#define HAND_CLK 12
#define HAND_ACLK 13
#define IR_1 A0
#define IR_2 A1
#define IR_3 A2
#define DEBUG false
void setup() {
  Serial.begin(9600);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(SENSOR_OUT, INPUT);
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  pinMode(IR_3, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BELT_CLK, OUTPUT);
  pinMode(BELT_ACLK, OUTPUT);
  pinMode(HAND_CLK, OUTPUT);
  pinMode(HAND_ACLK, OUTPUT);

  // Setting frequency-scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  digitalWrite(BUZZER, LOW);
  digitalWrite(BELT_CLK, LOW);
  digitalWrite(BELT_ACLK, LOW);
  digitalWrite(HAND_CLK, LOW);
  digitalWrite(HAND_ACLK, LOW);
  if (DEBUG) {
    Serial.println("Initialized");
  }
  delay(200);
}
int red, green, blue, samples = 5;
String color;
void loop() {
  if (!digitalRead(IR_1)) {
    if (DEBUG) {
      Serial.println("Sensed an object on color sensor");
    }
    int red_count = 0, blue_count = 0, green_count = 0;
    while (true) {
      if (digitalRead(IR_1)) {
        return;
      }

      readRGB();
      getColor();

      if (color == "RED") {
        red_count += 1;
      } else if (color == "GREEN") {
        green_count += 1;
      } else if (color == "BLUE") {
        blue_count += 1;
      }
      if (red_count > samples || green_count > samples || blue_count > samples) {
        doOperation();
        break;
      }
    }
    delay(100);
  }
}
void doOperation() 
{
  if (color == "RED") 
  {
    beep(1);
    moveBelt("clk");
    while (digitalRead(IR_2))
      ;
    moveBelt("stop");
    beep(1);
    Serial.println("1");
    while (true) 
    {
      char ch = Serial.read();
      if (ch != -1) 
      {
        if (ch == '1' || ch == '2') 
        {
          moveBelt("clk");
          while (digitalRead(IR_3))
            ;
          moveBelt("stop");
          beep(1);
          if (ch == '1') 
          {
            moveHand("clk");
            delay(2000);
            moveHand("stop");
            delay(100);
          } else if (ch == '2') 
          {
            moveHand("aclk");
            delay(2000);
            moveHand("stop");
            delay(100);
          }
          break;
        }
      }
      else
      {
        beep(1);
        moveBelt("aclk");
        delay(4000);
        moveBelt("stop");
        break;
      }
    }
  } 
  else if (color == "GREEN") 
  {
    beep(1);
    moveBelt("aclk");
    delay(2000);
    moveBelt("stop");
  }
}
void moveHand(String action) {
  if (action == "clk") {
    if (DEBUG) {
      Serial.println("Moving hand clockwise");
    }
    digitalWrite(HAND_CLK, HIGH);
    digitalWrite(HAND_ACLK, LOW);
  } else if (action == "aclk") {
    if (DEBUG) {
      Serial.println("Moving hand anti-clockwise");
    }
    digitalWrite(HAND_CLK, LOW);
    digitalWrite(HAND_ACLK, HIGH);
  } else if (action == "stop") {
    if (DEBUG) {
      Serial.println("hand stop");
    }
    digitalWrite(HAND_CLK, LOW);
    digitalWrite(HAND_ACLK, LOW);
  }
}

void moveBelt(String action) {
  if (action == "clk") {
    if (DEBUG) {
      Serial.println("moving belt clockwise");
    }
    digitalWrite(BELT_CLK, HIGH);
    digitalWrite(BELT_ACLK, LOW);
  } else if (action == "aclk") {
    
    if (DEBUG) {
      Serial.println("moving belt anti clockwise");
    }
    digitalWrite(BELT_CLK, LOW);
    digitalWrite(BELT_ACLK, HIGH);
  } else if (action == "stop") {
    
    if (DEBUG) {
      Serial.println("belt stopped");
    }
    digitalWrite(BELT_CLK, LOW);
    digitalWrite(BELT_ACLK, LOW);
  }
}
void beep(int times) {
  while (times--) {

    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);
    delay(100);
  }
}
void getColor() {

  if (red < green && red < blue) color = "RED";
  else if (green < red && green < blue) color = "GREEN";
  else if (blue < green && blue < red) color = "BLUE";
  else color = "NO_COLOR";

  if (DEBUG) {
    Serial.println(color);
  }
}
void readRGB() {
  // Setting red filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Reading the output frequency
  red = pulseIn(SENSOR_OUT, LOW);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  green = pulseIn(SENSOR_OUT, LOW);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  blue = pulseIn(SENSOR_OUT, LOW);

  if (DEBUG) {
    Serial.print("R= ");
    Serial.print(red);
    Serial.print("  ");
    Serial.print("G= ");
    Serial.print(green);
    Serial.print("  ");
    Serial.print("B= ");
    Serial.print(blue);
    Serial.println("  ");
  }
  delay(100);
}
