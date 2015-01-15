#include "Node.h"

Node::Node(){
    x = y = 0;
    xVel = yVel = 0;
    minX = minY = 5;
    maxX = ofGetWidth()-5;
    maxY = ofGetHeight()-5;
    damping = 0.1;
}

void Node::setPosition(float _x, float _y){
    x = _x;
    y = _y;
}

void Node::setVelocity(float _x, float _y){
    xVel = _x;
    yVel = _y;
}

void Node::setDamping(float _d){
    damping = _d;
}

void Node::setBoundary(float _minX, float _minY, float _maxX, float _maxY){
    minX = _minX;
    minY = _minY;
    maxX = _maxX;
    maxY = _maxY;
}

void Node::update(){
    // apply velocity
    x += xVel;
    y += yVel;
    
    // check boundaries
    if (x < minX) {
        x = minX - (x - minX);
        xVel = -xVel;
    }
    if (x > maxX) {
        x = maxX - (x - maxX);
        xVel = -xVel;
    }
    
    if (y < minY) {
        y = minY - (y - minY);
        yVel = -yVel;
    }
    if (y > maxY) {
        y = maxY - (y - maxY);
        yVel = -yVel;
    }
    
    // apply damping
    xVel *= (1-damping);
    yVel *= (1-damping);
}