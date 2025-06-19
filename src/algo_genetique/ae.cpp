#include "ae.h"
#include "utils.h"

using namespace std;

// initialisation des param�tres de l'AG et g�n�ration de la population initiale
Ae::Ae(int nbg, int tp, double tcroisement, double tmutation, int tc, char* nom_fichier,
       CroisementType croisement_type, bool utiliser_2opt, int mode_arret, int duree_seconde, MutationType mutation_type)
{
    nbgenerations     = nbg;
    taille_pop        = tp;
    taux_croisement   = tcroisement;
    taux_mutation     = tmutation;
    taille_chromosome = tc;
    this->croisement_type = croisement_type;
    this->utiliser_2opt = utiliser_2opt;
    this->mode_arret = mode_arret;
    this->duree_seconde = duree_seconde;
    this->mutation_type = mutation_type;
    constuction_distance(nom_fichier);
    pop   = new population(taille_pop, taille_chromosome);
}

// destructeur de l'objet Ae
Ae::~Ae()
{
    if (les_distances) {
        for (int i = 0; i < taille_chromosome; i++) {
            delete[] les_distances[i];
        }
        delete[] les_distances;
    }
    delete pop;
}

// proc�dure principale de la recherche
chromosome* Ae::optimiser(int& nb_generations_effectuees)
{
	int amelioration = 0;
	chromosome *fils1 = new chromosome(taille_chromosome);
	chromosome *fils2 = new chromosome(taille_chromosome);
	chromosome *pere1;
	chromosome *pere2;
	int best_fitness;

	// �valuation des individus de la population initiale
	for(int ind=0; ind<taille_pop; ind++)
		pop->individus[ind]->evaluer(les_distances);

	// on ordonne les indivudus selon leur fitness
	pop->ordonner();

	best_fitness = pop->individus[pop->ordre[0]]->fitness;

	auto start = std::chrono::high_resolution_clock::now();
    int g = 0;
    while (true) {
        // Arrêt selon le mode
        if (mode_arret == 1 && g >= nbgenerations) break;
        if (mode_arret == 2) {
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = now - start;
            if (elapsed.count() >= duree_seconde) break;
        }
        pere1 = pop->selection_roulette();
        pere2 = pop->selection_roulette();
        if(Random::aleatoire(1000)/1000.0 < taux_croisement)
        {
            if (croisement_type == CROISEMENT_1X)
                croisement1X(pere1, pere2, fils1, fils2);
            else if (croisement_type == CROISEMENT_2X)
                croisement2X(pere1, pere2, fils1, fils2);
            else
                croisementLOX(pere1, pere2, fils1, fils2);
        }
        else
        {
            fils1->copier(pere1);
            fils2->copier(pere2);
        }
        if (Random::aleatoire(1000) / 1000.0 < taux_mutation) {
            if (mutation_type == MUTATION_SWAP)
                fils1->echange_2_genes_consecutifs();
            else if (mutation_type == MUTATION_OROPT)
                fils1->or_opt();
            else if (mutation_type == MUTATION_TABOU)
                appliquer_mutation_tabou(fils1, les_distances);
            if (utiliser_2opt)
                fils1->ameliorer_2opt(les_distances);
        }
        if (Random::aleatoire(1000) / 1000.0 < taux_mutation) {
            if (mutation_type == MUTATION_SWAP)
                fils2->echange_2_genes_consecutifs();
            else if (mutation_type == MUTATION_OROPT)
                fils2->or_opt();
            else if (mutation_type == MUTATION_TABOU)
                appliquer_mutation_tabou(fils2, les_distances);
            if (utiliser_2opt)
                fils2->ameliorer_2opt(les_distances);
        }
        fils1->evaluer(les_distances);
        fils2->evaluer(les_distances);
        pop->remplacement_roulette(fils1);
		pop->remplacement_roulette(fils2);
        pop->reordonner();
        if (pop->individus[pop->ordre[0]]->fitness < best_fitness)
        {
            best_fitness = pop->individus[pop->ordre[0]]->fitness;
            amelioration = g;
        }
        g++;
    }
    nb_generations_effectuees = g;
    return pop->individus[pop->ordre[0]];
}

// op�rateur de croisement � un point : croisement 1X
// 1) l'op�rateur 1X choisit de mani�re al�atoire un point de croisement
// 2) l'op�rateur 1X recopie le d�but du parent 1 au d�but de l'enfant 1
//                     et le d�but du parent 2 au d�but de l'enfant 2.
// 3) l'op�rateur 1X compl�te l'enfant 1 avec les g�nes manquant en les pla�ant dans l'ordre du parent 2
//                         et l'enfant 2 avec les g�nes manquant en les pla�ant dans l'ordre du parent 1.
//    Le 1ier fils est le produit de la partie haute du premier parent et
//    de la partie basse du deuxi�me parent et inversement pour le 2�me fils
void Ae::croisement1X(chromosome* parent1, chromosome* parent2,
                      chromosome* enfant1, chromosome* enfant2)
{
	int nb_genes = parent1->taille;

	int* odre_parent1 = new int[nb_genes];
	int* odre_parent2 = new int[nb_genes];

	for (int i=0; i<nb_genes; i++)
	{
		odre_parent1[parent1->genes[i]] = i;
		odre_parent2[parent2->genes[i]] = i;
	}

	// 1) l'op�rateur 1X choisit de mani�re al�atoire le point de croisement
	int point = Random::aleatoire(nb_genes);

	// 2) l'op�rateur 1X recopie le d�but du parent 1 au d�but de l'enfant 1
	//                     et le d�but du parent 2 au d�but de l'enfant 2.
	enfant1->copier(parent1);
	enfant2->copier(parent2);

	// 3) l'op�rateur 1X compl�te l'enfant 1 avec les g�nes manquant en les pla�ant dans l'ordre du parent 2
	//                         et l'enfant 2 avec les g�nes manquant en les pla�ant dans l'ordre du parent 1.
	for (int k=point+1; k<nb_genes; k++)
	{
		for (int l=k+1; l<nb_genes; l++)
		{
			if(odre_parent2[enfant1->genes[k]]>odre_parent2[enfant1->genes[l]])
				enfant1->echange_2_genes(k,l);
			if(odre_parent1[enfant2->genes[k]]>odre_parent1[enfant2->genes[l]])
				enfant2->echange_2_genes(k,l);
		}
	}
	delete[] odre_parent1;
	delete[] odre_parent2;
}

// op�rateur de croisement � deux points : croisement 2X
// 1) l'op�rateur 2X choisit de mani�re al�atoire 2 points de croisement
// 2) l'op�rateur 2X recopie le d�but du parent 1 au d�but de l'enfant 1
//                        et le d�but du parent 2 au d�but de l'enfant 2.
// 3) l'op�rateur 2X compl�te l'enfant 1 avec les g�nes manquant en les pla�ant dans l'ordre du parent 2
//                         et l'enfant 2 avec les g�nes manquant en les pla�ant dans l'ordre du parent 1.
void Ae::croisement2X(chromosome* parent1, chromosome* parent2,
	chromosome* enfant1, chromosome* enfant2)
{
	int nb_genes = parent1->taille;

	// Choisir deux points de croisement aléatoires
	int point1 = Random::aleatoire(nb_genes);
	int point2 = Random::aleatoire(nb_genes);

	// S'assurer que point1 < point2
	if (point1 > point2) {
		std::swap(point1, point2);
	}

	// Copier les segments impairs du parent1 dans enfant1
	// et les segments impairs du parent2 dans enfant2
	for (int i = 0; i < nb_genes; i++) {
		if (i <= point1 || i > point2) {
			enfant1->genes[i] = parent1->genes[i];
			enfant2->genes[i] = parent2->genes[i];
		} else {
			enfant1->genes[i] = -1; // Marqueur temporaire
			enfant2->genes[i] = -1; // Marqueur temporaire
		}
	}

	// Compléter les enfants avec les gènes manquants dans l'ordre du parent opposé
	int index1 = (point2 + 1) % nb_genes;
	int index2 = (point2 + 1) % nb_genes;

	for (int i = 0; i < nb_genes; i++) {
		int gene_p2 = parent2->genes[(point2 + 1 + i) % nb_genes];
		int gene_p1 = parent1->genes[(point2 + 1 + i) % nb_genes];

		// Compléter enfant1 avec les gènes de parent2
		if (std::find(enfant1->genes, enfant1->genes + nb_genes, gene_p2) == enfant1->genes + nb_genes) {
			// Trouver la prochaine case libre (-1)
			while (enfant1->genes[index1] != -1)
				index1 = (index1 + 1) % nb_genes;
			enfant1->genes[index1] = gene_p2;
			index1 = (index1 + 1) % nb_genes;
		}

		// Compléter enfant2 avec les gènes de parent1
		if (std::find(enfant2->genes, enfant2->genes + nb_genes, gene_p1) == enfant2->genes + nb_genes) {
			// Trouver la prochaine case libre (-1)
			while (enfant2->genes[index2] != -1)
				index2 = (index2 + 1) % nb_genes;
			enfant2->genes[index2] = gene_p1;
			index2 = (index2 + 1) % nb_genes;
		}
	}
}

// Opérateur de croisement LOX (Linear Order Crossover)
// 1) On choisit aléatoirement deux points de croisement point1 et point2 (point1 < point2)
// 2) On copie le segment [point1, point2] du parent1 dans enfant1 et du parent2 dans enfant2
// 3) On complète chaque enfant avec les gènes manquants dans l'ordre du parent opposé, en commençant après point2 (avec retour circulaire)

void Ae::croisementLOX(chromosome* parent1, chromosome* parent2,
                       chromosome* enfant1, chromosome* enfant2)
{
    int nb_genes = parent1->taille;

    // 1) Choisir deux points de croisement aléatoires
    int point1 = Random::aleatoire(nb_genes);
    int point2 = Random::aleatoire(nb_genes);

    // S'assurer que point1 < point2
    if (point1 > point2) {
        std::swap(point1, point2);
    }

    // 2) Initialiser tous les gènes à -1
    for (int i = 0; i < nb_genes; i++) {
        enfant1->genes[i] = -1;
        enfant2->genes[i] = -1;
    }

    // Copier le segment [point1, point2] du parent1 dans enfant1 et du parent2 dans enfant2
    for (int i = point1; i <= point2; i++) {
        enfant1->genes[i] = parent1->genes[i];
        enfant2->genes[i] = parent2->genes[i];
    }

    // 3) Compléter enfant1 avec les gènes de parent2 non déjà présents dans le segment copié
    int index1 = (point2 + 1) % nb_genes;
    for (int i = 0; i < nb_genes; i++) {
        int gene = parent2->genes[(point2 + 1 + i) % nb_genes];
        // Vérifier si le gène est déjà dans le segment copié
        bool deja_present = false;
        for (int j = point1; j <= point2; j++) {
            if (enfant1->genes[j] == gene) {
                deja_present = true;
                break;
            }
        }
        if (!deja_present) {
            enfant1->genes[index1] = gene;
            index1 = (index1 + 1) % nb_genes;
        }
    }

    // Compléter enfant2 avec les gènes de parent1 non déjà présents dans le segment copié
    int index2 = (point2 + 1) % nb_genes;
    for (int i = 0; i < nb_genes; i++) {
        int gene = parent1->genes[(point2 + 1 + i) % nb_genes];
        bool deja_present = false;
        for (int j = point1; j <= point2; j++) {
            if (enfant2->genes[j] == gene) {
                deja_present = true;
                break;
            }
        }
        if (!deja_present) {
            enfant2->genes[index2] = gene;
            index2 = (index2 + 1) % nb_genes;
        }
    }
}

void Ae::constuction_distance(char* nom_fichier)
{
    // Vérifier si le fichier est un fichier .tsp
    std::string fichier(nom_fichier);
    if (fichier.find(".tsp") != std::string::npos) {
        // Lire les coordonnées des villes
        std::vector<Ville> villes = lireCoordonnees(fichier);
        if (villes.empty()) {
            cerr << "Erreur : Impossible de lire les coordonnées pour " << fichier << endl;
            exit(-1);
        }

        // Générer la matrice des distances
        std::vector<std::vector<double>> distances = genererMatriceDistances(villes);

        // Allouer la matrice des distances
        les_distances = new int*[taille_chromosome];
        for (int i = 0; i < taille_chromosome; i++) {
            les_distances[i] = new int[taille_chromosome];
            for (int j = 0; j < taille_chromosome; j++) {
                les_distances[i][j] = static_cast<int>(std::round(distances[i][j]));
            }
        }
        // Correction : mettre la diagonale à -1
        for (int i = 0; i < taille_chromosome; i++) {
            les_distances[i][i] = -1;
        }
    } else {
        // Lecture classique pour les fichiers de distances
        les_distances = new int*[taille_chromosome];
        for (int i = 0; i < taille_chromosome; i++) {
            les_distances[i] = new int[taille_chromosome];
        }

        ifstream fichier;
        fichier.open(nom_fichier, ifstream::in);
        if (!fichier.is_open()) {
            cerr << "Fichier [" << nom_fichier << "] invalide." << endl;
            exit(-1);
        }

        for (int i = 0; i < taille_chromosome; i++) {
            for (int j = 0; j < taille_chromosome; j++) {
                fichier >> les_distances[i][j];
            }
        }

        for (int i = 0; i < taille_chromosome; i++) {
            les_distances[i][i] = -1;
        }

        fichier.close();
    }

	// afficher la matrice 
    for (int i = 0; i < taille_chromosome; i++) {
        for (int j = 0; j < taille_chromosome; j++) {
            cout << les_distances[i][j];
            if (j < taille_chromosome - 1)
                cout << " | ";
        }
        cout << endl;
        // Ligne de séparation entre les lignes (optionnel)
        if (i < taille_chromosome - 1) {
            for (int k = 0; k < taille_chromosome; k++) {
                cout << "---";
            }
            cout << endl;
        }
    }
}

void Ae::appliquer_mutation_tabou(chromosome* chro, int **distances) {
    // Convertir chromosome en solution
    solution sol(chro->taille);
    for (int i = 0; i < chro->taille; ++i)
        sol.ville[i] = chro->genes[i];
    sol.evaluer(distances);

    // Appliquer la recherche tabou (paramètres à ajuster selon besoin)
    rechercheTabou tabou(100, 10, chro->taille, nullptr, 1); // nbiter, duree_tabou, taille, distances, type_voisinage
    tabou.les_distances = distances; // Utiliser la même matrice de distances
    tabou.possede_distances = false; // Indiquer que les distances sont fournies
    delete tabou.courant;
    tabou.courant = new solution(sol);

    solution* best = tabou.optimiser();

    // Copier la solution tabou dans le chromosome
    for (int i = 0; i < chro->taille; ++i)
        chro->genes[i] = best->ville[i];
    chro->evaluer(distances);

    delete best;
}

