//
//  SerialNetwork.h
//  Infinity
//
//  Created by Vincent Evrard on 18/04/20.
//  Copyright © 2020 OGRE. CC = BY SA NC
//



#ifndef SerialNetwork_h
#define SerialNetwork_h
class SerialNetwork {
    const uint8_t nodesLen;
    const SoftwareSerial * nodes;
    uint8_t flipFlop = 0;
    public :
    SerialNetwork(const SoftwareSerial * nodes, const uint8_t nodesLen)
    : nodes(nodes),
    nodesLen(nodesLen)
    {}

    void begin(uint16_t baudRate) {
        for (int i = 0 ; i < nodesLen ; i++) {
            (nodes+i)->begin(baudRate);
        }
    }

    uint8_t run() {
        uint8_t inByte = 0xFF;
        inByte = this->read(nodes+flipFlop);
        flipFlop ++;
        if(flipFlop>=nodesLen)flipFlop=0;
        return inByte;
    }

    void write(uint8_t outByte) {
        for (int i = 0 ; i < nodesLen ; i++) {
            (nodes+i)->write(outByte);
        }
    }

    private :
    uint8_t read(SoftwareSerial * node) {
        node->listen();
        if (node->available() > 0) {
            uint8_t inByte = node->read();
            for (int i = 0 ; i < nodesLen ; i++) {
                if ((nodes+i) != node) {
                    (nodes+i)->write(inByte);
                }
            }
            return inByte;
        }
        return 0xFF;
    }
};

#endif /* SerialNetwork_h */

