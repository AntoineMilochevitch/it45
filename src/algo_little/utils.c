#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils.h"

int lireCoordonneesFichier(const char* fichier, Ville* villes, int max_villes, int* nb_villes) {
    FILE* file = fopen(fichier, "r");
    if (!file) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", fichier);
        return 0;
    }

    char ligne[256];
    int section_coord = 0;
    int index = 0;
    *nb_villes = 0;

    while (fgets(ligne, sizeof(ligne), file)) {
        // Supprimer les espaces au début de la ligne
        char* trimmed = ligne;
        while (*trimmed == ' ') trimmed++;

        if (strncmp(trimmed, "DIMENSION", 9) == 0) {
            char* ptr = strstr(trimmed, ":");
            if (ptr) {
                *nb_villes = atoi(ptr + 1);
                if (*nb_villes > max_villes) {
                    fprintf(stderr, "Erreur : Trop de villes (max: %d)\n", max_villes);
                    fclose(file);
                    return 0;
                }
            }
        }

        if (strncmp(trimmed, "NODE_COORD_SECTION", 17) == 0) {
            section_coord = 1;
            continue;
        }

        if (strncmp(trimmed, "EOF", 3) == 0) {
            break;
        }

        if (section_coord && index < max_villes) {
            Ville ville;
            if (sscanf(trimmed, "%d %lf %lf", &ville.id, &ville.x, &ville.y) == 3) {
                villes[index] = ville;
                index++;
            }
        }
    }

    fclose(file);
    return (index == *nb_villes);
}

void genererMatriceDistances(const Ville* villes, int nb_villes, double distances[280][280]) {
    for (int i = 0; i < nb_villes; i++) {
        for (int j = 0; j < nb_villes; j++) {
            if (i != j) {
                double dx = villes[i].x - villes[j].x;
                double dy = villes[i].y - villes[j].y;
                distances[i][j] = round(sqrt(dx * dx + dy * dy));  // Ajout de round pour correspondre aux solutions optimales
            } else {
                distances[i][j] = -1; // Distance à soi-même
            }
        }
    }
}

