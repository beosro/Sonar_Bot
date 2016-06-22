#define FULL_SPEED 255
#define LEFT_WHEELS 1
#define RIGHT_WHEELS 2

#define trigPin 7
#define echoPin 6
//motor A connected between A01 and A02
//motor B connected between B01 and B02

int STBY = 10; //standby

//Motor A
int PWMA = 3; //Speed control
int AIN1 = 9; //Direction
int AIN2 = 8; //Direction

//Motor B
int PWMB = 5; //Speed control
int BIN1 = 11; //Direction
int BIN2 = 12; //Direction

void setup() {
  pinMode(STBY, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

int getDistance() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  if (distance >= 200 || distance <= 0) {
    Serial.println("Out of range");
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
    return distance;
  }

  return -1;
}

void loop() {
  int dist = getDistance();
  if (dist > 0 && dist >= 15) {
    move(LEFT_WHEELS, FULL_SPEED / 2, 1); //motor 1, full speed, left
    move(RIGHT_WHEELS, FULL_SPEED / 2, 1); //motor 2, full speed, left
  } else {
    //move back
    move(LEFT_WHEELS, FULL_SPEED / 2, 0); //motor 1, full speed, left
    move(RIGHT_WHEELS, FULL_SPEED / 2, 0); //motor 2, full speed, left
    delay(20 * (15 - dist));
    //turn
    move(LEFT_WHEELS, FULL_SPEED / 2, 1); //motor 1, full speed, left
    move(RIGHT_WHEELS, 0, 1); //motor 2, full speed, left
    delay(1800);    
    //stop
    move(LEFT_WHEELS, 0, 1); //motor 1, full speed, left
    move(RIGHT_WHEELS, 0, 1); //motor 2, full speed, left
  }

  delay(50);
}


void move(int motor, int speed, int direction) {
  //Move specific motor at speed and direction
  //motor: 0 for B 1 for A
  //speed: 0 is off, and 255 is full speed
  //direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if (direction == 1) {
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if (motor == 1) {
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  } else {
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}

void stop() {
  //enable standby
  digitalWrite(STBY, LOW);
}
