#include <AFMotor.h>  //Se includ librariile folosite
#include <NewPing.h>
#include <Servo.h> 

// Se definesc pinii de emitere / receptie pentru senzorul Ultrasonic, distana maxima la care poate ajunge senzorul, viteza maxima a motoarelor in cazul de obstacle avoiding

#define TRIG_PIN A0 
#define ECHO_PIN A1 
#define MAX_DISTANCE 200 
#define MAX_SPEED 190 // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20

//Se apeleaza constructorul clasei NewPing, necesara pentru senzorul ultrasonic

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

//Se declara motoarele, cu ajutorul clasei AFMotor

AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

//Se creeaza variabile care ne ajuta in program

Servo myservo;   // Un obiect al clasei Servo inclusa in program
char command;  // Un Char care capteaza input-ul de pe aplicatia de pe telefon
boolean goesForward=false; // Un Boolean pentru functia de obstacle avoiding
int distance = 100;
int speedSet = 0;


// In setup se scriu valoriile initiale 
void setup() {

  Serial.begin(9600);  //Se seteaza baud rate-ul pentru senzorul Bluetooth
  myservo.attach(10);  
  myservo.write(115); 
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

//Functia loop actioneaza ca o functie main in C++
void loop() 
{
if(Serial.available() > 0){ 
    command = Serial.read(); //Citeste input-ul de pe aplicatie si il salveaza in command
    Stop(); //Se initializeaza cu motoarele oprite
   //In functie de input, se apeleaza diverse functii:
    switch(command){
    case 'F':  
      forward();
      break;
    case 'B':  
       back();
      break;
    case 'L':  
      left();
      break;
    case 'R':
      right();
      break;
    case 'X':
      Obstacle();
      break;
    }
  } 
}


//Functia pentru obstacle avoiding

void Obstacle()
{
while (command == 'X')
{
int distanceR = 0;
 int distanceL =  0;
 delay(40);
 
 if(distance<=15)
 {
  moveStop();
  delay(100);
  moveBackward();
  delay(300);
  moveStop();
  delay(200);
  distanceR = lookRight();
  delay(200);
  distanceL = lookLeft();
  delay(200);

  if(distanceR>=distanceL)
  {
    turnRight();
    moveStop();
  }else
  {
    turnLeft();
    moveStop();
  }
 }else
 {
  moveForward();
 }
 distance = readPing();
 if(distance <=3) break;
}
}

//Functia pentru verificarea obstacolelor din dreapta
int lookRight()
{
    myservo.write(50); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(115); 
    return distance;
}

//Functia pentru verificarea obstacolelor din dreapta
int lookLeft()
{
    myservo.write(170); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(115); 
    return distance;
    delay(100);
}

//Functia prin care senzorul calcueaza distanta
int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}

//Functiile de miscare pentru obstacle avoiding

void moveStop() {
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  } 
  
void moveForward() {

 if(!goesForward)
  {
    goesForward=true;
    motor1.run(FORWARD);      
    motor2.run(FORWARD);
    motor3.run(FORWARD); 
    motor4.run(FORWARD);     
   for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
   }
  }
}

void moveBackward() {
    goesForward=false;
    motor1.run(BACKWARD);      
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);  
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
  }
}  

void turnRight() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);     
  delay(500);
  motor1.run(FORWARD);      
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);      
} 
 
void turnLeft() {
  motor1.run(BACKWARD);     
  motor2.run(BACKWARD);  
  motor3.run(FORWARD);
  motor4.run(FORWARD);   
  delay(500);
  motor1.run(FORWARD);     
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}  

//Functiile de miscare pentru controlul Bluetooth

void forward()
{
  motor1.setSpeed(255);
  motor1.run(FORWARD);
  motor2.setSpeed(255);
  motor2.run(FORWARD);
  motor3.setSpeed(255);
  motor3.run(FORWARD); 
  motor4.setSpeed(255);
  motor4.run(FORWARD); 
}

void back()
{
  motor1.setSpeed(255); 
  motor1.run(BACKWARD); 
  motor2.setSpeed(255); 
  motor2.run(BACKWARD); 
  motor3.setSpeed(255); 
  motor3.run(BACKWARD);
  motor4.setSpeed(255); 
  motor4.run(BACKWARD);
}

void left()
{
  motor1.setSpeed(255); 
  motor1.run(BACKWARD);
  motor2.setSpeed(255); 
  motor2.run(BACKWARD); 
  motor3.setSpeed(255);
  motor3.run(FORWARD); 
  motor4.setSpeed(255);
  motor4.run(FORWARD); 
}

void right()
{
  motor1.setSpeed(255);
  motor1.run(FORWARD); 
  motor2.setSpeed(255);
  motor2.run(FORWARD); 
  motor3.setSpeed(255); 
  motor3.run(BACKWARD); 
  motor4.setSpeed(255);
  motor4.run(BACKWARD);
} 

void Stop()
{
  motor1.setSpeed(0);
  motor1.run(RELEASE);
  motor2.setSpeed(0);
  motor2.run(RELEASE);
  motor3.setSpeed(0); 
  motor3.run(RELEASE); 
  motor4.setSpeed(0);
  motor4.run(RELEASE);
}
