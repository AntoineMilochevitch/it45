#include "rechercheTabou.h"

using namespace std;

// Initialisation des param�tres de la RechercheTabou
// et g�n�ration de la solution initiale.
// Initialisation  de la liste tabou
rechercheTabou::rechercheTabou(int nbiter,int dt,int nv, char* nom_fichier, int type_vois)
{
    nbiterations    = nbiter;
    iter_courante   = 0;
    duree_tabou     = dt;
    taille_solution = nv;
    type_voisinage  = type_vois;
    constuction_distance(taille_solution, nom_fichier);
    possede_distances = true;
    courant         = new solution(nv);
    courant->evaluer(les_distances);

    list_tabou = new int*[nv];
    for(int i=0; i<nv; i++)
    {
        list_tabou[i] = new int[nv];
        for(int j=0; j<nv; j++)
            list_tabou[i][j] = -1;
    }

    cout << "La solution initiale aleatoire est   : ";
    courant->afficher();

    list_tabou2 = new int*[duree_tabou];
    for(int i=0; i<duree_tabou; i++)
    {
        list_tabou2[i] = new int[taille_solution];
        for(int j=0; j<taille_solution; j++)
            list_tabou2[i][j] = -1;
    }
}

rechercheTabou::~rechercheTabou()
{
    delete courant;
    for(int i=0; i<taille_solution; i++)
    {
        delete[] list_tabou[i];
        if (possede_distances)
            delete[] les_distances[i];
    }
    for(int i=0; i<duree_tabou; i++)
        delete[] list_tabou2[i];
    delete[] list_tabou;
    delete[] list_tabou2;
    if (possede_distances)
        delete[] les_distances;
}


void rechercheTabou::constuction_distance(int nv, char* nom_fichier)
{
    std::string fichier(nom_fichier);
    if (fichier.find(".tsp") != std::string::npos) {
        // Lire les coordonnées des villes
        std::vector<Ville> villes = lireCoordonnees(fichier);
        if (villes.empty()) {
            std::cerr << "Erreur : Impossible de lire les coordonnées pour " << fichier << std::endl;
            exit(-1);
        }

        // Générer la matrice des distances
        std::vector<std::vector<double>> distances = genererMatriceDistances(villes);

        // Allouer la matrice des distances
        les_distances = new int*[nv];
        for (int i = 0; i < nv; i++) {
            les_distances[i] = new int[nv];
            for (int j = 0; j < nv; j++) {
                les_distances[i][j] = static_cast<int>(std::round(distances[i][j]));
            }
        }
        // Mettre la diagonale à -1 (ou -10 si tu préfères)
        for (int i = 0; i < nv; i++) {
            les_distances[i][i] = -1;
        }
    } else {
        les_distances = new int*[nv];
        for (int i = 0; i < nv; i++)
            les_distances[i] = new int[nv];

        std::ifstream fichier;
        fichier.open(nom_fichier, std::ifstream::in);
        if (!fichier.is_open()) {
            std::cerr << "Fichier [" << nom_fichier << "] invalide." << std::endl;
            exit(-1);
        }

        for (int i = 0; i < nv; i++) {
            for (int j = 0; j < nv; j++) {
                fichier >> les_distances[i][j];
            }
        }

        for (int i = 0; i < nv; i++)
            les_distances[i][i] = -1;
        fichier.close();
    }

    // Affichage de la matrice (optionnel)
    /*for (int i = 0; i < nv; i++) {
        for (int j = 0; j < nv; j++) {
            std::cout << les_distances[i][j];
            if (j < nv - 1)
                std::cout << " | ";
        }
        std::cout << std::endl;
        if (i < nv - 1) {
            for (int k = 0; k < nv; k++) {
                std::cout << "---";
            }
            std::cout << std::endl;
        }
    }*/

    /*if (fichier.find("kroA100.tsp") != std::string::npos) {
    std::cout << "Aperçu matrice distances kroA100:" << std::endl;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            std::cout << les_distances[i][j] << " ";
        }
        std::cout << std::endl;
    }
}*/
}

bool rechercheTabou::nonTabou(int i, int j)
{
    if(list_tabou[i][j]<iter_courante)
        return true;
    else
        return false;
}

bool rechercheTabou::nonTabou2(solution* sol)
{
    for(int i=0; i<duree_tabou; i++)
    {
        for(int j=1; j<taille_solution; j++)
        {
            if (list_tabou2[i][j]!=sol->ville[j])
                j = taille_solution;
            else if (j == taille_solution-1)
                return false;
        }
    }
    for(int i=0; i<duree_tabou; i++)
    {
        for(int j=1; j<taille_solution; j++)
        {
            if (list_tabou2[i][j]!=sol->ville[taille_solution-j])
                j = taille_solution;
            else if (j == taille_solution-1)
                return false;
        }
    }
    return true;
}

void rechercheTabou::mise_a_jour_liste_tabou_2(solution* sol, int&position)
{
    if (duree_tabou != 0)
    {
        for(int j=0; j<taille_solution; j++)
            list_tabou2[position][j] = sol->ville[j];
        position++;
        if (position == duree_tabou)
            position = 0;
    }
}

/**
 * Voisinage swap
 */
void rechercheTabou::voisinage_swap(int &best_i, int &best_j)
{
    int best_vois;
    //bool tous_tabou = true;
    best_vois = 100000;

    // on s�l�ctionne une premi�re ville pour le mouvement
    for(int i=0;i<taille_solution;i++)
    {
        // on s�l�ctionne une seconde ville pour le mouvement
        for(int j=i+1;j<taille_solution;j++)
        {
            if ( ((i!=0)||(j!=taille_solution-1)) && ((i!=0)||(j!=taille_solution-2)) )
            {
                // on transforme la solution courante vers le voisin
                //    gr�ce au mouvement d�finit par le couple de ville
                courant->swap(i,j);
                // on estime ce voisin
                courant->evaluer(les_distances);
                // si ce mouvement est non tabou et
                // si ce voisin a la meilleure fitness
                // alors ce voisin devient le meilleur voisin non tabou
                if(nonTabou(i,j) && courant->fitness<best_vois)
                    //if(nonTabou2(courant) && courant->fitness<best_vois)
                {
                    best_vois  = courant->fitness;
                    best_i     = i;
                    best_j     = j;
                    //tous_tabou = false;
                }
                // on re-transforme ce voisin en la solution courante
                courant->swap(i,j);
                // on r�-�value la solution courante
                courant->evaluer(les_distances);
            }
        }
    }
}


void rechercheTabou::voisinage_2opt(int &best_i, int &best_j)
{
    int best_vois = 100000;
    for (int i = 1; i < taille_solution - 1; i++) {
        for (int j = i + 1; j < taille_solution; j++) {
            // Appliquer 2-opt entre i et j
            courant->reverse(i, j);
            courant->evaluer(les_distances);
            if (nonTabou(i, j) && courant->fitness < best_vois) {
                best_vois = courant->fitness;
                best_i = i;
                best_j = j;
            }
            // Annuler le 2-opt
            courant->reverse(i, j);
            courant->evaluer(les_distances);
        }
    }
}

//proc�dure principale de la recherche
solution* rechercheTabou::optimiser(int nb_iter)
{
    // On conserve la solution courante et la liste tabou entre les appels
    static solution* best_solution = nullptr;
    if (best_solution == nullptr) {
        best_solution = new solution(taille_solution);
        *best_solution = *courant;
    }

    int best_eval = best_solution->fitness;

    for (int k = 0; k < nb_iter; ++k, ++iter_courante) {
        int best_i = 0, best_j = 0;
        if (type_voisinage == 1)
            voisinage_swap(best_i, best_j);
        else
            voisinage_2opt(best_i, best_j);

        if (type_voisinage == 1)
            courant->swap(best_i, best_j);
        else
            courant->reverse(best_i, best_j);

        courant->ordonner();
        courant->evaluer(les_distances);

        if (courant->fitness < best_eval) {
            best_eval = courant->fitness;
            *best_solution = *courant;
            best_solution->evaluer(les_distances);
        }

        list_tabou[best_i][best_j] = iter_courante + duree_tabou;
    }
    return new solution(*best_solution); // retourne une copie
}
