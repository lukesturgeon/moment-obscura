#ifndef attractor_h
#define attractor_h

#include "ofMain.h"
#include "ofMath.h"
#include "Node.h"

class Attractor {
public:
    // Constructor
    Attractor();
    
    // Position
    float x, y;
    
    // radius of the impact
    float radius;
    
    // strength: positive for attraction, negative for repulsion
    float strength;
    
    // parameter that influences the form of the function
    float ramp;
    
    // apply attraction to a Node
    void attract(Node * _node);
};
#endif
