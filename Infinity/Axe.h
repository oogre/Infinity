//
//  Axe.h
//  Infinity
//
//  Created by Vincent Evrard on 18/04/20.
//  Copyright © 2020 OGRE. CC = BY SA NC
//

#ifndef Axe_h
#define Axe_h

//#include "iSin.h"

class Axe {
    public :
    bool isX;
    uint32_t k;
    uint32_t kMax;
    uint32_t padding;
    
    float alphaInc;
    float offset = 0;
    
    long position = 0;
    uint16_t startAt;
    uint16_t stepDelay;
    uint16_t stepDuration;
    uint16_t maxStepPerBatch;
    bool sensorTouched = false;
    bool clockWise;
    
    void (*doStep)(float);
    bool (*testEndSensor)(void);
    void (*setDirection)(uint8_t);
    void (*debug)(uint16_t, uint16_t);
    
    public :
    Axe(uint32_t totalStep, bool isX, uint16_t stepDelay, float startAt, bool clockWise)
    :   isX(isX),
        stepDelay(stepDelay),
        stepDuration(stepDelay + 1),
        clockWise(clockWise)
    {
        this->k = 0;
        this->kMax = 4000;
        this->alphaInc = TWO_PI/this->kMax;
        this->maxStepPerBatch = totalStep / this->kMax;
        if(!isX){
            this->alphaInc *= 2;
            this->offset = HALF_PI;
            this->position = -1 * ( totalStep/4 );
        }
        this->padding = fmod(startAt + (clockWise ? 0.5 : 0 ) + 1.0f, 1.0f) * this->kMax;
    };
    
    bool run() {
        float angle = k * alphaInc + offset;
        float speed = (clockWise ? -1 : 1) * sin(angle);
        
        bool direction = speed > 0 ;
        int step = abs((int)round(speed * maxStepPerBatch));
        if (k >= kMax - 1) {
            direction = position < 0;
            step = abs((int)position);
        }
        
        if(setDirection)(*setDirection)(direction);
        for(int i = 0 ; i < step ; i ++){
            if(doStep)(*doStep)(stepDelay);
            position += direction ? 1 : -1;
        }
        
        uint32_t wait = (maxStepPerBatch - step) * stepDuration;
        if(wait > 16383) {
            delay(wait * 0.001);
        }else{
            delayMicroseconds(wait);
        }
        
        k++;
        if(k >= kMax){
            k = 0 ;
        }
        if(k == padding){
            return true;
        }
        return false;
    }
    // USED TO OFFSET THE START POINT
    bool virtualRun(){
        if(padding <= 0){
            padding = k;
            return true;
        }
        
        float angle = k * alphaInc + offset;
        float speed = sin(angle);
        
        bool direction = speed > 0 ;
        int step = abs((int)round(speed * maxStepPerBatch));
        if (k >= kMax - 1) {
            direction = position < 0;
            step = abs((int)position);
        }
        
        for(int i = 0 ; i < step ; i ++){
            position += direction ? -1 : 1;
        }
        k++;
        if(k >= kMax){
            k = 0 ;
        }
        
        padding--;
        if(padding <= 0){
            padding = k;
            return true;
        }
        return false;
    }
    
    bool goStart(){
        bool direction = position < 0 ? HIGH : LOW;
        if (setDirection)(*setDirection)(direction);
        if (doStep)(*doStep)(stepDelay);
        position += direction ? 1 : -1;
        return position == 0;
    }
    
    bool goHome(){
        if(!testEndSensor)return true;
        if(!sensorTouched){
            if((*testEndSensor)()){
                if (setDirection)(*setDirection)(LOW);
                if (doStep)(*doStep)(stepDelay);
            }else{
                sensorTouched = true;
            }
        }else{
            if(!(*testEndSensor)()){
                if (setDirection)(*setDirection)(HIGH);
                if (doStep)(*doStep)(stepDelay*20);
            }else{
                sensorTouched = false;
                return true;
            }
        }
        return false;
    }
    
    void setTestEndSensor(bool (*testEndSensor)(void)) {
        this->testEndSensor = testEndSensor;
    }
    void setDoStep(void (*doStep)(float)) {
        this->doStep = doStep;
    }
    
    void setSetDirection(void (*setDirection)(uint8_t)) {
        this->setDirection = setDirection;
    }
    
    void setDebug(void (*debug)(uint16_t, uint16_t)) {
        this->debug = debug;
    }
};

#endif /* Axe_h */
