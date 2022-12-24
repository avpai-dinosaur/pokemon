// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include<vector>
#include<iostream>
#include<limits>
#include<math.h>
#include<getopt.h>

using namespace std;

class PokeGraph
{
public:
	struct PrimData {
		double d; // lowest distance to MST
		int p; // index of vertex's parent in MST
		bool k; // whether or not vertex is already in MST

		PrimData() : d{ numeric_limits<double>::infinity() }, p{ -1 }, k{ false } {}
	};

	enum class pokeType { Land, Water, Coast };

	struct Pokemon {
		int x;
		int y;
		int node;
		pokeType type;
	};

	PokeGraph() : numPokemon(0), promisingUpperBound(0), mstMode(false), fastTspMode(false), optTspMode(false) {}

	void get_options(int argc, char** argv) {
		int option_index = 0, option = 0;

		// Don't display getopt error messages about options
		opterr = false;

		// use getopt to find command line options
		struct option longOpts[] = {
			{"mode", required_argument, nullptr, 'm'},
			{"help", no_argument, nullptr, 'h'},
			{ nullptr, 0, nullptr, '\0' } };

		while ((option = getopt_long(argc, argv, "m:h", longOpts, &option_index)) != -1) {
			switch (option) {
			case 'm':
				if (*optarg == 'M') {
					mstMode = true;
				}
				else if (*optarg == 'F') {
					fastTspMode = true;
				}
				else if (*optarg == 'O') {
					optTspMode = true;
				}
				else {
					std::cerr << "Invalid argument to --mode" << std::endl;
					exit(1);
				}
				break;
			case 'h':
				std::cout << "poke help message" << std::endl;
				exit(0);
				break;
			default:
				std::cerr << "Unknown command line option" << std::endl;
				exit(1);
			}
		}
	}

	void construct() {
		int numPokemon_in = 0;
		cin >> numPokemon_in;
		numPokemon = numPokemon_in;

		// resize/reserve all vectors
		pokemon.reserve(numPokemon);

		// Read in all pokemon positions
		for (int i = 0; i < numPokemon; ++i) {
			int xCoordinate;
			int yCoordinate;
			cin >> xCoordinate >> yCoordinate;
			if ((xCoordinate == 0 && yCoordinate <= 0) || (yCoordinate == 0 && xCoordinate <= 0)) {
				pokemon.push_back({ xCoordinate, yCoordinate, i, pokeType::Coast });
			}
			else if (xCoordinate < 0 && yCoordinate < 0) {
				pokemon.push_back({ xCoordinate, yCoordinate, i, pokeType::Water });
			}
			else {
				pokemon.push_back({ xCoordinate, yCoordinate, i, pokeType::Land });
			}
		}
	}

	// distance helper functions
	double euclideanDistanceFly(Pokemon& poke1, Pokemon& poke2) {
		return sqrt((double(poke1.x) - double(poke2.x)) * (double(poke1.x) - double(poke2.x))
			+ (double(poke1.y) - double(poke2.y)) * (double(poke1.y) - double(poke2.y)));
	}
	
	double euclideanDistance(Pokemon &poke1, Pokemon &poke2) {
		if (((poke1.type == pokeType::Land) && (poke2.type == pokeType::Water))
			|| ((poke2.type == pokeType::Land) && (poke1.type == pokeType::Water))) {
			return numeric_limits<double>::infinity();
		}

		return sqrt((double(poke1.x) - double(poke2.x)) * (double(poke1.x) - double(poke2.x))
			+ (double(poke1.y) - double(poke2.y)) * (double(poke1.y) - double(poke2.y)));
	}

	// Part A functions
	void primLinearSearch() {
		// Check to see if MST is possible
		bool landPokeExists = false;
		bool waterPokeExists = false;
		bool coastPokeExists = false;
		for (size_t i = 0; i < pokemon.size(); ++i) {
			if (pokemon[i].type == pokeType::Land) landPokeExists = true;
			else if (pokemon[i].type == pokeType::Water) waterPokeExists = true;
			else coastPokeExists = true;
		}
		if (landPokeExists && waterPokeExists && !coastPokeExists) {
			cerr << "Cannot construct MST" << "\n";
			exit(1);
		}

		primTable.resize(numPokemon);
		primTable[0].d = 0;
		for (size_t i = 0; i < primTable.size(); ++i) {
			int VMin = getVMin();
			primTable[VMin].k = true;
			for (size_t j = 0; j < pokemon.size(); ++j) {
				const double dist = euclideanDistance(pokemon[VMin], pokemon[j]);
				if ((!primTable[j].k) && (dist < primTable[j].d)) {
					primTable[j].d = dist;
					primTable[j].p = VMin;
				}
			}
		}
	}

	// returns index of vertex with min d
	int getVMin() {
		double dMin = std::numeric_limits<double>::infinity();
		int VMin = 0;
		for (int i = 0; i < numPokemon; ++i) {
			if ((!primTable[i].k) && (primTable[i].d < dMin)) {
				dMin = primTable[i].d;
				VMin = i;
			}
		}
		return VMin;
	}

	void printMST() {
		primLinearSearch();
		double totalWeight = 0;

		for (size_t i = 0; i < primTable.size(); ++i) {
			totalWeight += primTable[i].d;
		}

		cout << totalWeight << "\n";
		for (int i = 0; i < numPokemon; ++i) {
			if (primTable[i].p != -1) {
				cout << min(i, primTable[i].p) << " ";
				cout << max(i, primTable[i].p) << "\n";
			}
		}
	}
	// Part A functions end

	// Part B functions
	void arbitraryInsertion() {
		fastTspSol.reserve(numPokemon);
		fastTspSol.push_back(0);
		fastTspSol.push_back(1);
		fastTspSol.push_back(2);

		for (int permLength = 3; permLength < numPokemon; ++permLength) {
			double minCost = numeric_limits<double>::infinity();
			int index = 0;
			for (int j = 0; j < permLength; ++j) {
				// Calculate cost of inserting into subtour
				double cost = 0;
				if (j == permLength - 1) {
					cost = euclideanDistanceFly(pokemon[permLength], pokemon[fastTspSol[j]])
						+ euclideanDistanceFly(pokemon[permLength], pokemon[fastTspSol[0]])
						- euclideanDistanceFly(pokemon[fastTspSol[j]], pokemon[fastTspSol[0]]);
				}
				else {
					cost = euclideanDistanceFly(pokemon[permLength], pokemon[fastTspSol[j]])
						+ euclideanDistanceFly(pokemon[permLength], pokemon[fastTspSol[j + 1]])
						- euclideanDistanceFly(pokemon[fastTspSol[j]], pokemon[fastTspSol[j + 1]]);
				}

				if (cost < minCost) {
					minCost = cost;
					index = j;
				}
			}
			fastTspSol.insert(fastTspSol.begin() + index + 1, permLength);
		}
	}

	double calculateFastTspCost() {
		double cost = 0;
		for (int i = 0; i < numPokemon - 1; ++i) {
			cost += euclideanDistanceFly(pokemon[fastTspSol[i]], pokemon[fastTspSol[i + 1]]);
		}
		cost += euclideanDistanceFly(pokemon[fastTspSol[0]], pokemon[fastTspSol[numPokemon - 1]]);
		return cost;
	}

	void printFastTsp() {
		double cost = calculateFastTspCost();
		cout << cost << "\n";
		for (int i = 0; i < numPokemon; ++i) {
			cout << fastTspSol[i] << " ";
		}
		cout << "\n";
	}
	// Part B functions end

	// Part C functions
	void initliazeUpperBound() {
		arbitraryInsertion();
		promisingUpperBound = calculateFastTspCost();
	}

	bool promising(vector<uint32_t>& path, int permLength) {

		// Calculate cost of current partial solution
		double partialSolutionCost = 0;
		for (int i = 0; i < permLength - 1; ++i) {
			partialSolutionCost += euclideanDistanceFly(pokemon[path[i]], pokemon[path[i + 1]]);
		}
		
		// Calculate cost of unconnected mst
		vector<Pokemon> unjoinedPokemon;
		unjoinedPokemon.resize(numPokemon - permLength);
		int numUnjoined = numPokemon - permLength;
		for (int i = permLength; i < numPokemon; ++i) {
			unjoinedPokemon[i - permLength] = pokemon[path[i]];
		}

		double mstCost = 0;
		primTable.clear();
		mstCost = primLinearSearchC(unjoinedPokemon, numUnjoined);

		// Find nearest vertex of MST and connect
		double rightCostMin = std::numeric_limits<double>::infinity();
		double leftCostMin = std::numeric_limits<double>::infinity();
		for (size_t j = 0; j < unjoinedPokemon.size(); ++j) {
			double rightEdgeCost = euclideanDistanceFly(pokemon[path[permLength - 1]], unjoinedPokemon[j]);
			double leftEdgeCost = euclideanDistanceFly(pokemon[path[0]], unjoinedPokemon[j]);
			if (rightEdgeCost < rightCostMin) {
				rightCostMin = rightEdgeCost;
			}
			if (leftEdgeCost < leftCostMin) {
				leftCostMin = leftEdgeCost;
			}
		}

		return mstCost + partialSolutionCost + rightCostMin + leftCostMin < promisingUpperBound;
	}

	double primLinearSearchC(vector<Pokemon>& unjoinedPokemon, int numUnjoined) {
		primTable.resize(numUnjoined);
		primTable[0].d = 0;
		for (size_t i = 0; i < primTable.size(); ++i) {
			int VMin = getVMinC(numUnjoined);
			primTable[VMin].k = true;
			for (int j = 0; j < numUnjoined; ++j) {
				double dist = euclideanDistanceFly(unjoinedPokemon[VMin], unjoinedPokemon[j]);
				if ((!primTable[j].k) && (dist < primTable[j].d)) {
					primTable[j].d = dist;
					primTable[j].p = VMin;
				}
			}
		}

		double totalWeight = 0;
		for (size_t i = 0; i < primTable.size(); ++i) {
			totalWeight += primTable[i].d;
		}
		return totalWeight;
	}

	// returns index of vertex with min d
	int getVMinC(int numUnjoined) {
		double dMin = std::numeric_limits<double>::infinity();
		int VMin = 0;
		for (int i = 0; i < numUnjoined; ++i) {
			if ((!primTable[i].k) && (primTable[i].d < dMin)) {
				dMin = primTable[i].d;
				VMin = i;
			}
		}
		return VMin;
	}

	void genPerms(vector<uint32_t>& path, int permLength) {
		if (permLength == numPokemon) {
			double cost = 0;
			for (int i = 0; i < numPokemon - 1; ++i) {
				cost += euclideanDistanceFly(pokemon[path[i]], pokemon[path[i + 1]]);
			}
			cost += euclideanDistanceFly(pokemon[path[0]], pokemon[path[numPokemon - 1]]);
			
			if (cost < promisingUpperBound) {
				promisingUpperBound = cost;
				optTspSol = path;
			}
			return;
		}
		if (!promising(path, permLength))
			return;
		for (size_t i = permLength; i < path.size(); ++i) {
			swap(path[permLength], path[i]);
			genPerms(path, permLength + 1);
			swap(path[permLength], path[i]);
		}
	}

	void solveTsp() {
		initliazeUpperBound();
		vector<uint32_t> path;
		path.resize(numPokemon);
		optTspSol.resize(numPokemon);
		for (int i = 0; i < numPokemon; ++i) {
			path[i] = i;
			optTspSol[i] = fastTspSol[i];
		}
		genPerms(path, 1);
	}

	void printOptTsp() {
		double cost = 0;
		for (int i = 0; i < numPokemon - 1; ++i) {
			cost += euclideanDistanceFly(pokemon[optTspSol[i]], pokemon[optTspSol[i + 1]]);
		}
		cost += euclideanDistanceFly(pokemon[optTspSol[0]], pokemon[optTspSol[numPokemon - 1]]);


		cout << cost << "\n";
		for (int i = 0; i < numPokemon; ++i) {
			cout << optTspSol[i] << " ";
		}
	}

	// End Part C functions

	void run() {
		if (mstMode) {
			printMST();
		}
		else if (fastTspMode) {
			arbitraryInsertion();
			printFastTsp();
		}
		else if (optTspMode){
			solveTsp();
			printOptTsp();
		}
	}

private:
	int numPokemon;
	double promisingUpperBound;
	bool mstMode;
	bool fastTspMode;
	bool optTspMode;
	vector<Pokemon> pokemon;
	vector<uint32_t> fastTspSol;
	vector<uint32_t> optTspSol;
	vector<PrimData> primTable;
};