#include "little.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <chrono>

const int TIME_LIMIT_SECONDS = 60; //60, 600, 3600

bool TSP::readCoordinates(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find("DIMENSION");
        if (pos != std::string::npos) {
            size_t colon = line.find(':', pos);
            if (colon != std::string::npos) {
                std::istringstream iss(line.substr(colon + 1));
                iss >> size;
            }
        }

        pos = line.find("NODE_COORD_SECTION");
        if (pos != std::string::npos) break;
    }

    coordinates.resize(size);
    for (int i = 0; i < size; ++i) {
        int id;
        double x, y;
        file >> id >> x >> y;
        coordinates[i] = std::make_tuple(id, x, y);
    }

    return true;
}

Matrix TSP::buildMatrix() const {
    Matrix m(size, std::vector<double>(size, INF));

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i == j) continue;
            double dx = std::get<1>(coordinates[i]) - std::get<1>(coordinates[j]);
            double dy = std::get<2>(coordinates[i]) - std::get<2>(coordinates[j]);
            m[i][j] = std::round(std::sqrt(dx * dx + dy * dy));
        }
    }
    return m;
}

void TSP::reduceMatrix(Matrix& m, double& lower_bound) {
    // Réduction des lignes
    for (int i = 0; i < size; ++i) {
        double row_min = INF;
        for (int j = 0; j < size; ++j) {
            if (m[i][j] < row_min)
                row_min = m[i][j];
        }
        if (row_min != INF && row_min > 0) {
            for (int j = 0; j < size; ++j) {
                if (m[i][j] != INF)
                    m[i][j] -= row_min;
            }
            lower_bound += row_min;
        }
    }

    // Réduction des colonnes
    for (int j = 0; j < size; ++j) {
        double col_min = INF;
        for (int i = 0; i < size; ++i) {
            if (m[i][j] < col_min)
                col_min = m[i][j];
        }
        if (col_min != INF && col_min > 0) {
            for (int i = 0; i < size; ++i) {
                if (m[i][j] != INF)
                    m[i][j] -= col_min;
            }
            lower_bound += col_min;
        }
    }
}

void TSP::littleAlgorithm(Matrix& m, double current_bound, int level, std::vector<int>& path, std::chrono::steady_clock::time_point start_time, int time_limit_seconds) {
    auto now = std::chrono::steady_clock::now();
    double elapsed_sec = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();
    if (elapsed_sec > time_limit_seconds) return;
    
    double bound_after_reduction = current_bound;
    reduceMatrix(m, bound_after_reduction);

    if (bound_after_reduction >= best_bound_global) return;

     if (level == size) {
        int last = path.back();
        double retour = original_matrix[last][path[0]];
        if (retour == INF) return;

        double total_cost = bound_after_reduction + retour;
        if (total_cost < best_bound_global) {
            best_bound_global = total_cost;
            best_solution_path = path;
            best_solution_path.push_back(path[0]); // fermer le cycle
            std::cout << "Nouvelle meilleure solution trouvée (coût = " << total_cost << ")\n";
        }
        return;
    }

    int i0 = -1, j0 = -1;
    double max_penalty = -1;

    // Trouver la position (i0,j0) avec la plus grande pénalité
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (m[i][j] == 0) {
                double row_min = INF;
                for (int k = 0; k < size; ++k)
                    if (k != j && m[i][k] < row_min) row_min = m[i][k];

                double col_min = INF;
                for (int k = 0; k < size; ++k)
                    if (k != i && m[k][j] < col_min) col_min = m[k][j];

                double penalty = (row_min == INF ? 0 : row_min) + (col_min == INF ? 0 : col_min);

                if (penalty > max_penalty) {
                    max_penalty = penalty;
                    i0 = i;
                    j0 = j;
                }
            }
        }
    }

    if (i0 == -1 || j0 == -1) return; // Pas d'arc à sélectionner

    {
        Matrix m_incl = m;
        double bound_incl = bound_after_reduction;

        // Invalidations ligne i0 et colonne j0
        for (int k = 0; k < size; ++k) {
            m_incl[i0][k] = INF;
            m_incl[k][j0] = INF;
        }
        // Interdire arc inverse (j0 -> i0) pour éviter cycle prématuré
        m_incl[j0][i0] = INF;

        std::vector<int> path_incl = path;

        std::vector<int> temp_path = path;
        temp_path.push_back(j0);

        std::vector<bool> visited(size, false);
        int current = temp_path[0];
        int count = 0;
        while (true) {
            visited[current] = true;
            ++count;
            auto it = std::find(temp_path.begin(), temp_path.end(), current);
            if (it == temp_path.end() || it + 1 == temp_path.end()) break;
            current = *(it + 1);
            if (visited[current]) break; // Détecte un cycle
        }

        // Si on a formé un sous-cycle avant d’avoir visité toutes les villes
        if (count < size && current == temp_path[0]) {
            // Ne pas inclure (i0 -> j0) car cela créerait un sous-cycle
        } else {
            path_incl.push_back(j0);
            littleAlgorithm(m_incl, bound_incl, level + 1, path_incl, start_time, time_limit_seconds);
        }
    }

    // Exclusion de l'arc (i0 -> j0)
    {
        Matrix m_excl = m;
        m_excl[i0][j0] = INF;
        littleAlgorithm(m_excl, bound_after_reduction, level, path, start_time, time_limit_seconds);
    }
}


void TSP::buildSolution() {
    cost = 0;
    int current = 0;
    for (int i = 0; i < size - 1; ++i) {
        int next = next_town[current];
        double dx = std::get<1>(coordinates[next]) - std::get<1>(coordinates[current]);
        double dy = std::get<2>(coordinates[next]) - std::get<2>(coordinates[current]);
        cost += static_cast<int>(std::round(std::sqrt(dx * dx + dy * dy)));
        current = next;
    }
    double dx = std::get<1>(coordinates[0]) - std::get<1>(coordinates[current]);
    double dy = std::get<2>(coordinates[0]) - std::get<2>(coordinates[current]);
    cost += static_cast<int>(std::round(std::sqrt(dx * dx + dy * dy)));
}

int TSP::getCost() const {
    return cost;
}

std::vector<int> TSP::getPath() const {
    std::vector<int> path;
    if (size == 0) return path;

    int current = 0;
    path.push_back(current);
    for (int i = 1; i < size; ++i) {
        current = next_town[current];
        path.push_back(current);
    }
    return path;
}

std::pair<int, std::vector<int>> TSP::solve(const std::string& filename) {
    std::cout << "Début de solve() pour : " << filename << std::endl;

    if (!readCoordinates(filename)) {
        std::cerr << "Erreur lors de la lecture du fichier : " << filename << std::endl;
        return {-1, {}};
    }

    original_matrix = buildMatrix();
    std::cout << "Matrice de distance construite." << std::endl;
    
    const double INF = 1e9;
    best_bound_global = INF;
    best_solution_path.clear();

    std::vector<int> path = {0}; // commencer à la ville 0
    auto start_time = std::chrono::steady_clock::now();

    littleAlgorithm(original_matrix, 0.0, 1, path, start_time, TIME_LIMIT_SECONDS);
    std::cout << "Fin de l'algorithme de Little." << std::endl;

    if (best_solution_path.empty()) {
        std::cerr << "Aucune solution trouvée (chemin vide)." << std::endl;
        return {-1, {}};
    }

     next_town.resize(size);
    for (size_t i = 0; i < best_solution_path.size() - 1; ++i) {
        int from = best_solution_path[i];
        int to = best_solution_path[i + 1];
        next_town[from] = to;
    }

    // Calcul du coût
    cost = 0;
    for (size_t i = 0; i < best_solution_path.size() - 1; ++i) {
        int from = best_solution_path[i];
        int to = best_solution_path[i+1];
        double dx = std::get<1>(coordinates[to]) - std::get<1>(coordinates[from]);
        double dy = std::get<2>(coordinates[to]) - std::get<2>(coordinates[from]);
        cost += static_cast<int>(std::round(std::sqrt(dx * dx + dy * dy))); 
    }

    std::cout << "Solution trouvée de coût : " << cost << std::endl;

    return {cost, best_solution_path};
}

