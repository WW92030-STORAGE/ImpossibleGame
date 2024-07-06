#ifndef NEURAL_H
#define NEURAL_H

#include "gamestate.h"
#include <vector>
#include <iostream>
#include <cfloat>
#include <climits>
#include <algorithm>
#include <cmath>

// Implementation of a small evolving neural network system

#define DEFAULT_INPUT 8
#define DEFAULT_LAYERS 1
#define DEFAULT_HIDDEN 4
#define INF (100000000)
#define REFRESH_RATE (1.0 / 60.0)

    
    // This network takes in the following inputs: horizontal distance to nearest obstacle
	// upwards/downwards vertical distance to nearest obstacle that is above the player (2 inputs), player gravity
	// an anticipation of the next 60 ticks if no action is taken and if action is taken constantly and if only action is taken now
	// interaction with yellow orb

class NeuralNetwork {
    public:
    
    // The mechanism of a neural network is actually fairly simple. 
    // There are input nodes, hidden nodes, and output nodes. 
    // Hidden and output nodes simply receive data in the form of a linear combination of the data from their parents.
    // The node then puts this linear combination (input data) into an activation function to "accentuate" the value.
    // Nodes then send the activated data to the child nodes as part of the linear combinations of those child nodes.
    // Each hidden layer and output layer node also takes in a bias coefficient. The bias coefficient is represented as a node in each layer that always outputs one.
    
    // The output layer has only one node.
    // The activation function for each layer is a sigmoid.
    
    // How coefficients are encoded? weights[L][a][b] is the scale the data from node a in layer L 
    // is multiplied when inserted into node b in layer L + 1.
    
    int INPUT_SIZE = DEFAULT_INPUT;
    int HIDDEN_LAYERS = DEFAULT_LAYERS;
    int NODES_PER_HIDDEN = DEFAULT_HIDDEN;
    
    int edges = 0;
    std::vector<std::vector<std::vector<double>>> weights;
    std::vector<std::vector<double>> values;
    
    bool operator<(const NeuralNetwork& other) { return weights < other.weights; }
    
    NeuralNetwork(const NeuralNetwork& other) {
        INPUT_SIZE = other.INPUT_SIZE;
        HIDDEN_LAYERS = other.HIDDEN_LAYERS;
        NODES_PER_HIDDEN = other.NODES_PER_HIDDEN;
        weights = std::vector<std::vector<std::vector<double>>>();
        for (int i = 0; i < other.weights.size(); i++) {
            weights.push_back(std::vector<std::vector<double>>());
            for (int j = 0; j < other.weights[i].size(); j++) {
                weights[i].push_back(std::vector<double>());
                for (int k = 0; k < other.weights[i][j].size(); k++) weights[i][j].push_back(other.weights[i][j][k]);
            }
        }
        
        edges = 0;
        for (auto i : weights) {
            for (auto j : i) {
                for (auto k : j) edges++;
            }
        }
        
        values = std::vector<std::vector<double>>();
        values.push_back(std::vector<double>(INPUT_SIZE + 1, 1));
        for (int i = 0; i < HIDDEN_LAYERS; i++) values.push_back(std::vector<double>(NODES_PER_HIDDEN + 1, 1));
    }
    
    NeuralNetwork() {
        if (HIDDEN_LAYERS == 0) {
            weights.push_back(std::vector<std::vector<double>>(INPUT_SIZE + 1, std::vector<double>(1, 1)));
            return;
        }
        weights = std::vector<std::vector<std::vector<double>>>(1, std::vector<std::vector<double>>(INPUT_SIZE + 1, std::vector<double>(NODES_PER_HIDDEN, 1)));
        for (int i = 1; i < HIDDEN_LAYERS; i++) {
            weights.push_back(std::vector<std::vector<double>>(NODES_PER_HIDDEN + 1, std::vector<double>(NODES_PER_HIDDEN, 1)));
        }
        weights.push_back(std::vector<std::vector<double>>(NODES_PER_HIDDEN + 1, std::vector<double>(1, 1)));
        
        edges = 0;
        for (auto i : weights) {
            for (auto j : i) {
                for (auto k : j) edges++;
            }
        }
        
        values = std::vector<std::vector<double>>();
        values.push_back(std::vector<double>(INPUT_SIZE, 0));
        for (int i = 0; i < HIDDEN_LAYERS; i++) values.push_back(std::vector<double>(NODES_PER_HIDDEN + 1, 0));
    }
    
    NeuralNetwork(int protogen, int primagen, int primogenitor) {
        INPUT_SIZE = protogen;
        HIDDEN_LAYERS = primagen;
        NODES_PER_HIDDEN = primogenitor;
        
        if (HIDDEN_LAYERS == 0) {
            weights.push_back(std::vector<std::vector<double>>(INPUT_SIZE + 1, std::vector<double>(1, 1)));
            return;
        }
        weights = std::vector<std::vector<std::vector<double>>>(1, std::vector<std::vector<double>>(INPUT_SIZE + 1, std::vector<double>(NODES_PER_HIDDEN, 1)));
        for (int i = 1; i < HIDDEN_LAYERS; i++) {
            weights.push_back(std::vector<std::vector<double>>(NODES_PER_HIDDEN + 1, std::vector<double>(NODES_PER_HIDDEN, 1)));
        }
        weights.push_back(std::vector<std::vector<double>>(NODES_PER_HIDDEN + 1, std::vector<double>(1, 1)));
        
        edges = 0;
        for (auto i : weights) {
            for (auto j : i) {
                for (auto k : j) edges++;
            }
        }
        
        values = std::vector<std::vector<double>>();
        values.push_back(std::vector<double>(INPUT_SIZE, 0));
        for (int i = 0; i < HIDDEN_LAYERS; i++) values.push_back(std::vector<double>(NODES_PER_HIDDEN, 0));
    }
    
    double activation(double x) {
        return x;
        double res = 1 + exp(-1 * x);
        return (1.0 / res) - 0.5;
    }
    
    
    double activd(double y) {
        return 1;
        return y * (1 - y);
    }
    
    double eval(std::vector<double> input) {
        if (input.size() < INPUT_SIZE) return DBL_MIN;
        if (HIDDEN_LAYERS == 0) {
            double res = 0;
            for (int i = 0; i < INPUT_SIZE; i++) res += input[i] * weights[0][i][0];
            res += weights[0][INPUT_SIZE][0];
            return activation(res);
        }
        std::vector<double> data(NODES_PER_HIDDEN, 0);
        
        for (int i = 0; i < INPUT_SIZE; i++) values[0][i] = input[i];
        for (int i = 0; i < NODES_PER_HIDDEN; i++) {
            data[i] = weights[0][INPUT_SIZE][i];
            for (int j = 0; j < INPUT_SIZE; j++) data[i] += weights[0][j][i] * input[j];
            data[i] = activation(data[i]);
            values[1][i] = data[i];
        }
        
        // for (auto i : data) std::cout << i << " ";
        // std::cout << "\n";
        
        std::vector<double> newdata(NODES_PER_HIDDEN, 0);
        
        for (int layer = 1; layer < HIDDEN_LAYERS; layer++) {
            for (int i = 0; i < NODES_PER_HIDDEN; i++) { // next node
                newdata[i] = weights[layer][NODES_PER_HIDDEN][i];
                for (int j = 0; j < NODES_PER_HIDDEN; j++) newdata[i] += weights[layer][j][i] * data[j];
                newdata[i] = activation(newdata[i]);
                values[layer + 1][i] = newdata[i];
            }
            
            data = std::vector<double>(newdata);
            // for (auto i : data) std::cout << i << " ";
            // std::cout << "\n";
            newdata = std::vector<double>(NODES_PER_HIDDEN, 0);
        }
        
        double res = 0;
        for (int i = 0; i < NODES_PER_HIDDEN; i++) res += data[i] * weights[HIDDEN_LAYERS][i][0];
        res += weights[HIDDEN_LAYERS][NODES_PER_HIDDEN][0];
        // std::cout << res << std::endl;
        return activation(res);
    }
    
    std::string toString() {
        std::string res = "[" + std::to_string(INPUT_SIZE) + "," + std::to_string(HIDDEN_LAYERS) + "," + std::to_string(NODES_PER_HIDDEN) + "]\n";
        
        for (int i = 0; i < weights.size(); i++) {
            res = res + "\nLAYER " + std::to_string(i) + ":\n";
            for (int j = 0; j < weights[i].size(); j++) {
                for (int k = 0; k < weights[i][j].size(); k++) res = res + "" + std::to_string(weights[i][j][k]) + ",";
                res = res + "\n";
            }
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
    
    // Generates a neural network based on the toString readout of another.
    
    public:
    
    static NeuralNetwork readIn(std::string data) {
        int comma1 = find(data, ',');
        int input = std::stoi(substring(data, find(data, '[') + 1, comma1));
        int comma2 = find(data, ',', comma1 + 1);
        int layers = std::stoi(substring(data, comma1 + 1, comma2));
        int hidden = std::stoi(substring(data, comma2 + 1, find(data, ']')));
    
        NeuralNetwork nn(input, layers, hidden);
    
        int start = find(data, ':') + 1;
        int previouslayer = start;
        for (int in = 0; in <= input; in++) {
            for (int out = 0; out < hidden; out++) {
                int end = find(data, ',', start);
                nn.weights[0][in][out] = std::stod(substring(data, start, end));
                start = end + 1;
            }
        }
    
        for (int layer = 1; layer < layers; layer++) {
            start = find(data, ':', previouslayer) + 1;
            previouslayer = start;
            for (int in = 0; in <= hidden; in++) {
                for (int out = 0; out < hidden; out++) {
                    int end = find(data, ',', start);
                    nn.weights[layer][in][out] = std::stod(substring(data, start, end));
                    start = end + 1;
                }
            }
        }
    
    
        start = find(data, ':', previouslayer) + 1;
        for (int in = 0; in <= hidden; in++) {
            int end = find(data, ',', start);
            nn.weights[layers][in][0] = std::stod(substring(data, start, end));
            start = end + 1;
        }
    
        return nn;
    }
    
    
    
    bool pick(GameState state2) {
        GameState state(state2);
        std::vector<double> nninput(8, 0);
		int index = state.LBX(state.player.pos.x - 2);
        nninput[0] = state.width - state.player.pos.x;

		for (int i = index; i < state.objects.size(); i++) {
			Object obj = state.objects[i];
			if (obj.overlap(state.player) && obj.id == 6) nninput[7] = 1;
			if (obj.pos.y >= state.player.pos.y - 1 && obj.pos.y <= state.player.pos.y + 3) {
				nninput[0] = std::min(nninput[0], obj.pos.x - state.player.pos.x);
				break;
			}
			if (obj.pos.x > state.player.pos.x + 10) break;
		}
        
        double beep = 64;
		int startingindex = state.LBX(state.player.pos.x);
        for (int i = startingindex; i < state.objects.size(); i++) {
            Object obj = state.objects[i];
            if (obj.pos.x > state.player.pos.x + 2) break;
            if (obj.pos.y < state.player.pos.y + 1) continue;
            beep = std::min(beep, std::abs(obj.pos.y - state.player.pos.y));
        }
        nninput[1] = beep;

		beep = 64.0;

		startingindex = state.LBX(state.player.pos.x);
        for (int i = startingindex; i < state.objects.size(); i++) {
            Object obj = state.objects[i];
            if (obj.pos.x > state.player.pos.x + 2) break;
            if (obj.pos.y > state.player.pos.y - 1) continue;
            beep = std::min(beep, std::abs(obj.pos.y - state.player.pos.y));
        }
        nninput[2] = beep;
        
        // Player gravity
        
        nninput[3] = state.player.grav;

        // Anticipation of the next 60 ticks
        
        GameState predict(state2);
        nninput[4] = 60;
        for (int i = 0; i < 60; i++) {
            if (!predict.tick(REFRESH_RATE)) {
                nninput[4] = i;
                break;
            }
        }
        
        GameState predict2(state2);
        nninput[5] = 60;
        predict2.jump();
        for (int i = 0; i < 60; i++) {
            if (!predict2.tick(REFRESH_RATE)) {
                nninput[4] = i;
                break;
            }
        }
        
        GameState predict3(state2);
        nninput[6] = 60;
        for (int i = 0; i < 60; i++) {
            predict3.jump();
            if (!predict3.tick(REFRESH_RATE)) {
                nninput[6] = i;
                break;
            }
        }
        
        // for (auto i : nninput) std::cout << i << " ";
        // std::cout << "\n";
        
        return eval(nninput) > 0;
    }
};

bool operator<(const NeuralNetwork& nn, const NeuralNetwork& other) { return nn.weights < other.weights; }

namespace Genetic {

double randf() {
    return (double)(rand()) / (double)(RAND_MAX);
}

double randrad() {
    return 2 * (randf() - 0.5);
}

NeuralNetwork randomAI(double radius = 64, int protogen = DEFAULT_INPUT, int primagen = DEFAULT_LAYERS, int primogenitor = DEFAULT_HIDDEN) {
    NeuralNetwork nn(protogen, primagen, primogenitor);
    for (int i = 0; i < nn.weights.size(); i++) {
        for (int j = 0; j < nn.weights[i].size(); j++) {
            for (int k = 0; k < nn.weights[i][j].size(); k++) nn.weights[i][j][k] = radius * randrad();
        }
    }
    return nn;
}

NeuralNetwork cross(NeuralNetwork n1, NeuralNetwork n2) {
    NeuralNetwork res(n1);
    for (int i = 0; i < n1.weights.size(); i++) {
        for (int j = 0; j < n1.weights[i].size(); j++) {
            for (int k = 0; k < n1.weights[i][j].size(); k++) if (rand() % 2 == 0) res.weights[i][j][k] = n2.weights[i][j][k];
        }
    }
    return res;
}

NeuralNetwork mutate(NeuralNetwork nn, double radius = 64) {
    int threshold = (int)(nn.edges);
    
    NeuralNetwork res(nn);
    int beep = rand() % threshold;
    int count = 0;
    for (int i = 0; i < nn.weights.size(); i++) {
        for (int j = 0; j < nn.weights[i].size(); j++) {
            for (int k = 0; k < nn.weights[i][j].size(); k++) {
                if (rand() % threshold == 0) res.weights[i][j][k] = radius * randrad();
				if (count == beep) res.weights[i][j][k] = radius * randrad();
				count++;
			}
        }
    }
    return res;
}

int test(GameState game2, NeuralNetwork nn, bool verbose = false) {
    GameState game(game2);
    int res = 0;
    while (true) {
        if (game.player.pos.x >= game.width) {
            if (verbose) std::cout << ">> " << INF << "\n";
            return INF;
        }
        if (nn.pick(game)) game.jump();
        bool success = game.tick(REFRESH_RATE);
        if (verbose) std::cout << success << " " << game.player.grav << " " << game.player.vy << " " << game.player.pos.toString() << "\n";
        if (!success) break;
        res++;
    }
    if (verbose) std::cout << ">> " << res << " " << game.player.pos.x << "\n";
    return res;
}

std::vector<NeuralNetwork> tournament(GameState game, std::vector<NeuralNetwork> nets, int k, bool verbose = false) {
    std::vector<std::pair<int, NeuralNetwork>> ranks;
    for (auto nn : nets) {
        GameState gamex(game);
        int value = test(gamex, nn);
        ranks.push_back({-1 * value, NeuralNetwork(nn)});
        if (verbose) std::cout << (int)std::floor(value * (game.player.vx * REFRESH_RATE)) << " ";
    }
    std::sort(ranks.begin(), ranks.end());
    std::vector<NeuralNetwork> res;
    for (int i = 0; i < k && i < ranks.size(); i++) res.push_back(ranks[i].second);
    return res;
};

}

#endif