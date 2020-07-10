//
//  EventManager.h
//  Infinity
//
//  Created by Vincent Evrard on 18/04/20.
//  Copyright © 2020 OGRE. CC = BY SA NC
//

#ifndef EventManager_h
#define EventManager_h
class EventManager {
    public :
    enum STATES { STOP, HELLO, HOMING, AT_HOME, VIRTUAL_RUN, VIRTUAL_RUNNED, STARTING, WAIT, RUN };
    const String STATES_NAME [RUN+1] =  { "STOP", "HELLO", "HOMING", "AT_HOME", "VIRTUAL_RUN", "VIRTUAL_RUNNED",  "STARTING", "WAIT", "RUN" };
    
    uint32_t runCounter;
    bool (*goRun)();
    bool (*goVirtualRun)();
    bool (*goHome)();
    bool (*goStart)();
    bool (*statusChange)(String);
    public :
    STATES myState;
    EventManager() :
        myState(STOP)
    {
    }
    
    void begin() {
        setStatus(HELLO);
    }
    
    void onGoHome(bool (*goHome)()) {
        this->goHome = goHome;
    }
    
    void onGoStart(bool (*goStart)()) {
        this->goStart = goStart;
    }
    
    void onGoRun(bool (*goRun)()) {
        this->goRun = goRun;
    }
    
    void onGoVirtualRun(bool (*goVirtualRun)()) {
        this->goVirtualRun = goVirtualRun;
    }
    
    void onStatusChange(bool (*statusChange)(String)) {
        this->statusChange = statusChange;
    }
    
    void next(){
        if(myState == HOMING){
            setStatus(AT_HOME);
        }
        else if(myState == VIRTUAL_RUN){
            setStatus(VIRTUAL_RUNNED);
        }
        else if(myState == STARTING){
            setStatus(WAIT);
        }
        else if(myState == RUN){
            setStatus(WAIT);
        }
        else{
            setStatus((STATES)(myState+1));
        }
        if(myState == RUN){
            runCounter ++;
        }
    }
    
    void run(bool synched = false){
        if (myState == STOP) {
            pinMode(LED_BUILTIN, OUTPUT);
            digitalWrite(LED_BUILTIN, HIGH);
            delay(500);
            digitalWrite(LED_BUILTIN, LOW);
            delay(500);
            return;
        }
        switch (myState) {
            case HELLO :
            case WAIT :
            case AT_HOME :
            case VIRTUAL_RUNNED :
                if (synched)next();
                break;
            case HOMING :
                if ((*goHome)())next();
                break;
            case STARTING :
                if ((*goStart)())next();
                break;
            case RUN :
                if ((*goRun)())next();
                break;
            case VIRTUAL_RUN :
                if ((*goVirtualRun)())next();
                break;
            default :
                break;
        }
    }
    void stop(){
        setStatus(STOP);
    }
    private :
    
    void setStatus(STATES status) {
        if(status != this->myState && statusChange)(*statusChange)(STATES_NAME[status]);
        this->myState = status;
    }
};

#endif /* EventManager_h */
