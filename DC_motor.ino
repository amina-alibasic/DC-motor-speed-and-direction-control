// L239D pins
int enA = 9;
int in1 = 8;
int in2 = 7;

// variables for speed and direction control
int speed = 0;
int flag;
String direction;
String stop;

// variables for speed measurement
volatile unsigned int counter = 0;
unsigned int RPM;

void setup() {
  // defining control pins as outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(2, INPUT);


  // initially the motor is turned off when the program runs
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  Serial.begin(9600); // setting up serial monitor communication

  attachInterrupt(0, rpm_fun, RISING);

  // Output
  Serial.println("Enter the speed and then direction of rotation");
  Serial.println("Speed range is [40, 255]");
  Serial.println("");
  Serial.println("Enter cw for clockwise, ccw for counter-clockwise.");

}

void loop() {

  if (Serial.available() > 0)
  {
    // initializing entered speed and saving it into a variable
    speed = Serial.readStringUntil(' ').toInt();
    // skip the space between entered speed and direction, and save direction into a variable
    direction = Serial.readStringUntil('\n');

    flag = 0; 
  }

  // function calls
  speedControl(speed);
  directionControl(direction);

  delay(200);   // Sample Time

  static uint32_t previousMillis;
  if (millis() - previousMillis >= 1000) {
    RPM = counter * 60; // this equation depends on the physical appearance of the disk that spins on the motor
    counter = 0;
    previousMillis += 1000;
  }


  Serial.print("Motor is running at ");
  Serial.print(RPM);
  Serial.println(" RPM");
  delay(1000); // output new RPM every second
}


void directionControl(String direction) {
  if (direction == "ccw") 
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }

  if (direction == "cw")
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }

  if (direction == "stop") { 
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
  }


}

:
void speedControl(int speed) {
  /* Motor won't run at less than 40 pwm. This depends on the type of the motor. */
  if (speed >= 40 && speed <= 255 ) {
    analogWrite(enA, speed);
  }
}

void rpm_fun()
{
  //Update count
  counter++;
}
