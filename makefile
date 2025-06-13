# Règle par défaut
all:
	$(MAKE) -C src/algo_genetique
	$(MAKE) -C src/algo_recherche_tabou

# Nettoyage
clean:
	$(MAKE) -C src/algo_genetique clean
	$(MAKE) -C src/algo_recherche_tabou clean

# Phonies
.PHONY: all clean