
// controller.h

#ifndef _CONTROLLER_h
#define _CONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#endif
#include "RF24.h"
struct rx_values_t {
    float P = 1;
    float I = 0;
    float D = 0;
    uint8_t throttle=0;
    uint8_t yaw=127;
    uint8_t pitch=127;
    uint8_t roll=127;
    uint8_t flip : 1;
    uint8_t highspeed : 1;
    uint8_t auxLED : 1;
    uint8_t safety : 1;
    uint8_t pad : 4;
};
class Controller
{
private:
    RF24* mWireless;
    uint8_t mChannel;
    rf24_pa_dbm_e mPALevel;
    bool mWriting;
    bool isRunning=true;
public:
    uint8_t receive(rx_values_t* rxValues);
    void init();
    uint8_t send(rx_values_t* rxValues);
    void print(rx_values_t* rxValues);
    void setChannel(int channel) {
        mWireless->setChannel(channel);
    }
    bool isFunctioning() {
        return isRunning;
    }
    
    //constructors
    Controller(RF24* radio, uint8_t channel, bool isController);
    Controller(RF24* radio, uint8_t channel, rf24_pa_dbm_e PALevel, bool isController);
};
#endif
