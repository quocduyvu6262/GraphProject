/**
 * Name: Duy Vu
 * PID: A17190576
 * Email: dqvu@ucsd.edu
 * 
 * Sources: Lecture slide
 * This file provides the implementaion for 
 * methods described in Graph header file.
 */

#include <iostream>
#include <climits>
#include "Graph.h"
#include "fstream"
#include "sstream"
#include "algorithm"
#include "queue"

using namespace std;
// store <weight, label>
typedef pair<int, string> Pair;  
// store <label_u, label_v, weight>
typedef tuple<string, string, int> Tuple; 
// define weight index constant
const int WEIGHT_INDEX = 2; 


Graph::Graph(const string &edgelist_csv_fn) {
    // initialize instance variables 
    nodes_size = 0;
    edges_size = 0;
    ifstream inFile(edgelist_csv_fn); // open file 
    string line; // store current line 
    while(getline(inFile,line)){
        istringstream ss(line); 
        string vertex_u, vertex_v, weight; // variables 
        // store each column data into variables 
        getline(ss, vertex_u, ',');
        getline(ss, vertex_v, ',');
        getline(ss, weight, '\n');
        // add edge
        add_edge(vertex_u, vertex_v, stoi(weight));
        edges_size += 1;
    }
    inFile.close();
}

unsigned int Graph::num_nodes() {
    // TODO
    return adjlist.size();
}

unordered_set<string> Graph::nodes() {
    // TODO
    unordered_set<string>nodes;
    // insert vertices into nodes set
    for(const auto &el : adjlist){
        nodes.insert(el.first);
    }

    return nodes;
}

unsigned int Graph::num_edges() {
    // TODO
    return edges_size;
}

unsigned int Graph::num_neighbors(string const &node_label) {
    // TODO
    // return vector size at node_label
    return adjlist[node_label].size();
}

int Graph::edge_weight(string const &u_label, string const &v_label) {
    // TODO
    // check if edge exists between two vertices
    for(const auto &el : adjlist[u_label]){
        if(v_label.compare(el.first) == 0){
            return el.second;
        }
    }
    // otherwise
    return -1;
}

unordered_set<string> Graph::neighbors(string const &node_label) {
    // TODO
    unordered_set<string> neighbors;
    // Itereate through vector at node_label
    for(const auto &el : adjlist[node_label]){
        // add neighbor to unordered set
        neighbors.insert(el.first);
    }
    return neighbors;
}

vector<tuple<string, string, int>>
Graph::shortest_path_weighted(string const &start_label,
                              string const &end_label) {
    // TODO
    vector<tuple<string, string, int>> shortest_path;
    vector<tuple<string, string, int>> temp_shortest_path;
    // if label doesn't exist
    if(adjlist[start_label].size() == 0
        || adjlist[end_label].size() == 0){
        //return empty
        return shortest_path;
    }
    // if start_label and end_label is the same
    if(start_label.compare(end_label) == 0){
        shortest_path.push_back(make_tuple(start_label,end_label,0));
        return shortest_path;
    }
    // Otherwise,
    // implement the Dijkstra’s Algorithm
    // to find least cost path from source
    unordered_map<string,int> dist;
    for(const auto &el : adjlist){
        if(el.first.compare(start_label) == 0){
            dist[el.first] = 0;
        } else {
            dist[el.first] = INT_MAX;
        }
    }
    // previous node of that best path
    // Pair store <weight,label>
    unordered_map<string,Pair> prev;

    // priority queue to store pair (pair store <weight,label>)
    priority_queue<Pair, vector<Pair>, Graph::My_Comparator> priority_queue;

    priority_queue.push(make_pair(0,start_label));


    string shortest_label;
    while(!priority_queue.empty()){
        // get first minimum distance in priority queue
        shortest_label = priority_queue.top().second;
        priority_queue.pop();
        // get adjacent vertices of current vertex
        string label_v;
        int weight_v;
        for(const auto &itr : adjlist[shortest_label]){
            // get vertex label and weight
            label_v = itr.first;
            weight_v = itr.second;
            // if there is other shorest path from 
            // current vertex to label_v
            if(dist[shortest_label] + weight_v < dist[label_v]){
                // then update the dist
                dist[label_v] = dist[shortest_label] + weight_v;
                priority_queue.push(make_pair(dist[label_v], label_v));
                // update prev
                prev[label_v] = make_pair(weight_v, shortest_label);
            }
        }
    }
    // check if path between start_label and end_label exists
    if(prev.find(end_label) == prev.end()){
        // if no path, return empty vector
        return shortest_path;
    }
    // otherwise backtracking from end_label 
    Pair prev_pair = prev[end_label];
    string current_label;
    temp_shortest_path.push_back(
        make_tuple(prev_pair.second,end_label,prev_pair.first));
    while(prev_pair.second.compare(start_label) != 0){
        current_label = prev_pair.second;
        prev_pair = prev[current_label];
        temp_shortest_path.push_back(
            make_tuple(prev_pair.second,current_label,prev_pair.first));
    }
    // iterate temp vector backward and add to shortest path
    vector<tuple<string, string, int>>::iterator itr = temp_shortest_path.end();
    while(itr != temp_shortest_path.begin()){
        --itr;
        shortest_path.push_back(*itr);
    }
    return shortest_path;
}

int Graph::smallest_connecting_threshold(string const &start_label,
                                         string const &end_label) {
    // define smallest threshold
    int smallest_threshold;
    // if label doesn't exist
    if(adjlist[start_label].size() == 0
        || adjlist[end_label].size() == 0){
        //return 
        return -1;
    }
    // if start label and end_label is the same
    if(start_label.compare(end_label) == 0){
        return 0;
    }
    // sort edges in increasing order based on weight 
    sort(edges.begin(), edges.end(),
        [](const tuple<string, string, int> edge_e,
            const tuple<string, string, int> edge_f){
        return get<WEIGHT_INDEX>(edge_e) < get<WEIGHT_INDEX>(edge_f);
    });
    unordered_set<string> nodes_set = nodes();
    Graph::Disjoint_Sets disjoint_sets(nodes_set);
    // Iterate through edges
    string label_u, label_v, sentinel_u, sentinel_v;
    string sentinel_start, sentinel_end;
    for(auto itr = edges.begin(); itr != edges.end(); ++itr){
        label_u = get<0>(*itr);
        label_v = get<1>(*itr);
        // use find to find sentinel nodes
        sentinel_u = disjoint_sets.find(label_u);
        sentinel_v = disjoint_sets.find(label_v);
        // if two nodes are not in same set, then union
        if(sentinel_u.compare(sentinel_v) != 0){
            disjoint_sets.union_by_size(sentinel_u,sentinel_v);
            // check if start label and end label are in same set
            sentinel_start = disjoint_sets.find(start_label);
            sentinel_end = disjoint_sets.find(end_label);
            if(sentinel_start.compare(sentinel_end) == 0){
                // since they are in the same set,
                // the explored edge u-v forms a path
                // from start_label to end_label. 
                // Because all edges are sorted in the 
                // increasing order, this explored edge weight 
                // is largest in this path, and also is the 
                // smallest connecting threshold
                smallest_threshold = get<WEIGHT_INDEX>(*itr);
                return smallest_threshold;
            }
        }
    }
    // if start label and end label not in same set
    return -1;
}

void Graph::add_edge(string const &vertex_u, 
        string const &vertex_v, int const &weight){
    // add edge from u to v
    adjlist[vertex_u].push_back(make_pair(vertex_v,weight));
    // add edge from v to u
    adjlist[vertex_v].push_back(make_pair(vertex_u,weight));
    // store edges in edges
    edges.push_back(make_tuple(vertex_u,vertex_v,weight));
}

Graph::Disjoint_Sets::Disjoint_Sets(unordered_set<string> &nodes){
    // initialize parent from set of nodes
    for(const auto &el : nodes){
        // every element is parent itself
        parent[el] = el;
        sizes[el] = 1;
    }
}

string Graph::Disjoint_Sets::find(string label){
    // recursively call until reaching the sentinel node
    if(parent[label].compare(label) != 0){
        parent[label] = find(parent[label]);
    }
    return parent[label];
}

void Graph::Disjoint_Sets::union_by_size(
        string label_u, string label_v){
    string sentinel_u = find(label_u);
    string sentinel_v = find(label_v);
    // compare size to determine the parent node
    if(sizes[sentinel_u] > sizes[sentinel_v]){
        parent[sentinel_v] = sentinel_u;
        //update sizes
        sizes[sentinel_u] += sizes[sentinel_v];
    } else {
        parent[sentinel_u] = sentinel_v;
        //update sizes
        sizes[sentinel_v] += sizes[sentinel_u];
    }
}