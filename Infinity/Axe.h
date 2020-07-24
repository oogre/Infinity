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
        uint32_t k;
        uint32_t kMax;
    
        float alphaInc;
        float alphaMax;
        long position = -1;
        uint16_t offset;
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
        :   stepDuration(stepDelay + 1),
            stepDelay(stepDelay),
            clockWise(clockWise),
            position(0),
            k(0)
        {
            this->kMax = 4000;
            this->alphaInc = (isX ? 1 : 2) / (float) this->kMax;
            this->maxStepPerBatch = totalStep / this->kMax;
            this->offset = (int)round(startAt * this->kMax);
            this->position = -1 * (isX ? 0 : (totalStep/4));
        };
    
        bool run() {
            float speed = (clockWise ? -1 : 1) * sin((k+offset) * alphaInc * TWO_PI);
            int step = abs((int)round(speed * maxStepPerBatch));
            bool direction = speed > 0 ;
            if (k >= kMax-1) {
                step = abs((int)position);
                direction = position < 0;
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
                return true;
            }
            return false;
        }
        // USED TO OFFSET THE START POINT
        bool virtualRun(){
            if(this->offset <= 0) {
                offset = k;
                k = 0;
                return true;
            }
            float speed = sin(k * alphaInc * TWO_PI) * (clockWise ? -1 : 1);
            bool direction = speed > 0 ;
            int step = abs((int)round(speed * maxStepPerBatch));
            for(int i = 0 ; i < step ; i ++){
                position += direction ? -1 : 1;
            }
            this->offset--;
            k++;
            if(k >= kMax){
                k = 0 ;
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
