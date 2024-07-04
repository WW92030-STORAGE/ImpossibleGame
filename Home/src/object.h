#ifndef OBJECT_H
#define OBJECT_H

#include "point.h"

class Object {
    public:
    Point HBBL;
    Point HBTR;
    Point pos;
    int width;
    int height;
    int id;
    int rotation;
    
    bool safelanding = false;
    bool tangible = false;
    bool unused = true;
    
    bool operator<(const Object& other) {
        if (pos != other.pos) return pos < other.pos;
        if (rotation != other.rotation) return rotation < other.rotation;
        if (HBBL != other.HBBL) return HBBL < other.HBBL;
        if (HBTR != other.HBTR) return HBTR < other.HBTR;
        if (width != other.width) return width < other.width;
        if (height != other.height) return height < other.height;
        if (safelanding != other.safelanding) return safelanding < other.safelanding;
        if (tangible != other.tangible) return tangible < other.tangible;
        if (unused != other.unused) return unused < other.unused;
        return id < other.id;
    }
    
    bool operator!=(Object& other) {
        return *this < other || other < *this;
    }
    
    bool operator==(Object& other) {
        return !(*this != other);
    }
    
    Object() {
        HBBL = {0, 0};
        HBTR = {1, 1};
        pos = {0, 0};
        width = 1; // Visual size not hitbox size although when object is scaled both are changed.
        height = 1;
        safelanding = false; // Can you land on this object?
        tangible = false;
        id = -1;
        unused = true;
        rotation = 0;
    }
    
    Object(const Object& other) {
        HBBL = Point(other.HBBL);
        HBTR = Point(other.HBTR);
        pos = Point(other.pos);
        width = other.width;
        height = other.height;
        safelanding = other.safelanding;
        tangible = other.tangible;
        id = other.id;
        unused = other.unused;
        rotation = other.rotation;
    }
    
    void translate(Point p) {
        pos += p;
        HBBL += p;
        HBTR += p;
    }
    
    void scale(double d) {
		Point original = pos;
        translate(original * -1);
        translate(Point(-0.5 * width, -0.5 * height));
        
        HBBL *= d;
        HBTR *= d;
        translate(pos * -1);
        translate(original);
        
        width *= d;
        height *= d;
    }
    
    void rotate(int x) {
        while (x < 0) x += 4;
        x = x % 4;
        if (x == 0) return;
        rotate(x - 1);
		rotation++;

		Point original = pos;
        
        translate(original * -1);
        translate(Point(-0.5 * width, -0.5 * height));
        
        double temp = width;
        width = height;
        height = temp;
        
        Point BL = {HBBL.y, HBBL.x};
        Point TR = {HBTR.y, HBTR.x};
        HBBL = BL;
        HBTR = TR;
        
        translate(pos * -1);
        translate(original);
        
    }
    
    double hbwidth() { return HBTR.x - HBBL.x; }
    double hbheight() { return HBTR.y - HBBL.y; }
    
    // strict overlap (does not include edge collisions)
    bool overlap(Object other) {
        if (HBBL.x >= other.HBTR.x) return false;
        if (HBTR.x <= other.HBBL.x) return false;
        if (HBBL.y >= other.HBTR.y) return false;
        if (HBTR.y <= other.HBBL.y) return false;
        return true;
    }
    
    // includes edge collisions
    bool intersect(Object other) {
        if (HBBL.x > other.HBTR.x) return false;
        if (HBTR.x < other.HBBL.x) return false;
        if (HBBL.y > other.HBTR.y) return false;
        if (HBTR.y < other.HBBL.y) return false;
        return true;
    }

	bool isSolidBlock() {
		return id == 2;
	}
};

// Hitboxes are estimated based on images of actual gd hitboxes

// Large spike
class Spike : public Object {
    public:
    
    Spike() : Object() {
        id = 1;
        HBBL = {0.4, 0.3};
        HBTR = {0.6, 0.7};
    }
    
    Spike(Point p) : Object() {
        id = 1;
        HBBL = {0.4, 0.3};
        HBTR = {0.6, 0.7};
        translate(p);
    }
};

// Large block
class Block : public Object {
    public:
    
    Block() : Object() {
        id = 2;
        HBBL = {0, 0};
        HBTR = {1, 1};
        safelanding = true;
        tangible = true;
    }
    
    Block(Point p) : Object() {
        id = 2;
        HBBL = {0, 0};
        HBTR = {1, 1};
        safelanding = true;
        translate(p);
        tangible = true;
    }
};

// Yellow pad
class PadYellow : public Object {
    public:
    
    PadYellow() : Object() {
        id = 3;
        HBBL = {1.0 / 15.0, 0};
        HBTR = {14.0 / 15.0, 0.15};
        safelanding = false;
        tangible = false;
    }
    
    PadYellow(Point p) : Object() {
        id = 3;
        HBBL = {1.0 / 15.0, 0};
        HBTR = {14.0 / 15.0, 0.15};
        safelanding = false;
        tangible = false;
        translate(p);
    }
};

class UpPortal : public Object {
    public:
    
    UpPortal() : Object() {
        id = 4;
        HBBL = {1.0 / 12.0, -0.75};
        HBTR = {11.0 / 12.0, 1.75};
        safelanding = false;
        tangible = false;
		height = 3;
    }
    
    UpPortal(Point p) : Object() {
        id = 4;
        HBBL = {1.0 / 12.0, -0.75};
        HBTR = {11.0 / 12.0, 1.75};
        safelanding = false;
        tangible = false;
        translate(p);
		height = 3;
    }
};

class DownPortal : public Object {
    public:
    
    DownPortal() : Object() {
        id = 5;
        HBBL = {1.0 / 12.0, -0.75};
        HBTR = {11.0 / 12.0, 1.75};
        safelanding = false;
        tangible = false;
		height = 3;
    }
    
    DownPortal(Point p) : Object() {
        id = 5;
        HBBL = {1.0 / 12.0, -0.75};
        HBTR = {11.0 / 12.0, 1.75};
        safelanding = false;
        tangible = false;
        translate(p);
		height = 3;
    }
};

// Yellow orb
class OrbYellow : public Object {
    public:
    
    OrbYellow() : Object() {
        id = 6;
		HBBL = {-0.1, -0.1};
        HBTR = {1.1, 1.1};
        safelanding = false;
        tangible = true;
    }
    
    OrbYellow(Point p) : Object() {
        id = 6;
		HBBL = {-0.1, -0.1};
        HBTR = {1.1, 1.1};
        safelanding = false;
        tangible = true;
        translate(p);
    }
};

// Player object (This object has a forward velocity and physics)
// Data is given by this article - https://gdforum.freeforums.net/thread/48749/p1kachu-presents-physics-geometry-dash
// One delta is the amount of time it takes for a normal speed player to cross a block.
class Player : public Object {
    public:
    int grav = 1; // 1 = normal -1 = reverse
    double vx = 360.0 / 43.0; // This is the horizontal speed of the player. It is also how many deltas are in a second.
    double vy = 0;
    double g[2] = {-0.876 * vx * vx, -0.876 * vx * vx};
    double cap[2] = {2.6, 2.6};

    int mode = 0; // 0 cube 1 ship
    
    // Momentum imparted upon interactions (at the current moment most of these values are not used.)
    double JUMP_VY = 1.94 * vx;
    double PAD_YELLOW = 2.77 * vx;
    double ORB_YELLOW = 1.91 * vx;
    
    Player() : Object() {
        mode = 0;
        id = 0;
    }
    
    bool operator<(Player& other) {
        if (pos != other.pos) return pos < other.pos;
        if (vx != other.vx) return vx < other.vx;
        if (vy != other.vy) return vy < other.vy;
        return g < other.g;
    }
};


#endif