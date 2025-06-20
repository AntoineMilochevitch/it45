# 🧭 Problème du Voyageur de Commerce (TSP) - Projet IT45

L’objectif de ce projet est de se confronter à un problème d’optimisation combinatoire, le problème du voyageur de commerce (TSP). Ce problème consiste à déterminer le plus court circuit permettant de visiter une fois chaque ville d’un ensemble de villes donné, et de revenir à la ville de départ à la fin.

Ce problème a une complexité exponentielle : il est facile de déterminer le plus court chemin avec une poignée de villes, mais dès que l’on commence à en avoir plusieurs dizaines, voire centaines, cela devient bien plus compliqué.

Dans le cadre de ce projet, nous abordons trois approches distinctes pour résoudre ce problème :
- 🔄 **Une métaheuristique de recherche locale : la recherche taboue**
- 🧮 **Une méthode exacte : l’algorithme de Little**
- 🧬 **Une métaheuristique évolutive : l’algorithme génétique**

Par le biais de ces algorithmes, nous obtiendrons des solutions que nous pourrons analyser et comparer pour évaluer l’efficacité de ces algorithmes selon différents critères et paramètres (nombre d’itérations, durée de calcul, éloignement de la solution trouvée par rapport à la solution optimale, etc.).

Plusieurs ensembles de villes sont utilisés, pour obtenir des résultats selon différentes tailles d’ensembles, allant de 52 villes (berlin52) à 280 villes (a280).

---

## 🗂️ Structure du projet

- `src/` : Contient le code source C++ des trois algorithmes.
- `data/` : Contient les fichiers de données (instances TSP et matrices de distances).
- `bin/` : Contient les exécutables compilés.
- `results/` : Résultats et logs d’exécution.
- `*.py` : Scripts Python pour l’analyse et la visualisation des résultats.

---

## ⚙️ Compilation

Assurez-vous d’avoir `make` et un compilateur C++ (g++) installés.

Pour compiler tous les algorithmes :
```sh
make
```

Pour compiler un algorithme spécifique :
```sh
make -C src/algo_genetique
make -C src/algo_little
make -C src/algo_recherche_tabou
```

Les exécutables seront générés dans le dossier `bin/`.

---

## 🚀 Exécution

Lancez les exécutables depuis le dossier racine du projet :

- **Algorithme génétique :**
  ```sh
  ./bin/genetique
  ```

- **Algorithme de Little :**
  ```sh
  ./bin/little
  ```

- **Recherche taboue :**
  ```sh
  ./bin/tabou
  ```

Suivez les instructions affichées pour choisir les paramètres et les instances à traiter.

---

## 📊 Analyse des résultats

Les scripts Python (`genetique_report.py`, `little_report.py`, `tabou_report.py`) permettent de visualiser et comparer les résultats. Installez les dépendances nécessaires avec :

```sh
pip install -r requirements.txt
```

Puis lancez les scripts pour générer les graphiques :

```sh
python genetique_report.py
python little_report.py
python tabou_report.py
```

---

## 👥 Auteurs

- **Alexis CORREARD** : [GitHub](https://github.com/AlexiCor)
- **Antoine MILOCHEVITCH** : [GitHub](https://github.com/AntoineMilochevitch)