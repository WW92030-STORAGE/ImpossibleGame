#include "gamestate.h"
#include "genetic.h"
#include <iostream>
#include <ctime>
#include <fstream>

#define INF (100000000)

int main()
{
    std::ifstream level("example.lvl");
	std::string lvldat;

	std::string lvldata = "";
	while (getline(level, lvldat)) lvldata = lvldata + lvldat + " \n";
	GameState game = GameState::readIn(lvldata);

	std::cout << "WIDTH = " << game.width << "\n";
	
	std::cout << game.draw() << "\n";
    
    
    
    int N = 16;
    int GEN = 1024;
    
    std::vector<NeuralNetwork> v;
    for (int i = 0; i < N; i++) v.push_back(Genetic::randomAI());
    
    for (int gen = 0; gen < GEN; gen++) {
        std::cout << "\nGEN " << gen << "\n";
        std::vector<NeuralNetwork> res = Genetic::tournament(game, v, 1, true);
		if (res.size() < 2) res.push_back(NeuralNetwork(res[0]));
		v = std::vector<NeuralNetwork>(1, NeuralNetwork(res[0])); // Keep a copy of the best run
		
		if (Genetic::test(game, v[0]) >= INF) break;
        
        for (int round = 0; round < N; round++) {
            std::random_shuffle(res.begin(), res.end());
            for (int i = 0; i < res.size() - 1; i++) {
				v.push_back(Genetic::mutate(res[i]));
                if (v.size() >= N) break;
            }
            if (v.size() >= N) break;
        }
    }
    
    Genetic::test(game, v[0], true);
    for (int i = 0; i < 16; i++) std::cout << Genetic::test(game, v[0]) << " ";
    
    std::cout << "\n" << v[0].toString() << "\n";

    return 0;
}