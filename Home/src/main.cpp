#include "gamestate.h"
#include "genetic.h"
#include <iostream>
#include <ctime>
#include <fstream>

#define INF (100000000)

int main()
{
    srand(time(0));
	GameState game;

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

	std::cout << "WIDTH = " << game.width << "\n";
	
	std::cout << game.draw() << "\n";
	std::cout << game.toString() << "\n";

	GameState game2 = GameState::readIn(game.toString());
    
    
    std::ifstream fin("example.dat");
	std::string linear;

	std::string nndata = "";
	while (getline(fin, linear)) nndata = nndata + linear + " \n";
	
    
    NeuralNetwork nn = NeuralNetwork::readIn(nndata);
    std::cout << nn.toString() << "\n";
    
    for (int i = 0; i < 32; i++) std::cout << Genetic::test(game2, nn) << "\n";

    return 0;

    return 0;
}