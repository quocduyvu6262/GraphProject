#include <iostream>
#include <string>
#include <chrono>
#include <limits>
#include <unordered_set>
#include "Graph.h"

/* Macro to explicity print tests that are run along with colorized result. */
#define TEST(EX) (void)((fprintf(stdout, "(%s:%d) %s:", __FILE__, __LINE__,\
                 #EX)) && ((EX && fprintf(stdout, "\t\x1b[32mPASSED\x1b[0m\n"))\
                 || (fprintf(stdout, "\t\x1b[31mFAILED\x1b[0m\n")) ))

bool compare(const double& d1, const double& d2) {
    return std::abs(d1 - d2) <= std::numeric_limits<double>::epsilon();
}

int main() {
    Graph graph("example/small.csv");
    auto n = graph.nodes();
    TEST(graph.nodes() == unordered_set<string>({"A", "B", "C", "D", "E", "F", "G"}));
    TEST(graph.num_nodes() == 7);
    TEST(graph.edge_weight("A", "B") == 1);
    TEST(graph.edge_weight("E", "F") == 4);
    TEST(graph.edge_weight("A", "D") == -1);
    TEST(graph.edge_weight("C", "E") == -1);
    TEST(graph.num_edges() == 6);

    vector<tuple<string, string, int>> result {{"A", "B", 1}, {"B", "C", 1}};
    auto start = std::chrono::steady_clock::now();
    TEST(graph.shortest_path_weighted("A", "C") == result);
    vector<tuple<string, string, int>> result1 {{"B", "B", 0}};
    TEST(graph.shortest_path_weighted("B", "B") == result1);
    vector<tuple<string, string, int>> result2 {};
    TEST(graph.shortest_path_weighted("A", "E") == result2);
    TEST(graph.shortest_path_weighted("A", "Z") == result2);
    TEST(graph.shortest_path_weighted("Z", "Z") == result2);
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    TEST(elapsed.count() <= 1000.0); // microseconds

    start = std::chrono::steady_clock::now();
    TEST(graph.smallest_connecting_threshold("A", "C") == 1);
    TEST(graph.smallest_connecting_threshold("E", "G") == 5);
    TEST(graph.smallest_connecting_threshold("A", "D") == 1);
    TEST(graph.smallest_connecting_threshold("E", "E") == 0);
    TEST(graph.smallest_connecting_threshold("Z", "Z") == -1);
    TEST(graph.smallest_connecting_threshold("B", "E") == -1);
    TEST(graph.smallest_connecting_threshold("E", "F") == 4);
    TEST(graph.smallest_connecting_threshold("F", "C") == -1);
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    TEST(elapsed.count() <= 1000.0); // microseconds


    Graph one("example/one.csv");
    start = std::chrono::steady_clock::now();
    TEST(one.nodes() == unordered_set<string>({"A","B"}));
    TEST(one.num_nodes() == 2);
    TEST(one.num_edges() == 1);
    TEST(one.num_neighbors("A") == 1);
    TEST(one.num_neighbors("B") == 1);
    TEST(one.num_neighbors("C") == 0);
    TEST(one.edge_weight("A","B") == 1);
    TEST(one.edge_weight("A","C") == -1);
    TEST(one.neighbors("A") == unordered_set<string>({"B"}));
    TEST(one.neighbors("B") == unordered_set<string>({"A"}));
    TEST(one.neighbors("C") == unordered_set<string>({}));
    vector<tuple<string, string, int>> one_result {{"A","B",1}};
    start = std::chrono::steady_clock::now();
    TEST(one.shortest_path_weighted("A", "B") == one_result);
    TEST(one.smallest_connecting_threshold("A", "B") == 1);
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    TEST(elapsed.count() <= 1000.0); // microseconds


    Graph graph2("example/hiv.csv");

    start = std::chrono::steady_clock::now();
    for(auto uNode: graph2.nodes()) {
        for(auto vNode: graph2.nodes()) {
            graph2.shortest_path_weighted(uNode, vNode);
        }
    }
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // pay attention to the units here |------------|

    cout << "Finished in " << elapsed.count() << "ms" << endl;

    start = std::chrono::steady_clock::now();
    for(auto uNode: graph2.nodes()) {
        for(auto vNode: graph2.nodes()) {
            graph2.smallest_connecting_threshold(uNode, vNode);
        }
    }
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // pay attention to the units here |------------|

    cout << "Finished in " << elapsed.count() << "ms" << endl;

    start = std::chrono::steady_clock::now();
    graph2.smallest_connecting_threshold("222-4ki_06-08-09_1244419200","222-b7i_12-08-11_1323302400");
    graph2.smallest_connecting_threshold("222-47u_08-30-04_1093824000","222-4jw_04-24-09_1240531200");
    graph2.smallest_connecting_threshold("222-3pu_03-28-01_985737600","222-b7i_12-08-11_1323302400");
    graph2.smallest_connecting_threshold("222-47r_07-27-04_1090886400","222-3yw_04-18-03_1050624000");
    graph2.smallest_connecting_threshold("222-4ki_06-08-09_1244419200","222-b7i_12-08-11_1323302400");
    graph2.smallest_connecting_threshold("222-47u_08-30-04_1093824000","222-4jw_04-24-09_1240531200");
    graph2.smallest_connecting_threshold("222-3pu_03-28-01_985737600","222-b7i_12-08-11_1323302400");
    graph2.smallest_connecting_threshold("222-47r_07-27-04_1090886400","222-3yw_04-18-03_1050624000");
    graph2.smallest_connecting_threshold("222-4ki_06-08-09_1244419200","222-b7i_12-08-11_1323302400");
    graph2.smallest_connecting_threshold("222-47u_08-30-04_1093824000","222-4jw_04-24-09_1240531200");
    graph2.smallest_connecting_threshold("222-3pu_03-28-01_985737600","222-b7i_12-08-11_1323302400");
    graph2.smallest_connecting_threshold("222-47r_07-27-04_1090886400","222-3yw_04-18-03_1050624000");
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // pay attention to the units here |------------|

    cout << "Finished in " << elapsed.count() << "ms" << endl;

}

