#include <vector>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include <sstream>
#include "minisat/core/SolverTypes.h"
#include "minisat/simp/SimpSolver.h"

using namespace Minisat;
using namespace std;

enum {
    EDGE = 'e',
    PROPERTY = 'p'
};

vector<vector<int>> edges;
unsigned int num_nodes = 0;
unsigned int num_edges = 0;
constexpr size_t max_bits = sizeof(int) * 8; // bits in int
size_t num_bits = 0; // (max bit size of node value) + 1

string parseProperties(string property_line) {
    istringstream properties(property_line);
    properties.ignore(7);     /* ignore "p edges" */
    properties >> std::skipws >> num_nodes;
    properties >> std::skipws >> num_edges;
    return property_line;
}

void parseEdge(string edge_line) {
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
    edges[source - 1].push_back(destination);
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

void generateCNF(SimpSolver &solver) {
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
    for (int i = 2; i <= num_nodes; i++) {
        for (int j = 1; j < num_nodes; j++) {
            n_clause.push(mkLit(encode(i, j)));
        }
        solver.addClause_(n_clause);
        n_clause.clear();
    }

    /* Visit each node at most once */
    for (int i = 1; i < num_nodes; i++) {
        one_clause.push(~mkLit(encode(1, i)));
        solver.addClause_(one_clause);
        one_clause.clear();
    }
    for (int i = 2; i <= num_nodes; i++) {
        for (int j = 1; j < num_nodes - 1; j++) {
            for (int k = j + 1; k < num_nodes; k++) {
                two_clause.push(~mkLit(encode(i, j)));
                two_clause.push(~mkLit(encode(i, k)));
                solver.addClause_(two_clause);
                two_clause.clear();
            }
        }
    }

    /* Visit at least one node at each step */
    for (int i = 1; i < num_nodes; i++) {
        for (int j = 2; j <= num_nodes; j++) {
            n_clause.push(mkLit(encode(j, i)));
        }
        solver.addClause_(n_clause);
        n_clause.clear();
    }

    /* Visit at most one node at each step */
    for (int i = 2; i <= num_nodes; i++) {
        one_clause.push(~mkLit(encode(i, 0)));
        solver.addClause_(one_clause);
        one_clause.clear();
        one_clause.push(~mkLit(encode(i, num_nodes)));
        solver.addClause_(one_clause);
        one_clause.clear();
    }
    for (int i = 1; i < num_nodes; i++) {
        for (int j = 2; j < num_nodes; j++) {
            for (int k = j + 1; k <= num_nodes; k++) {
                two_clause.push(~mkLit(encode(j, i)));
                two_clause.push(~mkLit(encode(k, i)));
                solver.addClause_(two_clause);
                two_clause.clear();
            }
        }
    }

    /* Successors of each node at each step depending on edges */
    for (int i = 1; i <= num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            n_clause.push(~mkLit(encode(i, j)));
            for (int k = 0; k < edges[i - 1].size(); k++) {
                n_clause.push(mkLit(encode(edges[i - 1][k], j + 1)));
            }
            solver.addClause_(n_clause);
            n_clause.clear();
        }
    }
}

int main(int argc, char* argv[]) {
    SimpSolver solver;

    if (argc != 2) {
        return -1;
    }
    parseDIMACSGraph(argv[1]);

    /* Get maximum number of bits for variable encoding */
    for (num_bits = 0; num_bits < max_bits; num_bits++) {
        if ((num_nodes >> num_bits) == 0) break;
    }

    /* Pass the number of variables to minisat */
    for (int max_var = encode(num_nodes, num_nodes) + 1;
            solver.nVars() < max_var;
            solver.newVar())
        ; /* PASS */

    generateCNF(solver);

    solver.eliminate(true);         /* Performance test later */
    vec<Lit> dummy;
    lbool result = l_Undef;
    result = solver.solveLimited(dummy);

    if (result == l_True) {
        cout << "s SATISFIABLE" << endl << "v ";
        for (int i = 0; i < solver.nVars(); i++) {
            if (solver.model[i] == l_True && std::get<0>(decode(i + 1)) != 1) {
                cout << std::get<0>(decode(i + 1)) << " ";
            }
        }
        cout << "1" << endl;
        exit(10);
    } else if (result == l_False) {
        cout << "s UNSATISFIABLE" << endl;
        exit(20);
    }
    return 0;
}
