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
    float alpha;
    float alphaInc;
    float alphaMax;
    float offset = 0;
    long position = -1;
    uint16_t startAt;
    uint16_t stepDelay;
    uint16_t stepDuration;
    uint16_t maxStepPerBatch;
    bool sensorTouched = false;
    
    void (*doStep)(float);
    bool (*testEndSensor)(void);
    void (*setDirection)(uint8_t);
    void (*debug)(uint16_t, uint16_t);
    
    public :
    Axe(uint32_t totalStep, bool isX, uint16_t stepDelay, float startAt)
    :   alpha(0.f),
        isX(isX),
        stepDelay(stepDelay),
        stepDuration(stepDelay + 1)
    {
        this->k = 0;
        this->kMax = 4000 * PI;
        this->alphaMax = TWO_PI;
        float resolution = 1.0f/4000;
        this->maxStepPerBatch = totalStep * resolution;
        this->alphaInc = 2.f * resolution;
        
        if(!isX){
            this->alphaInc *= 2;
            this->alphaMax *= 2;
            this->offset = HALF_PI;
            this->position = -1 * ( totalStep/4 );
        }
        this->startAt = (int)(startAt * this->alphaMax / this->alphaInc);
    };
    
    bool run() {
        float speed = sin(k * alphaInc + offset);
        bool direction = speed > 0 ;
        int step = abs((int)round(speed * maxStepPerBatch));
        if(setDirection)(*setDirection)(direction);
        
        if (k > kMax - 1) {
            step = abs((int)position);
        }
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
        //alpha += alphaInc;
        k++;
        if(k > kMax){
            //alpha = 0;
            k = 0 ;
            return true;
        }
        return false;
    }
    // USED TO OFFSET THE START POINT
    bool virtualRun(){
        float speed = sin(alpha+offset);
        /*
         position -= (int)round(speed * maxStepPerBatch);
        */
        /**/
        bool direction = speed > 0 ;
        int step = abs((int)round(speed * maxStepPerBatch));
        for(int i = 0 ; i < step ; i ++){
            position += direction ? -1 : 1;
        }
        /**/
        alpha += alphaInc;
        if(alpha >= alphaMax){
            alpha = 0 ;
        }
        if(startAt-- <= 0) {
            offset += alpha;
            alpha = 0;
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
