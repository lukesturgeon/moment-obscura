#include "Node.h"

Node::Node(){
    position.set(0, 0);
    xVel = yVel = 0;
    minX = minY = 5;
    maxX = ofGetWidth()-5;
    maxY = ofGetHeight()-5;
    damping = 0.1;
}

void Node::setPosition(float _x, float _y){
    position.set(_x, _y);
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
    position.x += xVel;
    position.y += yVel;
    
    // check boundaries
    if (position.x < minX) {
        position.x = minX - (position.x - minX);
        xVel = -xVel;
    }
    if (position.x > maxX) {
        position.x = maxX - (position.x - maxX);
        xVel = -xVel;
    }
    
    if (position.y < minY) {
        position.y = minY - (position.y - minY);
        yVel = -yVel;
    }
    if (position.y > maxY) {
        position.y = maxY - (position.y - maxY);
        yVel = -yVel;
    }
    
    // apply damping
    xVel *= (1-damping);
    yVel *= (1-damping);
}