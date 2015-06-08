#include "Node.h"
#include "HandleFile.h"
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include <sstream>

using namespace std;

vector<struct Node> nodes;
unsigned int n = 0; // number of nodes

size_t max_edges = 0; // maximum amount of edges from one node
size_t max_atom_length = 0; // maximum length of an atom (chars)
constexpr size_t max_bits = sizeof(int) * 8; // bits in int
size_t num_bits = 0; // (max bit size of node value) + 1

vector<vector<int>> edges;

/* Start miniSAT with apropriate parameters */
int CallMiniSat(const string& inputpath, const string& outputpath){
    string systemcall = "minisat " + inputpath + " " + outputpath;
    return system(systemcall.c_str());
}

/* initialise global variables 'edges' & 'max_edges' */
void initialiseEdges(HandleFile& CurrentFile) {
    edges.insert(edges.begin(), CurrentFile.getNumOfEdges(), vector<int>());

    /* read from line to 'edges' */
    for (unsigned int i = 0; i < CurrentFile.getNumOfEdges(); i++) {
        unsigned int source;
        unsigned int destination;
        istringstream edge_line(CurrentFile.edges[i]);
        edge_line.ignore(); // ignore 'e'
        edge_line >> skipws >> source;
        edge_line >> skipws >> destination;
        edges[source - 1].push_back(destination);
    }
    /* get maximum amount of edges from one node */
    for (const vector<int> &vec : edges) {
        if (vec.size() > max_edges) max_edges = vec.size();
    }
}

/* initialise global variable 'nodes' */
void initialiseNodes(HandleFile& CurrentFile) {
    n = CurrentFile.getNumOfNodes();
    nodes.reserve(n * (n + 1));

    unsigned count = 0;
    for (unsigned int i = 1; i < n + 1; i++) {
        for (unsigned int j = 0; j < n + 1; j++, count++) {
            nodes[count] = {i, j};
        }
    }

    cout << "c Number of initialised nodes: " << count << endl ;
}

/* encodes a node and a time value to a MiniSAT Value */
inline string encode(int node, int time) {
    return to_string((time << num_bits) | node);
}

/* decodes a value from MiniSAT
 * returns a (node, time)-pair
 */
inline std::pair<int, int> decode(int dec_val) {
    return {dec_val & ~(~0 << num_bits), dec_val >> num_bits};
}

/* returns maximum length of a clause */
inline size_t clause_length(size_t atoms) {
    return atoms * max_atom_length + 2 /* "0\n" */;
}

/* Performance optimization:
 *   - nest the for-loops
 *   - do not access the nodes array, work with indices directly
 */
void generateCNF() {
    string alpha;
    int clause_count = 0;

    /* Initialise maximum atom length (chars)
     * [MINUS][NUMBER][SPACE]
     */
    max_atom_length = encode(n,n).length() + 2;

    /* Initialises number of bits a node value can hold at max */
    for (num_bits = 0; num_bits < max_bits; num_bits++) {
        if ((n >> num_bits) == 0) break;
    }

    /* APPROXIMATE size of the whole string */
    size_t size = 3 * n             * clause_length(1) +\
                  2 * n * n * n     * clause_length(2) +\
                  n                 * clause_length(n) +\
                  n * n             * clause_length(max_edges + 1);
    alpha.reserve(size);

    /* Each node must be visited at least once */
    alpha += encode(1, 0);alpha += " 0\n"; /* Node 1 can be visited only at step 0 */
    alpha += encode(1, nodes[n].time);alpha += " 0\n";	/* and at step n */
    clause_count += n + 1;
    for (unsigned int i = n + 1; i < n * (n + 1); i += n + 1) {
        for (unsigned int j = i + 1; j < i + n; j++) {
            alpha += encode(nodes[j].id, nodes[j].time);alpha += " ";
        }
        alpha += " 0\n";
    }

    /* Each node must be visited only once*/
    clause_count += n - 1;
    for (unsigned int i = 1; i < n; i++) {
        /* Node 1 cannot be visited at any step other than 0 or n */
        alpha += "-";alpha += encode(nodes[i].id, nodes[i].time);alpha += " 0\n";
    }
    for (unsigned int i = n + 1; i < n * (n + 1); i += n + 1) {
        for (unsigned int j = i + 1; j < i + n - 1; j++) {
            for (unsigned int k = j + 1; k < i + n; k++) {
                alpha += "-";alpha += encode(nodes[j].id, nodes[j].time);alpha += " -";alpha += encode(nodes[k].id, nodes[k].time);alpha += " 0\n";
                clause_count++;
            }
        }
    }

    /* At each step at least one node must be visited
     * The clauses for step 0 and n visiting node 1 already exist
     */
    clause_count += n - 1;
    for (unsigned int i = 1; i < n; i++) {
        for (unsigned int j = (n + 1) + i; j < n * (n + 1); j += n + 1) {
            alpha += encode(nodes[j].id, nodes[j].time);alpha += " ";
        }
        alpha += " 0\n";
    }

    /* At each step only one node must be visited */
    clause_count += 2 * (n - 1);
    for (unsigned int i = n + 1; i < n * (n + 1); i += n + 1) {
        /* Steps 0 and n cannot visit any node other than 1 */
        alpha += "-";alpha += encode(nodes[i].id, nodes[i].time);alpha += " 0\n";
        alpha += "-";alpha += encode(nodes[i + n].id, nodes[i + n].time);alpha += " 0\n";
    }
    for (unsigned int i = 1; i < n; i++) {
        for (unsigned int j = (n + 1) + i; j < n * n; j += n + 1) {
            for (unsigned int k = (n + 1) + j; k < n * (n + 1); k += n + 1) {
                alpha += "-";alpha += encode(nodes[j].id, nodes[j].time);alpha += " -";alpha += encode(nodes[k].id, nodes[k].time);alpha += " 0\n";
                clause_count++;
            }
        }
    }

    /* All exisiting edges at any time */
    clause_count += n * n;
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n; j++) {
            alpha += "-";alpha += encode(j + 1, i);alpha += " ";
            for (unsigned int k = 0; k < edges[j].size(); k++) {
                alpha += encode(edges[j][k], i + 1);alpha += " ";
            }
            alpha += "0\n";
        }
    }


    /* Write full problem specification to "cnf.in" */
    string header;
    header += "p cnf " + encode(n, n) + " " + to_string(clause_count) + "\n";
    FILE *cnf;
    cnf = fopen("cnf.in", "w");
    fwrite(header.c_str(), header.length(), 1, cnf);
    fwrite(alpha.c_str(), alpha.length(), 1, cnf);
    fclose(cnf);
}

/* start point of console application */
int main(int argc, char* argv[]) {
    if (argc != 2) {
        return -1;
    }

    HandleFile CurrentFile(argv[1]); /* Change your Path */

    cout << "c Filepath: " << CurrentFile.getPath() << endl;
    cout << "c Number of edges: " << CurrentFile.getNumOfEdges() << endl;
    cout << "c Number of nodes: " << CurrentFile.getNumOfNodes() << endl;
    initialiseNodes(CurrentFile);
    initialiseEdges(CurrentFile);

    /* Example for simple MiniSAT call
     * "test.col" is a example file
     */
    cout << "c Generating CNF ... "; cout.flush();
    generateCNF();
    cout << "Done" << endl;

    int minisatreturn = CallMiniSat("cnf.in","../cnf.out");
    if (WEXITSTATUS(minisatreturn) == 10) {
        cout << "c Solution:" << endl;
        ifstream out("../cnf.out", std::ifstream::in);
        out.ignore(numeric_limits<streamsize>::max(),'\n'); // skip "SAT" line
        int x;
        out >> x;
        out.ignore();
        while(out.good()) {
            if (x > 0) {
                std::pair<int,int> node_time;
                node_time = decode(x);
                cout << "c Value: " << x << " Node: " << node_time.first << " Time: " << node_time.second << endl;
            }
            out >> x;
            out.ignore();
        }
        out.close();
    }

    return WEXITSTATUS(minisatreturn);
}
