/*Note: values received will be all off since other radio from teensy is not plugged in but the screen will still correctly print the gyroscope and accelerometer numbers
 Goal: understand what values correspond with rotation, acceleration direction (which one will print out positive and which one will print out negative) --- look at my comment and figure out value for it*/
________________________________________
//ARDUINO

#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
RF24 radio(9, 10);
struct {
    int16_t a_x;
    int16_t a_y;
    int16_t a_z;
    int16_t g_x;
    int16_t g_y;
    int16_t g_z;
} game;
char pipes[2] = {'s', 'l'};
void startListeningPrep(uint8_t sizes) {//NUMBER CAN BE ANY PIPE FROM 0-5 check this, try pipe 1 first
    radio.setPayloadSize(sizes);//← just sending 8 bytes, so set size = 8 in setup
    uint8_t channel = 1;
    radio.setChannel(channel);//channel 1 all the time!
    radio.setCRCLength(RF24_CRC_16);//changed to 16 since sending more data over
    radio.setDataRate(RF24_1MBPS);
}
#include <printf.h>
#include <Wire.h>
#define MPUAddr 0x68
typedef enum : uint8_t {
    GYRO_PREC_250 = 0,
    GYRO_PREC_500,
    GYRO_PREC_1000,
    GYRO_PREC_2000
} gyro_precision_e;
typedef enum : uint8_t {
    ACCEL_PREC_2 = 0,
    ACCEL_PREC_4,
    ACCEL_PREC_8,
    ACCEL_PREC_16
} accel_precision_e;

void setSleep(bool enable) {
    Wire.beginTransmission (MPUAddr);
    Wire.write (0x6B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPUAddr, 1, true);
    uint8_t power = Wire.read();
    power = ((enable) ? (power | 0b01000000) : (power & 0b10111111));
    Wire.beginTransmission(MPUAddr);
    Wire.write (0x6B);
    Wire.write (power);
    Wire.endTransmission(true);
}

void getAccelData (int16_t *ax, int16_t *ay, int16_t *az) {
    Wire.beginTransmission(MPUAddr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPUAddr, 6, true);
    *ax = Wire.read() << 8 | Wire.read();
    *ay = Wire.read() << 8 | Wire.read();
    *az = Wire.read() << 8 | Wire.read();
}

void setAccelPrec(int8_t prec) {
    int8_t Prec = (int8_t) prec;
    Prec = Prec << 3;
    Wire.beginTransmission(MPUAddr);
    Wire.write(0x1B);
    Wire.write(Prec);
    Wire.endTransmission(true);
}

void getGyroData (int16_t * gx, int16_t *gy, int16_t*gz) {
    Wire.beginTransmission(MPUAddr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPUAddr, 6, true);
    *gx = Wire.read() << 8 | Wire.read();
    *gy = Wire.read() << 8 | Wire.read();
    *gz = Wire.read() << 8 | Wire.read();
}

void setGyroPrec(int8_t scale) {
    int8_t Scale = (int8_t) scale;
    Scale = Scale << 3;
    Wire.beginTransmission(MPUAddr);
    Wire.write(0x1B);
    Wire.write(Scale);
    Wire.endTransmission(true);
}

void setup() {
    Serial.begin(38400);
    //while (!Serial.available());
    
    radio.begin();
    printf_begin();
    radio.setPALevel(RF24_PA_LOW);// change this to something large enough to fit everything
    startListeningPrep(32);
    radio.openReadingPipe (1, pipes[1]); //calling pipe 1 + the 2nd # in pre-set pipes array in setup
    radio.openWritingPipe(pipes[0]);
    radio.startListening();
    radio.printDetails();
    Wire.begin();
    setSleep(false);
    setAccelPrec(ACCEL_PREC_2);
    setGyroPrec(GYRO_PREC_250);
}

void loop() {
    
    int16_t ax;
    int16_t ay;
    int16_t az;
    getAccelData(&ax, &ay, &az);
    Serial.print("this is the Acceleration Data");
    game.a_x = (ax - 1200);
    game.a_y = (ay - 700);
    game.a_z = (az - 700);
    Serial.print(game.a_x);
    Serial.print(game.a_y);
    Serial.println(game.a_z);
    delay(50);
    
    int16_t gx;
    int16_t gy;
    int16_t gz;
    getGyroData(&gx, &gy, &gz);
    Serial.print("this is the Gyro Data");
    game.g_x = (gx - 1200);
    game.g_y = (gy - 700);
    game.g_z = (gz - 700);
    Serial.print(game.g_x);
    Serial.print(game.g_y);
    Serial.println(game.g_z);
    delay(50);
    
    delay(1000);
    radio.stopListening();
    
}


