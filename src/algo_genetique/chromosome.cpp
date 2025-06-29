#include "chromosome.h"
#include <algorithm>

using namespace std;

// initialisation des param�tres d'un chromosome
chromosome::chromosome(int tc)
{
	int a;
	bool recommence = true;
	taille          = tc;
	// un chromosome est compos� de 'taille' g�nes,
	// les g�nes sont carat�ris� par un entier compris entre 0 et 'taille-1'
	// il ne peux avoir 2 fois le m�me g�ne dans un chromosome
	genes = new int[taille];
	//  Arbitrairement, on choisit de toujours commencer un chromosome par le g�ne 0
	genes[0] = 0;
	for(int i=1; i<taille; i++)
	{
		recommence = true;
		while(recommence)
		{
			recommence = false;
			// on tire al�atoirement le g�ne suivante
			a = Random::aleatoire(taille);
			// le g�ne ne doit pas �tre d�j� dans le chromosome
			// si tel est le cas on re-tire al�atoirement le g�ne
			for (int j=0; j<i; j++)
				if (a==genes[j])
					recommence = true;
		}
		genes[i]=a;
	}
	// on impose arbitrairement que gene[1] > gene[taille-1]
	ordonner();
}

// destruction de l'objet 'chromosome'
chromosome::~chromosome()
{
	delete genes;
}

// on impose arbitrairement que la 2i�me ville visit�e (gene[1])
//   ait un n� plus petit que la derni�re ville visit�e (gene[taille-1])
//   i.e. : gene[1] > gene[taille-1]
void chromosome::ordonner()
{
	int inter, k;
	// Place la ville "0" en t�te du chromosome (genes[0])
	if (genes[0] != 0)
	{
		int position_0=0;
		int * genes_c = new int[taille];
		for (int i=0; i<taille; i++)
		{
			genes_c[i] = genes[i];
			if (genes[i] == 0)
				position_0=i;
		}
		k=0;
		for (int i=position_0; i<taille; i++)
		{
			genes[k] = genes_c[i];
			k++;
		}
		for (int i=0; i<position_0; i++)
		{
			genes[k] = genes_c[i];
			k++;
		}
		delete[] genes_c;
	}

	// Le num�ro de la 2eme ville doit �tre plus petit que celui de la derni�re ville
	if (genes[1] > genes[taille-1])
	{
		for(int k=1; k<=1+(taille-2)/2; k++)
		{
			inter = genes[k];
			genes[k] = genes[taille-k];
			genes[taille-k] = inter;
		}
	}
}

// �valuation d'une solution : fonction qui calcule la fitness d'une solution
void chromosome::evaluer(int **distance)
{
    fitness = 0;
    for (int i = 0; i < taille - 1; i++) {
        if (distance[genes[i]][genes[i + 1]] < 0) {
            cerr << "Erreur : Distance invalide entre " << genes[i] << " et " << genes[i + 1] << endl;
            exit(-1);
        }
        fitness += distance[genes[i]][genes[i + 1]];
    }
    if (distance[genes[0]][genes[taille - 1]] < 0) {
        cerr << "Erreur : Distance invalide entre " << genes[0] << " et " << genes[taille - 1] << endl;
        exit(-1);
    }
    fitness += distance[genes[0]][genes[taille - 1]];
}

// copie les genes d'un chromosome. la fitness n'est reprise
void chromosome::copier(chromosome* source)
{
	for(int i=0; i<taille; i++)
		genes[i] = source->genes[i];
}

// on �change les 2 g�nes
void chromosome::echange_2_genes(int gene1, int gene2)
{
	int inter    = genes[gene1];
	genes[gene1] = genes[gene2];
	genes[gene2] = inter;
}

void chromosome::echange_2_genes_consecutifs()
{
	// on s�l�ctionne un g�ne al�atoirement entre premier et l'avant dernier.
	// Rappel : Random::aleatoire(taille-1) retourne un entier al�atoire compris entre 0 et taille-2
	int i = Random::aleatoire(taille-1);

	// on �change le g�ne s�l�ctionn� al�atoirement avec le g�ne le succ�dant
	echange_2_genes(i, i+1);

	ordonner();
}

void chromosome::echange_2_genes_quelconques()
{
	// Sélectionne deux positions aléatoires différentes
    int gene1 = Random::aleatoire(taille);
    int gene2 = Random::aleatoire(taille);
    while (gene1 == gene2)
    {
        gene2 = Random::aleatoire(taille);
    }

    // Échange les deux gènes
    echange_2_genes(gene1, gene2);

    // Réordonne le chromosome si nécessaire
    ordonner();
}

void chromosome::deplacement_1_gene()
{
    // Sélectionne un gène aléatoire
    int geneIndex = Random::aleatoire(taille);

    // Sélectionne une nouvelle position aléatoire différente
    int newPosition = Random::aleatoire(taille);
    while (newPosition == geneIndex)
    {
        newPosition = Random::aleatoire(taille);
    }
}


// Mutation or-opt : déplace un gène (hors 0) à une autre position (hors 0)
void chromosome::or_opt() {
    if (taille <= 3) return; // rien à faire
    int from = Random::aleatoire(taille - 1) + 1; // [1, taille-1]
    int to = Random::aleatoire(taille - 1) + 1;
    while (to == from) to = Random::aleatoire(taille - 1) + 1;

    int gene = genes[from];
    if (from < to) {
        for (int i = from; i < to; ++i)
            genes[i] = genes[i + 1];
        genes[to] = gene;
    } else {
        for (int i = from; i > to; --i)
            genes[i] = genes[i - 1];
        genes[to] = gene;
    }
    ordonner();
}

void chromosome::afficher() {
    afficher(std::cout);
}

	// affichage des param�tre d'un chromosome
void chromosome::afficher(std::ostream& os)
{
    os << genes[0];
    for (int i = 1; i < taille; i++)
        os << "-" << genes[i];
    os << " => fitness = " << fitness << std::endl;
}

bool chromosome::identique(chromosome* chro)
{
    for(int i=1; i<taille; i++)
    	if (chro->genes [i] != this->genes[i])
    		return false;
    return true;
}

void chromosome::ameliorer_2opt(int **distance) {
    bool amelioration = true;
    while (amelioration) {
        amelioration = false;
        for (int i = 0; i < taille - 1; i++) { // Inclure le premier gène
            for (int j = i + 1; j < taille; j++) {
                // Calcul du gain si on inverse les gènes entre i+1 et j
                int gain = distance[genes[i]][genes[j]] + distance[genes[(i + 1) % taille]][genes[(j + 1) % taille]] -
                           distance[genes[i]][genes[(i + 1) % taille]] - distance[genes[j]][genes[(j + 1) % taille]];
                
                if (gain < 0) {
                    // Inverser les gènes entre i+1 et j
                    std::reverse(genes + (i + 1), genes + (j + 1));
                    amelioration = true;
                }
            }
        }
    }
    evaluer(distance); // Réévaluer la fitness après amélioration
}