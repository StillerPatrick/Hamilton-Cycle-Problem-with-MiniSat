#include <vector>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include <sstream>
#include <map>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"

using namespace Minisat;
using namespace std;

enum {
    EDGE = 'e',
    PROPERTY = 'p'
};

vector<vector<int>> edges;
map<int, int> file_to_vars;
map<int, int> vars_to_file;
unsigned int num_nodes = 0;
unsigned int num_edges = 0;
constexpr size_t max_bits = sizeof(int) * 8; // bits in int
size_t num_bits = 0; // (max bit size of node value) + 1

string parseProperties(string const &property_line) {
    istringstream properties(property_line);
    properties.ignore(7);     /* ignore "p edges" */
    properties >> std::skipws >> num_nodes;
    properties >> std::skipws >> num_edges;
    return property_line;
}

void parseEdge(string const &edge_line) {
    static bool uninitialized = true;
    if (uninitialized) {
        edges.insert(edges.begin(), num_edges, vector<int>());
        uninitialized = false;
    }
    unsigned int source;
    unsigned int destination;
    istringstream edge(edge_line);
    edge.ignore();      /* ignore leading 'e' */
    edge >> skipws >> source;
    edge >> skipws >> destination;
    if (file_to_vars.count(source) == 0) {
        file_to_vars.insert(pair<int, int>(source, file_to_vars.size() + 1));
        vars_to_file.insert(pair<int, int>(vars_to_file.size() + 1, source));
    }
    if (file_to_vars.count(destination) == 0) {
        file_to_vars.insert(pair<int, int>(destination, file_to_vars.size() + 1));
        vars_to_file.insert(pair<int, int>(vars_to_file.size() + 1, destination));
    }
    if (file_to_vars.at(source) > num_nodes) {
        edges.insert(edges.end(), file_to_vars.at(source) - num_nodes, vector<int>());
        num_nodes = file_to_vars.at(source);
    }
    edges[file_to_vars.at(source) - 1].push_back(file_to_vars.at(destination));
}

void parseDIMACSGraph(char *path) {
    ifstream file(path, std::ifstream::in); // file handle
    string properties; // property string
    string line; // buffer string
    while (file.good()) {
        getline(file, line);
        switch (line.front()) {
            case PROPERTY:
            if (properties.empty()) {
                properties = parseProperties(line);
            }
            break;
            case EDGE:
            parseEdge(line); break;
        }
	}
	file.close();
    if (file_to_vars.size() != num_nodes) {
        num_nodes = file_to_vars.size();
    }
#if 0
    for (int i = 1; i <= num_nodes; i++) {
        cerr << "Variable: " << i << ", node: " << vars_to_file.at(i) << endl;
    }
#endif
}

/* encodes a node and a time value to a MiniSAT Value */
int encode(int node, int time) {
    return (time << num_bits) | node - 1;
}

/* decodes a value from MiniSAT
 * returns a (node, time)-pair
 */
std::pair<int, int> decode(int dec_val) {
    return {dec_val & ~(~0 << num_bits), dec_val >> num_bits};
}

void generateCNF(Solver &solver) {
    vec<Lit> n_clause;
    vec<Lit> one_clause;
    vec<Lit> two_clause;

    /* Visit each node at least once */
    one_clause.push(mkLit(encode(1, 0)));
    solver.addClause_(one_clause);
    one_clause.clear();
    one_clause.push(mkLit(encode(1, num_nodes)));
    solver.addClause_(one_clause);
    one_clause.clear();
    for (register int i = 2; i <= num_nodes; i++) {
        for (register int j = 1; j < num_nodes; j++) {
            n_clause.push(mkLit(encode(i, j)));
        }
        solver.addClause_(n_clause);
        n_clause.clear();
    }

    /* Visit each node at most once */
    for (register int i = 1; i < num_nodes; i++) {
        one_clause.push(~mkLit(encode(1, i)));
        solver.addClause_(one_clause);
        one_clause.clear();
    }
    for (register int i = 2; i <= num_nodes; i++) {
        for (register int j = 1; j < num_nodes - 1; j++) {
            for (register int k = j + 1; k < num_nodes; k++) {
                two_clause.push(~mkLit(encode(i, j)));
                two_clause.push(~mkLit(encode(i, k)));
                solver.addClause_(two_clause);
                two_clause.clear();
            }
        }
    }

    /* Visit at least one node at each step */
    for (register int i = 1; i < num_nodes; i++) {
        for (register int j = 2; j <= num_nodes; j++) {
            n_clause.push(mkLit(encode(j, i)));
        }
        solver.addClause_(n_clause);
        n_clause.clear();
    }

    /* Visit at most one node at each step */
    for (register int i = 2; i <= num_nodes; i++) {
        one_clause.push(~mkLit(encode(i, 0)));
        solver.addClause_(one_clause);
        one_clause.clear();
        one_clause.push(~mkLit(encode(i, num_nodes)));
        solver.addClause_(one_clause);
        one_clause.clear();
    }
    for (register int i = 1; i < num_nodes; i++) {
        for (register int j = 2; j < num_nodes; j++) {
            for (register int k = j + 1; k <= num_nodes; k++) {
                two_clause.push(~mkLit(encode(j, i)));
                two_clause.push(~mkLit(encode(k, i)));
                solver.addClause_(two_clause);
                two_clause.clear();
            }
        }
    }

    /* Successors of each node at each step depending on edges */
    for (register int i = 1; i <= num_nodes; i++) {
        for (register int j = 0; j < num_nodes; j++) {
            n_clause.push(~mkLit(encode(i, j)));
            for (register int k = 0; k < edges[i - 1].size(); k++) {
                n_clause.push(mkLit(encode(edges[i - 1][k], j + 1)));
            }
            solver.addClause_(n_clause);
            n_clause.clear();
        }
    }
}

int main(int argc, char* argv[]) {
    Solver solver;

    if (argc != 2) {
        return -1;
    }
    parseDIMACSGraph(argv[1]);

    /* Get maximum number of bits for variable encoding */
    for (num_bits = 0; num_bits < max_bits; num_bits++) {
        if ((num_nodes >> num_bits) == 0) break;
    }

    /* Pass the number of variables to minisat */
    for (register int max_var = encode(num_nodes, num_nodes) + 1;
            solver.nVars() < max_var;
            solver.newVar())
        ; /* PASS */

    generateCNF(solver);

    if (!solver.simplify()) {
        cout << "s UNSATISFIABLE" << endl;
        exit(20);
    }
    bool result = solver.solve();

    if (result == true) {
        cout << "s SATISFIABLE" << endl << "v ";
        for (register int i = 0; i < solver.nVars(); i++) {
            if (solver.model[i] == l_True && std::get<0>(decode(i + 1)) != 1) {
                cout << vars_to_file.at(std::get<0>(decode(i + 1))) << " ";
            }
        }
        cout << vars_to_file.at(1) << endl;
        exit(10);
    } else if (result == false) {
        cout << "s UNSATISFIABLE" << endl;
        exit(20);
    }
    return 0;
}
