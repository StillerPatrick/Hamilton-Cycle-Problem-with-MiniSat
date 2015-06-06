// main.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "Node.h"
#include "Edge.h"
#include "HandleFile.h"
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <string>
#include <utility>
#include <limits>

using namespace std;

vector<string> lines;
struct Node* Nodes = nullptr;
int n = 0;

size_t max_edges = 0; // maximum amount of edges from one node
size_t max_atom_length = 0; // maximum length of an atom (chars)
constexpr size_t max_bits = sizeof(int) * 8; // bits in int
size_t num_bits = 0; // (max bit size of node value) + 1

int edge_count = 0;
vector< vector<int> > edges;

int CallMiniSat(const string& inputpath, const string& outputpath){
    string systemcall = "minisat " + inputpath + " " + outputpath;
    return system(systemcall.c_str());
}

void initializeEdges(HandleFile *CurrentFile) {
    edges.insert(edges.begin(), CurrentFile->getNumOfEdges(), vector<int>());
    for (int i = 0; i < CurrentFile->getNumOfEdges(); i++) {
        string source ;
        string destination;
        string buffer ;
        buffer = CurrentFile->edges[i] ;
        buffer.erase(0,buffer.find(' ')+1);
        source = buffer.substr(0,buffer.find(' '));
        buffer.erase(0,buffer.find(' ')+1);
        destination = buffer.substr(0,buffer.find(' '));
        edges[atoi(source.c_str()) - 1].push_back(atoi(destination.c_str()));
    }
    for (const vector<int> &vec : edges) {
        if (vec.size() > max_edges) max_edges = vec.size();
    }
}

void intitializeNodes(HandleFile* CurrentFile){
    Nodes = (struct Node*) malloc(sizeof(struct Node)*CurrentFile->getNumOfNodes() * (CurrentFile->getNumOfNodes()+1));
    int count = 0 ;
    int time = CurrentFile->getNumOfNodes()+1 ;
    cout << "Time: " << time << endl ;
    // i is the runtime for the node id
    // j is the runtime for the node time
    for(int i = 1; i < CurrentFile->getNumOfNodes()+1 ; i++)
    {
        for(int j = 0; j < CurrentFile->getNumOfNodes()+1; j++)
        {
            Nodes[count].id = i ;
            Nodes[count].time = j ;
            count++ ;
        }
    }
    cout << "Num of initialized nodes: " << count << endl ;
    n = CurrentFile->getNumOfNodes();
}

// encodes a node and a time value to a MiniSAT Value
inline string encode(int node, int time) {
    return to_string((time << num_bits) | node);
}

// decodes a value from MiniSAT
// returns a (node, time)-pair
std::pair<int, int> decode(int dec_val) {
    return {dec_val & ~(~0 << num_bits), dec_val >> num_bits};
}

// returns maximum length of a clause
inline size_t clause_length(size_t atoms) {

    return atoms * max_atom_length + 2 /* "0\n" */;
}


/* Performance optimization:
    - nest the for-loops
    - do not access the Nodes array, work with indices directly
*/
void generateCNF() {
    string alpha;
    int clause_count = 0;

    /* APPROXIMATE size of the whole string */
    size_t size = 3 * n             * clause_length(1) +\
                  n * (2 * + n * n) * clause_length(n) +\
                  n * n * n         * clause_length(2) +\
                  n * n             * clause_length(max_edges);
    alpha.reserve(size);

    /* Initialise maximum atom length (chars)
       [MINUS][NUMBER][SPACE]
    */
    max_atom_length = encode(n,n).size() + 2;

    /* Initialises number of bits a node value can hold at max */
    for (num_bits = 0; num_bits < max_bits; num_bits++) {
        if ((n >> num_bits) == 0) break;
    }

    /* Each node must be visited at least once */
    alpha += encode(1, 0);alpha += " 0\n"; /* Node 1 can be visited only at step 0 */
    alpha += encode(1, Nodes[n].time);alpha += " 0\n";	/* and at step n */
    clause_count += n + 1;
    for (int i = n + 1; i < n * (n + 1); i += n + 1) {
        for (int j = i + 1; j < i + n; j++) {
            alpha += encode(Nodes[j].id, Nodes[j].time);alpha += " ";
        }
        alpha += " 0\n";
    }

    /* Each node must be visited only once*/
    clause_count += n - 1;
    for (int i = 1; i < n; i++) {
        /* Node 1 cannot be visited at any step other than 0 or n */
        alpha += "-";alpha += encode(Nodes[i].id, Nodes[i].time);alpha += " 0\n";
    }

    for (int i = n + 1; i < n * (n + 1); i += n + 1) {
        for (int j = i + 1; j < i + n - 1; j++) {
            for (int k = j + 1; k < i + n; k++) {
                alpha += "-";alpha += encode(Nodes[j].id, Nodes[j].time);alpha += " -";alpha += encode(Nodes[k].id, Nodes[k].time);alpha += " 0\n";
                clause_count++;
            }
        }
    }

    /* At each step at least one node must be visited */
    // The clauses for step 0 and n visiting node 1 already exist
    clause_count += n - 1;
    for (int i = 1; i < n; i++) {
        for (int j = (n + 1) + i; j < n * (n + 1); j += n + 1) {
            alpha += encode(Nodes[j].id, Nodes[j].time);alpha += " ";
        }
        alpha += " 0\n";
    }

    /* At each step only one node must be visited */
    clause_count += 2 * (n - 1);
    for (int i = n + 1; i < n * (n + 1); i += n + 1) {
        /* Steps 0 and n cannot visit any node other than 1 */
        alpha += "-";alpha += encode(Nodes[i].id, Nodes[i].time);alpha += " 0\n";
        alpha += "-";alpha += encode(Nodes[i + n].id, Nodes[i + n].time);alpha += " 0\n";
    }

    for (int i = 1; i < n; i++) {
        for (int j = (n + 1) + i; j < n * n; j += n + 1) {
            for (int k = (n + 1) + j; k < n * (n + 1); k += n + 1) {
                alpha += "-";alpha += encode(Nodes[j].id, Nodes[j].time);alpha += " -";alpha += encode(Nodes[k].id, Nodes[k].time);alpha += " 0\n";
                clause_count++;
            }
        }
    }

    clause_count += n * n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            alpha += "-";alpha += encode(j + 1, i);alpha += " ";
            for (unsigned int k = 0; k < edges[j].size(); k++) {
                alpha += encode(edges[j][k], i + 1);alpha += " ";
            }
            alpha += "0\n";
        }
    }
    /*
    ofstream file;
    file.open("cnf.in");
    file << "p cnf " << encode(n, n) << " " << clause_count << endl << alpha;
    file.close();
    */
    string header;
    header += "p cnf " + encode(n, n) + " " + to_string(clause_count) + "\n";
    FILE *cnf;
    cnf = fopen("cnf.in", "w");
    fwrite(header.c_str(), header.length(), 1, cnf);
    fwrite(alpha.c_str(), alpha.length(), 1, cnf);
    fclose(cnf);
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        return -1;
    }

    HandleFile CurrentFile(argv[1]); //Change your Path */

    cout << "Filepath: " << CurrentFile.getPath() << endl;
    cout << "Number of Edges: " << CurrentFile.getNumOfEdges() << endl;
    cout << "Number of Nodes: " << CurrentFile.getNumOfNodes() << endl;
    intitializeNodes(&CurrentFile);
    initializeEdges(&CurrentFile);

    //
    //Example for simple MiniSAT call
    // "test.in" is a example file
    cout << "Generating CNF ... ";
    cout.flush();
    generateCNF();
    cout << "Done" << endl;

    int minisatreturn = CallMiniSat("cnf.in","../cnf.out");
    if (WEXITSTATUS(minisatreturn) == 10) {
        cout << "Solution:" << endl;
        ifstream out("../cnf.out", std::ifstream::in);
        out.ignore(numeric_limits<streamsize>::max(),'\n'); // skip "SAT" line
        int x;
        out >> x;
        out.ignore();
        while(out.good()) {
            if (x > 0) {
                std::pair<int,int> node_time;
                node_time = decode(x);
                cout << "Value: " << x << " Node: " << node_time.first << " Time: " << node_time.second << endl;
            }
            out >> x;
            out.ignore();
        }
        out.close();
    }

    return WEXITSTATUS(minisatreturn);
}
