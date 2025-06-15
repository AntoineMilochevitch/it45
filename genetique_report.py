# genetique_report.py
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Charger les résultats
df = pd.read_csv("results/algo_genetique_results.csv")

# Créer une figure
fig, ax = plt.subplots(figsize=(10, 6))
sns.barplot(data=df, x="instance", y="fitness", ax=ax)
ax.set_title("Fitness par instance")
ax.set_ylabel("Fitness (coût de la tournée)")
ax.set_xlabel("Instances")
ax.xaxis.labelpad = 8 
ax.tick_params(axis='x', pad=10) 


# Ajouter le texte des gaps sur les barres
for i, row in df.iterrows():
    ax.text(i, row["fitness"], f"Gap: {row['gap']:.2f}%", ha='center', va='bottom')

# Afficher un tableau sous le graphique
table_data = df[["instance", "fitness", "gap", "temps"]]
able = plt.table(
    cellText=table_data.values,
    colLabels=table_data.columns,
    cellLoc='center',
    loc='bottom',
    bbox=[0, -0.5, 1, 0.3] 
)
plt.subplots_adjust(left=0.2, bottom=0.35) 


# Récupérer les paramètres (on suppose qu'ils sont identiques pour toutes les lignes)
params = df.iloc[0]
param_text = (
    f"Paramètres : nb_generation={params['nb_generation']}, "
    f"taille_population={params['taille_population']}, "
    f"taux_croisement={params['taux_croisement']}, "
    f"taux_mutation={params['taux_mutation']}"
)

# Ajouter le texte des paramètres sous le graphique
plt.figtext(0.5, 0.02, param_text, ha='center', fontsize=12, bbox={"facecolor":"#eee","pad":5,"edgecolor":"none"})

filename = (
    f"results/genetique_summary_"
    f"gen{params['nb_generation']}_pop{params['taille_population']}_"
    f"cross{params['taux_croisement']}_mut{params['taux_mutation']}.png"
)

# Sauvegarder l'image
plt.savefig(filename, bbox_inches='tight')
plt.show()
plt.close()