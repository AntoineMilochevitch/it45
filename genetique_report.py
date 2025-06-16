import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Charger les résultats
df = pd.read_csv("results/algo_genetique_results.csv")
df["gap"] = df["gap"].astype(float)

# Affichage conditionnel selon le mode d'arrêt
if "mode_arret" in df.columns:
    for mode, group in df.groupby("mode_arret"):
        fig, ax = plt.subplots(figsize=(10, 6))
        sns.barplot(data=group, x="instance", y="fitness", ax=ax)
        ax.set_title(f"Fitness par instance (mode: {mode})")
        ax.set_ylabel("Fitness (coût de la tournée)")
        ax.set_xlabel("Instances")
        ax.xaxis.labelpad = 8 
        ax.tick_params(axis='x', pad=10) 

        # Ajouter le texte des gaps sur les barres
        for i, row in group.iterrows():
            ax.text(i, row["fitness"], f"Gap: {row['gap']:.2f}%", ha='center', va='bottom')

        # Colonnes du tableau selon le mode
        table_cols = ["instance", "fitness", "gap", "temps"]
        if mode == "temps":
            table_cols += ["nb_generations_effectuees"]
        else:
            table_cols += ["nb_generation"]

        table_data = group[table_cols]
        plt.table(
            cellText=table_data.values,
            colLabels=table_data.columns,
            cellLoc='center',
            loc='bottom',
            bbox=[0, -0.5, 1, 0.3] 
        )
        plt.subplots_adjust(left=0.2, bottom=0.35) 

        # Récupérer les paramètres (on suppose qu'ils sont identiques pour toutes les lignes du groupe)
        params = group.iloc[0]
        param_text = (
            f"Paramètres : mode_arret={params['mode_arret']}, "
            f"nb_generation={params['nb_generation']}, "
            f"taille_population={params['taille_population']}, "
            f"taux_croisement={params['taux_croisement']}, "
            f"taux_mutation={params['taux_mutation']}"
        )
        plt.figtext(0.5, 0.02, param_text, ha='center', fontsize=12, bbox={"facecolor":"#eee","pad":5,"edgecolor":"none"})

        if mode == "temps":
            mode_str = f"temps{int(params['duree_seconde'])}"
        else:
            mode_str = f"gen{params['nb_generation']}"

        filename = (
            f"results/genetique_summary_{mode}_"
            f"{mode_str}_pop{params['taille_population']}_"
            f"cross{params['taux_croisement']}_mut{params['taux_mutation']}_"
            f"{params['croisement']}_{params['2opt']}.png"
        )

        plt.savefig(filename, bbox_inches='tight')
        plt.show()
        plt.close()
else:
    # Cas où la colonne mode_arret n'existe pas (ancien format)
    fig, ax = plt.subplots(figsize=(10, 6))
    sns.barplot(data=df, x="instance", y="fitness", ax=ax)
    ax.set_title("Fitness par instance")
    ax.set_ylabel("Fitness (coût de la tournée)")
    ax.set_xlabel("Instances")
    ax.xaxis.labelpad = 8 
    ax.tick_params(axis='x', pad=10) 

    for i, row in df.iterrows():
        ax.text(i, row["fitness"], f"Gap: {row['gap']:.2f}%", ha='center', va='bottom')

    table_data = df[["instance", "fitness", "gap", "temps"]]
    plt.table(
        cellText=table_data.values,
        colLabels=table_data.columns,
        cellLoc='center',
        loc='bottom',
        bbox=[0, -0.5, 1, 0.3] 
    )
    plt.subplots_adjust(left=0.2, bottom=0.35) 

    params = df.iloc[0]
    param_text = (
        f"Paramètres : nb_generation={params['nb_generation']}, "
        f"taille_population={params['taille_population']}, "
        f"taux_croisement={params['taux_croisement']}, "
        f"taux_mutation={params['taux_mutation']}"
    )
    plt.figtext(0.5, 0.02, param_text, ha='center', fontsize=12, bbox={"facecolor":"#eee","pad":5,"edgecolor":"none"})

    filename = (
        f"results/genetique_summary_"
        f"gen{params['nb_generation']}_pop{params['taille_population']}_"
        f"cross{params['taux_croisement']}_mut{params['taux_mutation']}_"
        f"{params['croisement']}_{params['2opt']}.png"
    )

    plt.savefig(filename, bbox_inches='tight')
    plt.show()