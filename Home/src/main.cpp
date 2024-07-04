#include "gamestate.h"
#include "genetic.h"
#include <iostream>
#include <ctime>
#include <fstream>

#define INF (100000000)

int main()
{
    srand(time(0));
	std::ifstream level("example.lvl");
	std::string lvldat;

	std::string lvldata = "";
	while (getline(level, lvldat)) lvldata = lvldata + lvldat + " \n";
	GameState game = GameState::readIn(lvldata);

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
}