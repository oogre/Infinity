///
/// @mainpage	Infinity
///
/// @details	Description of the project
/// @n
/// @n
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		Vincent Evrard
/// @author		OGRE
/// @date		18/04/20 14:13
/// @version	0.1
///
/// @copyright	(c) Vincent Evrard, 2020
/// @copyright	CC = BY SA NC
///
/// @see		ReadMe.txt for references
///

// 14210 : DRAWER X - id : 0
// 14320 : DRAWER Y - id : 1
// 14230 : ERASER X - id : 2
// 14240 : ERASER Y - id : 3

#include "Arduino.h"
#include "EventManager.h"
#include "Axe.h"

#define DEBUG           false
#define MODULE_COUNT    4
#define END_PIN         7
#define DIR_PIN         A1
#define PUL_PIN         A0
#define STEP_PER_REV    3200
#define X_RAIL_IN_STEP  83.75 * STEP_PER_REV // 268000 // 270000 // 280000
#define Y_RAIL_IN_STEP  83.75 * STEP_PER_REV // 180000 // 179200 // 179200 // 56.25

// 82 >> 112

enum PORT  { _14210, _14220, _14230, _14240 };
enum ROBOT { DRAWER, ERASER };
enum AXES  { X, Y };

static const PORT  port = _14240;
static const ROBOT role = port == _14210 || port == _14220 ? DRAWER : ERASER;
static const AXES  _axe = port == _14210 || port == _14230 ? X : Y;
static const uint8_t ID = (role << 1) | _axe;

static const uint32_t      step = _axe == X ? X_RAIL_IN_STEP : Y_RAIL_IN_STEP;
static const uint16_t stepDelay = _axe == X ? 66 : 66; // 27 : 50; // 106
static const float      startAt = role == DRAWER ? 0.25 : 0.125;

Axe axe(step, _axe == X, stepDelay, startAt);
uint32_t lastMessageAt;
EventManager Manager;
char * buffer;

/* DRIVER */
bool testEndSensor(){
    return digitalRead(END_PIN);
}

void SetDirection(uint8_t Direction){
    if(_axe == Y) Direction = !Direction;
    if(role == DRAWER && Manager.myState == Manager.RUN) Direction = !Direction;
    digitalWrite(DIR_PIN, Direction );
}

void DoStep(float Duration){
    digitalWrite(PUL_PIN, HIGH);
    delayMicroseconds(1);
    digitalWrite(PUL_PIN, LOW);
    if (Duration > 16383) {
        delay(Duration * 0.001);
    } else {
        delayMicroseconds(Duration);
    }
}
/* END DRIVER */

/* MANAGER */
bool goHome(){
    //return random(1000.0f)<0.01;
    return axe.goHome();
}

bool goStart(){
    //return random(100.0f)<0.01;
    return axe.goStart();
}
bool goVirtualRun(){
    //return random(100.0f)<0.01;
    return axe.virtualRun();
}
bool goRun(){
    //return random(100.0f)<0.01;
    return axe.run();
}
/* END MANAGER */

void send(){
    buffer[ID] = Manager.myState+64;
    Serial.write(buffer, MODULE_COUNT);
    Serial.write(13);
}

bool all(char * a, uint8_t n){
    char r = a[0];
    for(uint8_t i = 0 ; i < n ; i ++){
        if(r != a[i]){
            return false;
        }
    }
    return true;
}

void setup(){
    delay(1000 + ID * 1000);
    Serial.begin(9600);
    while(!Serial){}
    
    axe.setDoStep(DoStep);
    axe.setSetDirection(SetDirection);
    axe.setTestEndSensor(testEndSensor);

    Manager.onGoRun(goRun);
    Manager.onGoHome(goHome);
    Manager.onGoStart(goStart);
    Manager.onGoVirtualRun(goVirtualRun);
    Manager.begin();
    
    pinMode(DIR_PIN, OUTPUT);
    pinMode(PUL_PIN, OUTPUT);
    pinMode(END_PIN, INPUT);
    randomSeed(analogRead(A0) + ID);
    
    buffer = (char*)malloc(sizeof(char)*MODULE_COUNT);
    for(uint8_t i = 0 ; i < MODULE_COUNT ; i ++){
        buffer[i] = 63;
    }
    send();
    lastMessageAt = millis();
}

void loop(){
    uint32_t time = millis();
    if(Manager.myState <= Manager.HELLO){
        lastMessageAt = time;
    }
    if(time - lastMessageAt > 200){
        Manager.stop();
    }
    
    if(Serial.available() > MODULE_COUNT){
        lastMessageAt = time;
        size_t bufferLen = Serial.readBytesUntil(13, buffer, MODULE_COUNT);
        if(bufferLen != 0) send();
        Manager.run(bufferLen != 0 &&  all(buffer, MODULE_COUNT));
    }else{
       Manager.run();
    }
}
