#include "gamestate.h"
#include "genetic.h"
#include <iostream>
#include <ctime>
#include <fstream>

#define INF (100000000)

int main()
{
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