
#include "solution.h"

using namespace std;


//initialisation d'une solution : un cycle passant par toutes les villes
solution::solution(int nv):taille(nv)
{
    ville           = new int[taille];
    //  Arbitrairement, on choisit de toujours commencer par la ville 0
    ville[0]        = 0;
    for(int i=1; i<taille; i++)
    {
        bool recommence  = true;
        int a;
        while(recommence)
        {
            recommence = false;
            // on tire aléatoirement la ville suivante
            a = Random::aleatoire(taille);
            // la ville ne doit pas être déjà dans le cycle
            // si tel est le cas on recommence
            for (int j=0; j<i; j++)
                if (a==ville[j])
                    recommence = true;
        }
        ville[i] = a;
    }
    // on impose arbitrairement que ville[1] > ville[taille-1]
    ordonner();
}

// copie les villes d'une solution. la fitness n'est reprise
solution& solution::operator=(const solution& source)
{
    if (this->taille!=source.taille) {
        if (ville) delete[] ville; 
        taille = source.taille ;
        ville  = new int[taille];
    }
    for(int i=0; i<taille; i++)
        ville[i] = source.ville[i];
    return *this ;
}



solution::~solution()
{
    if (ville) delete[] ville; 
}

/**
 * operaotr==
 */
bool solution::operator==(const solution& chro)
{
    for(int i=1; i<taille; i++)
        if (chro.ville [i] != this->ville[i])
            return false;
    return true;
}

// évaluation d'une solution : c'est la somme des distances reliant les villes
void solution::evaluer(int **distance)
{
    fitness = 0;
    for(int i=0;i<taille-1;i++)
        fitness += distance[ville[i]][ville[i+1]];
    fitness+=distance[ville[0]][ville[taille-1]];
}

void solution::afficher()
{
    for(int i=0;i<taille;i++)
        cout << ville[i] << "-";
    cout << "--> " << fitness << " km" << endl;
}

// on impose arbitrairement que la 2ième ville visitée (ville[1])
//   ait un n° plus petit que la dernière ville visitée (ville[taille-1])
//   i.e. : ville[1] > ville[taille-1]
void solution::ordonner()
{
    int inter, k;
    // Place la ville "0" en tête de la solution (ville[0])
    if (ville[0] != 0)
    {
        int position_0 = 0;
        int * ville_c  = new int[taille];
        for (int i=0; i<taille; i++)
        {
            ville_c[i] = ville[i];
            if (ville[i]==0)
                position_0 = i;
        }
        k = 0;
        for (int i=position_0; i<taille; i++)
        {
            ville[k] = ville_c[i];
            k++;
        }
        for (int i=0; i<position_0; i++)
        {
            ville[k] = ville_c[i];
            k++;
        }
        delete[] ville_c;
    }

    // Le numéro de la 2eme ville doit être plus petit que celui de la dernière ville
    if (ville[1] > ville[taille-1])
    {
        for(int k=1; k<=1+(taille-2)/2; k++)
        {
            inter = ville[k];
            ville[k] = ville[taille-k];
            ville[taille-k] = inter;
        }
    }
}

// on échange 2 villes dans la solution
void solution::swap(int ville1, int ville2)
{
    int inter    = ville[ville1];
    ville[ville1] = ville[ville2];
    ville[ville2] = inter;
}



