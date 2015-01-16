#include "Attractor.h"

Attractor::Attractor() {
    x = 0;
    y = 0;
    radius = 200;
    strength = 100;
    ramp = 0.5;
}

void Attractor::attract(ofPtr<Node> _node){
    // calculate distance
    float dx = x - _node->position.x;
    float dy = y - _node->position.y;
    float d = (float)sqrt(dx*dx + dy*dy);
    
    if (d > 0 && d < radius) {
        // calculate force
        float s = d/radius;
        float f = (1 / pow(s, (float)0.5*ramp) - 1);
        f = strength * f/radius;
        
        // apply force to node velocity
        _node->xVel += dx * f;
        _node->yVel += dy * f;
    }
}