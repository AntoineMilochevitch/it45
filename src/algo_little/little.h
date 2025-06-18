#ifndef LITTLE_H
#define LITTLE_H

#include <vector>
#include <array>
#include <string>
#include <utility>
#include <limits>
#include <tuple>
#include <chrono>

constexpr double INF = std::numeric_limits<double>::infinity();

using Matrix = std::vector<std::vector<double>>;

class TSP {
public:
    bool readCoordinates(const std::string& filename);
    Matrix buildMatrix() const;
    void reduceMatrix(Matrix& m, double& lower_bound);
    void littleAlgorithm(Matrix& m, double current_bound, int level, std::vector<int>& path, std::chrono::steady_clock::time_point start_time, int time_limit_seconds);
    void buildSolution();
    int getCost() const;
    std::vector<int> getPath() const;
    std::pair<int, std::vector<int>> solve(const std::string& filename);

private:
    std::vector<std::tuple<int, double, double>> coordinates;
    int size = 0;
    std::vector<int> next_town;
    int cost = 0;
    double best_bound_global = INF;
    std::vector<int> current_path; 
    std::vector<int> best_solution_path;
    Matrix original_matrix;
};

#endif // LITTLE_H
