// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include"pokeGraph.h"
#include<iomanip>

using namespace std;

int main(int argc, char** argv) {
	cout << std::setprecision(2); // Always show 2 decimal places
	cout << std::fixed; // Disable scientific notation for large numbers

	PokeGraph graph;
	graph.get_options(argc, argv);
	graph.construct();
	graph.run();

	return 0;
}