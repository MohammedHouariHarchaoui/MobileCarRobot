// Motor pins (adjust as per your setup)
const int motorPin1 = 9;  // Motor 1 forward
const int motorPin2 = 10; // Motor 1 backward
const int motorPin3 = 11; // Motor 2 forward
const int motorPin4 = 12; // Motor 2 backward

// Ultrasonic Sensor pins
const int triggerPin = 7;
const int echoPin = 6;

long duration;
int distance;

void setup() {
  // Initialize motor control pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  // Initialize ultrasonic sensor pins
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  // Read the distance from the ultrasonic sensor
  distance = readUltrasonicDistance();
  
  // If the distance is less than 15 cm, stop the motors
  if (distance < 15) {
    stopMotors();
    Serial.println("Obstacle detected! Stopping.");
    return;  // Stop further processing if obstacle is detected
  }

  // Check for incoming commands from Raspberry Pi
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');  // Read incoming command

    // Process the received command
    if (command == "MOVE_FORWARD") {
      moveForward();
    } else if (command == "MOVE_BACKWARD") {
      moveBackward();
    } else if (command == "MOVE_LEFT") {
      moveLeft();
    } else if (command == "MOVE_RIGHT") {
      moveRight();
    } else if (command == "STOP") {
      stopMotors();
    } else {
      Serial.println("Unknown command");
    }
  }
}

// Function to measure the distance using the ultrasonic sensor
long readUltrasonicDistance() {
  digitalWrite(triggerPin, LOW);  // Ensure trigger pin is low
  delayMicroseconds(2);
  
  digitalWrite(triggerPin, HIGH);  // Send a pulse
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);   // End the pulse
  
  duration = pulseIn(echoPin, HIGH);  // Measure the duration of the pulse
  long distance = (duration / 2) * 0.0344;  // Convert the time into distance (in cm)
  
  return distance;  // Return the distance measured
}

// Motor control functions
void moveForward() {
  digitalWrite(motorPin1, HIGH);   // Move forward
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);   // Move forward
  digitalWrite(motorPin4, LOW);
  Serial.println("Moving forward.");
}

void moveBackward() {
  digitalWrite(motorPin1, LOW);    // Move backward
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);    // Move backward
  digitalWrite(motorPin4, HIGH);
  Serial.println("Moving backward.");
}

void moveLeft() {
  digitalWrite(motorPin1, LOW);    // Turn left
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);   // Turn left
  digitalWrite(motorPin4, LOW);
  Serial.println("Moving left.");
}

void moveRight() {
  digitalWrite(motorPin1, HIGH);   // Turn right
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);    // Turn right
  digitalWrite(motorPin4, HIGH);
  Serial.println("Moving right.");
}

void stopMotors() {
  digitalWrite(motorPin1, LOW);    // Stop motors
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);    // Stop motors
  digitalWrite(motorPin4, LOW);
  Serial.println("Motors stopped.");
}
