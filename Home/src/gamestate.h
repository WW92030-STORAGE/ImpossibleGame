#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "object.h"
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <string>

class GameState {
    public:
    std::vector<Object> objects;
    double width = 0;
	double height = 0;
    
    Player player;
    
    GameState(const GameState& other) {
        width = other.width;
		height = other.height;
        player = Player(other.player);
        for (auto i : other.objects) objects.push_back(Object(i));
    }
    
    GameState(std::vector<Object> objs) {
        for (auto i : objs) {
            objects.push_back(Object(i));
            width = std::max(width, i.pos.x + 10);
			height = std::max(height, i.pos.y + 10);
        }
        
        std::sort(objects.begin(), objects.end());
        
        player = Player();
    }
    
    GameState() {
        player = Player();
    }
    
    void add(Object o, bool sort = true) {
        objects.push_back(o);
        width = std::max(width, o.pos.x + 10);
		height = std::max(height, o.pos.y + 10);
        
        if (sort)std::sort(objects.begin(), objects.end());
    }

    std::vector<Object> getAll(int id) {
        std::vector<Object> objs;
        for (auto i : objects) {
            if (i.id == id) objs.push_back(Object(i));
        }
        std::sort(objs.begin(), objs.end());
        return objs;
    }
    
    int LBX(double xpos) {
        int low = 0;
        int high = objects.size() - 1;
        while (low + 10 < high) {
            int mid = low + (high - low) / 2;
            if (objects[mid].pos.x >= xpos) high = mid;
            else low = mid;
        }
        
        for (int i = std::max(0, low - 10); i < objects.size(); i++) {
            if (objects[i].pos.x >= xpos) return i;
        }
        return objects.size();
    }
    
    std::vector<Object> getRelevantObjects() {
        int start = std::max(0, LBX(player.pos.x - 4));
        int end = std::min((int)(objects.size()) - 1, LBX(player.pos.x + 4));
        std::vector<Object> res;
        for (int i = start; i <= end; i++) res.push_back(objects[i]);
        std::sort(res.begin(), res.end());
        return res;
    }
    
    void jump() {
        std::vector<Object> objs = getRelevantObjects();
		bool isjumping = false;
		bool isorbyellow = false;
        for (auto i : objs) {
            if (i.intersect(player) && i.tangible) {
                if (i.isSolidBlock()) isjumping = true;
				else if (i.id == 6) isorbyellow = true;
				break;
            }
        }
		if (isorbyellow) {
			player.vy = player.ORB_YELLOW * player.grav;
            return;
		}
		if (isjumping) {
			player.vy = player.JUMP_VY * player.grav;
            return;
		}
		
        if (player.pos.y <= 0 && player.vy * player.grav <= 0) {
            player.vy = player.JUMP_VY * player.grav;
            return;
        }
    }
    
    // Tangible objects denote objects that the player can voluntarily change movement from. These include blocks and orbs.
    double MARGIN = 0.4;
    bool tick(double seconds) {
        double prevvy = player.vy;
        player.vy = player.vy + seconds * player.g[player.mode] * player.grav;
        if (player.vy < -1 * player.cap[player.mode] * player.vx) player.vy = -1 * player.cap[player.mode] * player.vx;
        if (player.vy > player.cap[player.mode] * player.vx) player.vy = player.cap[player.mode] * player.vx;
        player.translate(Point(seconds * player.vx, seconds * 0.5 * (prevvy + player.vy)));
    
        if (player.pos.y < 0 && player.grav == 1) {
			player.translate({0, 0 - player.pos.y});
			player.vy = 0;
		}
        if (player.pos.y < 0 && player.grav == -1) return false;
        
        int start = 0;
        int end = objects.size() - 1;
        
        std::vector<Object> objs = getRelevantObjects();
        
        for (auto obj : objs) {
            if (!obj.intersect(player)) continue;
            if (!obj.safelanding) return false;
            if (obj.id == 1 || obj.id == 2 || obj.id == 7) { // Large block, Spike, Sawblade
				// std::cout << "> " << obj.id << " " << obj.pos.toString() << "\n";
            
                double thresh1 = obj.HBBL.y + MARGIN * obj.hbheight();
                double thresh2 = obj.HBTR.y - MARGIN * obj.hbheight();
                if (player.HBBL.y < thresh2 && player.HBTR.y > thresh1) return false;
                else if (player.HBBL.y >= thresh2 && player.grav == 1) {
                    player.translate({0, obj.HBTR.y - player.HBBL.y});
                    player.vy = 0;
                }
                else if (player.HBTR.y <= thresh1 && player.grav == -1) {
                    player.translate({0, obj.HBBL.y - player.HBTR.y});
                    player.vy = 0;
                }
                else return false;
            }
            else if (obj.id == 3) {
                if (!obj.unused) continue;
                player.vy = player.PAD_YELLOW * player.grav;
                obj.unused = false;
            }
            else if (obj.id == 4) {
                if (!obj.unused) continue;
                player.grav = -1;
                obj.unused = false;
            }
            else if (obj.id == 5) {
                if (!obj.unused) continue;
                player.grav = 1;
                obj.unused = false;
            }
        }
        
        return true;
        
    }

	// Draws a diagram of the level. Positions are aligned to the nearest integer.
	// IDs are checked one at a time in case of layering in which the most "prominent" object is drawn last.
	std::string draw() {
		std::string res = "";
		for (int y = height - 1; y >= 0; y--) {
			for (int x = 0; x < width; x++) {
				char thechar = ' ';
				for (auto i : getAll(4)) {
					if (x >= i.HBBL.x && x < i.HBTR.x && y >= std::floor(i.HBBL.y) && y < i.HBTR.y) thechar = '/';
				}
				for (auto i : getAll(5)) {
					if (x >= i.HBBL.x && x < i.HBTR.x && y >= std::floor(i.HBBL.y) && y < i.HBTR.y) thechar = '\\';
				}
				for (auto i : getAll(6)) {
					Point thepos = Point(i.pos);
					thepos.align();
					if (thepos == Point(x, y)) thechar = 'O';
				}
				for (auto i : getAll(3)) {
					Point thepos = Point(i.pos);
					thepos.align();
					if (thepos == Point(x, y)) {
						if (i.rotation == 0) thechar = '_';
						if (i.rotation == 1) thechar = '[';
						if (i.rotation == 2) thechar = '-';
						if (i.rotation == 3) thechar = ']';
					}
				}
                for (auto i : getAll(7)) {
                    Point thepos = Point(i.pos);
                    double dist = std::max(std::abs(thepos.x - x), std::abs(thepos.y - y));
					if (dist < 1.5) thechar = '+';
                }
				for (auto i : getAll(2)) {
					Point thepos = Point(i.pos);
					thepos.align();
					if (thepos == Point(x, y)) thechar = '#';
				}
				for (auto i : getAll(1)) {
					Point thepos = Point(i.pos);
					thepos.align();
					if (thepos == Point(x, y)) {
						if (i.rotation == 0) thechar = '^';
						if (i.rotation == 1) thechar = '>';
						if (i.rotation == 2) thechar = 'v';
						if (i.rotation == 3) thechar = '<';
					}
				}


				res = res + thechar;
			}
			res = res + "\n";
		}
		
		for (int x = 0; x < width; x++) {
		    if (x % 10 != 0) res = res + ((x & 1) ? '-' : '.');
		    else res = res + "|";
		}
		return res + "\n";
	}

    std::string toString() {
        std::string res = std::to_string(objects.size()) + ":\n";
        for (auto i : objects) {
            res = res + "" + std::to_string(i.id) + "," + i.pos.toString() + "," + std::to_string(i.rotation) + ":\n";
        }
        return res;
    }

    private:

    static int find(std::string value, char c, int start = 0) {
        for (int i = start;i < value.length(); i++) {
            if (value[i] == c) return i;
        }
        return value.length();
    }

    static std::string substring(std::string data, int a, int b) { // [a, b)
        return data.substr(a, b - a);
    }

    public:

    static Object create(int id) {
        if (id == 0) return Player();
        if (id == 1) return Spike();
        if (id == 2) return Block();
        if (id == 3) return PadYellow();
        if (id == 4) return UpPortal();
        if (id == 5) return DownPortal();
        if (id == 6) return OrbYellow();
        if (id == 7) return SawbladeLarge();
        return Object();
    }

    static GameState readIn(std::string data) {
        GameState res;
        int i = find(data, ':', 0);
        int num = stoi(substring(data, 0, i));
        i++;
        for (int k = 0; k < num; k++) {
            int comma1 = find(data, ',', i);
            int openbracket = find(data, '[', comma1 + 1);
            int space = find(data, ' ', openbracket + 1);
            int closebracket = find(data, ']', space + 1);
            int comma2 = find(data, ',', closebracket + 1);
            int colon = find(data, ':', comma2 + 1);

            int id = stoi(substring(data, i, comma1));
            double px = stod(substring(data, openbracket + 1, space));
            double py = stod(substring(data, space + 1, closebracket));
            int rot = stoi(substring(data, comma2 + 1, colon));

            Object x = create(id);
            x.translate({px, py});
            x.rotate(rot);
            res.add(x, false);

            i = colon + 1;
            if (i >= data.length()) break;
        }

        std::sort(res.objects.begin(), res.objects.end());
        return res;
    }
};

#endif