
/*Note: check to see if motors will move at set speeds (255 is max that a motor can reach but the minimum threshold ma have to be higher/lower)
 
 M1	M2
 M3	M4
 */

#define motor1 5
#define motor2 6
#define motor3 9
#define motor4 10

void pitchForward() {
    //If (cForward > 0 && nothing else on){
    analogWrite(motor1, 30);
    analogWrite(motor2, 30);
    analogWrite(motor3, 75);
    analogWrite(motor4, 75);
    //}
}

void pitchBackward() {
    //if (cBackward>0 && nothing else on){
    analogWrite(motor1, 75);
    analogWrite(motor2, 75);
    analogWrite(motor3, 30);
    analogWrite(motor4, 30);
    //}
}

void rollLeft() {
    //if (cRollLeft>0 && nothing else on){
    analogWrite(motor1, 30);
    analogWrite(motor2, 75);
    analogWrite(motor3, 30);
    analogWrite(motor4, 75);
    //}
}

void rollRight() {
    //if (cRollRight>0 && nothing else on){
    analogWrite(motor1, 75);
    analogWrite(motor2, 30);
    analogWrite(motor3, 75);
    analogWrite(motor4, 30);
    //}
}

void yawLeft() {
    //if (cYawLeft>0 && nothing else on){
    analogWrite(motor1, 30);
    analogWrite(motor2, 75);
    analogWrite(motor3, 75);
    analogWrite(motor4, 30);
    //}
}

void yawRight() {
    //if (cYawRight>0 && nothing else on){
    analogWrite(motor1, 75);
    analogWrite(motor2, 30);
    analogWrite(motor3, 30);
    analogWrite(motor4, 75);
    //}
}

void setup() {
    Serial.begin(9600);
    pinMode(motor1, OUTPUT);
    pinMode(motor2, OUTPUT);
    pinMode(motor3, OUTPUT);
    pinMode(motor4, OUTPUT);
    analogWrite(motor1, 0);
    analogWrite(motor2, 0);
    analogWrite(motor3, 0);
    analogWrite(motor4, 0);
}

void loop() {
    Serial.println("Pitching forward");
    pitchForward();
    delay(300);
    
    Serial.println("Pitching backward");
    pitchBackward();
    delay(300);
    
    Serial.println("rolling left");
    rollLeft();
    delay(300);
    
    Serial.println("rolling right");
    rollRight();
    delay(300);
    
    Serial.println("yawing left");
    yawLeft();
    delay(300);
    
    Serial.println("yawing right");
    yawLeft();
    delay(300);
    
    Serial.println("Done with sequence");
    delay(300);
}


