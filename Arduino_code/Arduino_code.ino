#include <Servo.h>

#define in1 5  // L298N Motor Driver pins
#define in2 6
#define in3 10
#define in4 11

Servo servo1; // Servo for the seed dispenser mechanism
Servo servo2; // Servo for the moisture sensor mechanism

int command;     // Stores app command state
int Speed = 204; // Initial speed (0 - 255)
int Speedsec;
int buttonState = 0;
int lastButtonState = 0;
int Turnradius = 0; // Set the radius of a turn, 0 - 255 (must be ≤ Speed)
int brakeTime = 45;
int brkonoff = 1;  // 1 for electronic braking system, 0 for normal

void setup() {
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    
    servo1.attach(8);  // Attach servo1 to pin 8
    servo2.attach(9);  // Attach servo2 to pin 9

    Serial.begin(9600);  // Set baud rate for Bluetooth communication
}

void loop() {
    if (Serial.available() > 0) {
        command = Serial.read();
        Stop();  // Initialize with motors stopped

        switch (command) {
            case 'F': forward(); break;
            case 'B': back(); break;
            case 'L': left(); break;
            case 'R': right(); break;
            case 'G': forwardleft(); break;
            case 'I': forwardright(); break;
            case 'H': backleft(); break;
            case 'J': backright(); break;

            // Speed control cases
            case '0': Speed = 100; break;
            case '1': Speed = 140; break;
            case '2': Speed = 153; break;
            case '3': Speed = 165; break;
            case '4': Speed = 178; break;
            case '5': Speed = 191; break;
            case '6': Speed = 204; break;
            case '7': Speed = 216; break;
            case '8': Speed = 229; break;
            case '9': Speed = 242; break;
            case 'q': Speed = 255; break;

            // Servo control cases
            case 'O':  // Servo ON command for seed dispenser mechanism
                for (int i = 0; i <= 90; i++) {
                    servo1.write(i);
                    delay(15);
                }
                for (int i = 90; i >= 0; i--) {
                    servo1.write(i);
                    delay(15);
                }
                break;

            case 'o':  // Servo OFF command for seed dispenser mechanism
                servo1.write(0);  // Set servo1 to 0 degrees
                break;

            case 'M':  // Servo ON command for moisture mechanism
                for (int i = 0; i <= 180; i++) {
                    servo2.write(i);
                    delay(15);
                }
                for (int i = 180; i >= 0; i--) {
                    servo2.write(i);
                    delay(15);
                }
                break;
        }

        Speedsec = Turnradius;
        if (brkonoff == 1) {
            brakeOn();
        } else {
            brakeOff();
        }
    }
}

// Motor control functions
void forward() {
    analogWrite(in1, Speed);
    analogWrite(in3, Speed);
}

void back() {
    analogWrite(in2, Speed);
    analogWrite(in4, Speed);
}

void left() {
    analogWrite(in3, Speed);
    analogWrite(in2, Speed);
}

void right() {
    analogWrite(in1, Speed);
    analogWrite(in4, Speed);
}

void forwardleft() {
    analogWrite(in1, Speed);
    analogWrite(in3, Speedsec);
}

void forwardright() {
    analogWrite(in1, Speed);
    analogWrite(in3, Speedsec);
}

void backleft() {
    analogWrite(in2, Speedsec);
    analogWrite(in4, Speed);
}

void backright() {
    analogWrite(in2, Speed);
    analogWrite(in4, Speedsec);
}

void Stop() {
    analogWrite(in1, 0);
    analogWrite(in2, 0);
    analogWrite(in3, 0);
    analogWrite(in4, 0);
}

void brakeOn() {
    Stop();
    delay(brakeTime);
}

void brakeOff() {
    // No braking applied, motors stop as per normal
}
