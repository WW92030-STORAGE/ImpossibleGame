#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>

#include "gamestate.h"
#include "genetic.h"
#include <iostream>
#include <ctime>
#include<windows.h>           // for windows
#include <fstream>
#include <cmath>

#define PI (std::atan(1) * 4)

GameState setup() {
    GameState game;
    srand(time(0));

    game.add(Spike({3, 0}));
    
    game.add(Spike({10, 0}));
    game.add(Spike({11, 0}));
    game.add(Spike({12, 0}));
    game.add(Spike({13, 0}));
    game.add(PadYellow({9, 0}));
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j <= i; j++) game.add(Block({20 + 4 * i, j}));
    }

	game.add(Block({36, 5}));
	game.add(Block({36, 4}));
	game.add(Block({36, 6}));
	
	game.add(Spike({50, 0}));
	game.add(Spike({51, 0}));
	game.add(Spike({52, 0}));
	
	
	game.add(PadYellow({60, 0}));
	for (int i = 55; i < 65; i++) game.add(Block({i, 4}));
	
	for (int i = 70; i < 75; i++) game.add(Spike({i, 0}));
	game.add(OrbYellow({72, 2}));
	
	game.add(Block({80, 0}));
	game.add(Block({80, 1}));
	
	for (int i = 81; i < 100; i++) {
	    game.add(Spike({i, 0}));
	    game.add(Block({i, 20}));
	}
	game.add(UpPortal({82, 4}));
	game.add(OrbYellow({82, 4}));
	for (int i = 4; i < 10; i++) game.add(Spike({84, i}));

	game.add(SawbladeLarge({96, 20}));

	return game;
}

char* nndata1 = 
"[8,1,5]"
"LAYER 0:"
"-48.839381,-8.342051,-11.365581,-37.444502,-8.552995,"
"5.119297,-14.326609,29.651296,-55.288797,-58.562334,"
"-56.331797,5.924009,19.307230,16.967315,-58.593585,"
"-15.518052,62.523392,45.210364,-37.573412,27.866085,"
"29.471603,53.827815,43.526719,-47.054170,-41.569628,"
"40.682882,-59.308451,22.358104,19.037690,-39.721915,"
"35.124119,24.830836,3.166112,55.214576,1.607471,"
"-52.616840,-5.349773,-29.510666,55.855220,-34.956145,"
"-38.206244,-54.003601,39.823481,62.503861,1.517624,"
"LAYER 1:"
"28.815332,34.569414,21.721366,24.174566,-53.316080,"
"-35.299905,11.271828,16.627461,62.820276,-26.928556,"
"55.749748,39.596912,55.882565,-30.756798,41.432905,"
"-52.784814,-30.733360,45.577563,-36.803858,32.877956,"
"26.209784,-42.378246,19.502548,29.299722,-45.370525,"
"-38.835170,53.905942,-29.151280,21.951842,15.682119,"
"LAYER 2:"
"-63.781243,"
"-37.854671,"
"59.581896,"
"6.322459,"
"34.913175,"
"56.863063,"

;

void rect(int x1, int y1, int x2, int y2) {
	if (x1 > x2) std::swap(x1, x2);
	if (y1 > y2) std::swap(y1, y2);

	line(x1, y1, x1, y2);
	line(x1, y1, x2, y1);
	line(x1, y2, x2, y2);
	line(x2, y1, x2, y2);
}

char* lvldat = 
"88:"
"1,[3.000000 0.000000],0:"
"3,[9.000000 0.000000],0:"
"1,[10.000000 0.000000],0:"
"1,[11.000000 0.000000],0:"
"1,[12.000000 0.000000],0:"
"1,[13.000000 0.000000],0:"
"2,[20.000000 0.000000],0:"
"2,[24.000000 0.000000],0:"
"2,[24.000000 1.000000],0:"
"2,[28.000000 0.000000],0:"
"2,[28.000000 1.000000],0:"
"2,[28.000000 2.000000],0:"
"2,[32.000000 0.000000],0:"
"2,[32.000000 1.000000],0:"
"2,[32.000000 2.000000],0:"
"2,[32.000000 3.000000],0:"
"2,[36.000000 4.000000],0:"
"2,[36.000000 5.000000],0:"
"2,[36.000000 6.000000],0:"
"1,[50.000000 0.000000],0:"
"1,[51.000000 0.000000],0:"
"1,[52.000000 0.000000],0:"
"2,[55.000000 4.000000],0:"
"2,[56.000000 4.000000],0:"
"2,[57.000000 4.000000],0:"
"2,[58.000000 4.000000],0:"
"2,[59.000000 4.000000],0:"
"3,[60.000000 0.000000],0:"
"2,[60.000000 4.000000],0:"
"2,[61.000000 4.000000],0:"
"2,[62.000000 4.000000],0:"
"2,[63.000000 4.000000],0:"
"2,[64.000000 4.000000],0:"
"1,[70.000000 0.000000],0:"
"1,[71.000000 0.000000],0:"
"1,[72.000000 0.000000],0:"
"6,[72.000000 2.000000],0:"
"1,[73.000000 0.000000],0:"
"1,[74.000000 0.000000],0:"
"2,[80.000000 0.000000],0:"
"2,[80.000000 1.000000],0:"
"1,[81.000000 0.000000],0:"
"2,[81.000000 20.000000],0:"
"1,[82.000000 0.000000],0:"
"6,[82.000000 4.000000],0:"
"4,[82.000000 4.000000],0:"
"2,[82.000000 20.000000],0:"
"1,[83.000000 0.000000],0:"
"2,[83.000000 20.000000],0:"
"1,[84.000000 0.000000],0:"
"1,[84.000000 4.000000],0:"
"1,[84.000000 5.000000],0:"
"1,[84.000000 6.000000],0:"
"1,[84.000000 7.000000],0:"
"1,[84.000000 8.000000],0:"
"1,[84.000000 9.000000],0:"
"2,[84.000000 20.000000],0:"
"1,[85.000000 0.000000],0:"
"2,[85.000000 20.000000],0:"
"1,[86.000000 0.000000],0:"
"2,[86.000000 20.000000],0:"
"1,[87.000000 0.000000],0:"
"2,[87.000000 20.000000],0:"
"1,[88.000000 0.000000],0:"
"2,[88.000000 20.000000],0:"
"1,[89.000000 0.000000],0:"
"2,[89.000000 20.000000],0:"
"1,[90.000000 0.000000],0:"
"2,[90.000000 20.000000],0:"
"1,[91.000000 0.000000],0:"
"2,[91.000000 20.000000],0:"
"1,[92.000000 0.000000],0:"
"2,[92.000000 20.000000],0:"
"1,[93.000000 0.000000],0:"
"2,[93.000000 20.000000],0:"
"1,[94.000000 0.000000],0:"
"2,[94.000000 20.000000],0:"
"1,[95.000000 0.000000],0:"
"2,[95.000000 20.000000],0:"
"1,[96.000000 0.000000],0:"
"2,[96.000000 20.000000],0:"
"1,[97.000000 0.000000],0:"
"2,[97.000000 20.000000],0:"
"1,[98.000000 0.000000],0:"
"2,[98.000000 20.000000],0:"
"1,[99.000000 0.000000],0:"
"2,[99.000000 20.000000],0:"
"7,[96.000000 20.000000],0:"
;

int main()
{
	std::string lvldata = lvldat;
	GameState game = GameState::readIn(lvldata);

    int gm, x, y, gd = DETECT, i;
    // int midx, midy;
    int stangle = 45, endangle = 50;
    int radius = 50;
    
    char data[] = "C:\\MinGW\\lib\\libbgi.a"; //static file

    initgraph(&gd, &gm, data);
    x = getmaxx(); // to get the co-ordinates i.e. x & y
    y = getmaxy();
    cleardevice();

	std::string nndata = nndata1;
    
    NeuralNetwork resnn = NeuralNetwork::readIn(nndata);
    std::cout << resnn.toString() << "\n";

	std::string waitforit;
	// std::cin >> waitforit;

	GameState game2(game);
	for (int i = 0; ; i++) {
		if (resnn.pick(game2)) game2.jump();

		if (game2.player.pos.x > game2.width) break;
		if (game2.tick(1.0 / 60.0));
		else break;

		cleardevice();

		int resolution = 30;

		int offx = 4 * resolution;
		int offy = 4 * resolution;

		line(-100, offy - game2.player.pos.y * resolution, 1000, offy - game2.player.pos.y * resolution);

		for (auto obj : game2.objects) {
			int relx = (int)std::floor((obj.pos.x - game2.player.pos.x) * resolution) + offx;
			int rely = (int)std::floor((obj.pos.y - game2.player.pos.y) * resolution) + offy;
			if (obj.id == 2) rect(relx, rely, relx + resolution, rely + resolution);

			if (obj.id == 1) {
				if (obj.rotation == 0) {
					line(relx, rely, relx + resolution, rely);
					line(relx, rely, relx + resolution / 2, rely + resolution);
					line(relx + resolution, rely, relx + resolution / 2, rely + resolution);
				}
				if (obj.rotation == 2) {
					line(relx, rely + resolution, relx + resolution, rely + resolution);
					line(relx, rely + resolution, relx + resolution / 2, rely);
					line(relx + resolution, rely + resolution, relx + resolution / 2, rely);
				}
				if (obj.rotation == 1) {
					line(relx, rely, relx, rely + resolution);
					line(relx, rely, relx + resolution, rely + resolution / 2);
					line(relx, rely + resolution, relx + resolution, rely + resolution / 2);
				}
				if (obj.rotation == 3) {
					line(relx + resolution, rely, relx, rely + resolution / 2);
					line(relx + resolution, rely + resolution, relx, rely + resolution / 2);
					line(relx + resolution, rely, relx + resolution, rely + resolution);
				}
			}

			if (obj.id == 3) {
				if (obj.rotation == 0) rect(relx, rely, relx + resolution, rely + resolution / 4);
				else if (obj.rotation == 2) rect(relx, rely + resolution * 0.75, relx + resolution, rely + resolution);
				else if (obj.rotation == 1) rect(relx, rely, relx + resolution / 4, rely + resolution);
				else if (obj.rotation == 3) rect(relx + resolution * 0.75, rely, relx + resolution, rely);
			}
			if (obj.id == 4) {
				if (obj.rotation % 2 == 0) {
					rect(relx, rely - resolution, relx + resolution, rely + 2 * resolution);
					line(relx, rely + resolution * 2.25, relx + resolution, rely + resolution * 2.25);
				}
				else {
					rect(relx - resolution, rely, relx + 2 * resolution, rely);
				}
			}
			if (obj.id == 5) {
				if (obj.rotation % 2 == 0) {
					rect(relx, rely - resolution, relx + resolution, rely + 2 * resolution);
					line(relx, rely - resolution / 4, relx + resolution, rely - resolution / 4);
				}
				else {
					rect(relx - resolution, rely, relx + 2 * resolution, rely);
				}
			}
			if (obj.id == 6) circle(relx + resolution / 2, rely + resolution / 2, resolution / 3);
			if (obj.id == 7) {
				circle(relx + resolution / 2, rely + resolution / 2, resolution * 13.0 / 12.0);
				double theta = 5 * i * PI / 180;
				for (int i = 0; i < 4; i++) {
					line(relx + (0.5 - std::cos(theta)) * resolution, rely + (0.5 - std::sin(theta)) * resolution, relx + (0.5 + std::cos(theta)) * resolution, rely + (0.5 + std::sin(theta)) * resolution);
					theta += PI / 4;
				}
			}
		}

		rect(offx, offy, offx + resolution, offy + resolution);
		rect(offx + resolution / 4, offy + resolution / 4, offx + resolution * 3 / 4, offy + resolution * 3 / 4);

		int bling = offx + (game2.width - game.player.pos.x) * resolution;
		line(bling, -100, bling, 1000);

		std::cout << i << "\n";

		delay(10);
	}
    
	std::string xxxxx;
	std::cin >> xxxxx;
    getch();
    closegraph();

    return 0;
}