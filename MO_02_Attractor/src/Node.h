#ifndef node_h
#define node_h

#include "ofMain.h"

class Node {
public:
    // Constructor
    Node();
    
    // Position
    float x, y;
    
    // Velocity
    float xVel, yVel;
    
    // Boundary
    float minX, minY, maxX, maxY;
    
    // damping the velocity (0 = no damping, 1 = full damping)
    float damping;
    
    // update the position
    void setPosition(float _x, float _y);
    
    // update the position
    void setVelocity(float _x, float _y);
    
    // update the Boundary values
    void setBoundary(float _x, float _y, float _w, float _h);
    
    // update the damping value
    void setDamping(float _d);
    
    // update velocity
    void update();
    
};
#endif
