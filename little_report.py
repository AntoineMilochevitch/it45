import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Charger les résultats
df = pd.read_csv("results/little_results.csv")
df["gap"] = df["gap"].astype(float)

fig, ax = plt.subplots(figsize=(10, 6))
sns.barplot(data=df, x="instance", y="cout", ax=ax)
ax.set_title("Fitness par instance (Little)")
ax.set_ylabel("Fitness (coût de la tournée)")
ax.set_xlabel("Instances")
ax.xaxis.labelpad = 8 
ax.tick_params(axis='x', pad=10) 

# Ajouter le texte des gaps sur les barres
for i, row in df.iterrows():
    ax.text(i, row["cout"], f"Gap: {row['gap']:.2f}%", ha='center', va='bottom')

# Tableau en dessous du graphe
table_data = df[["instance", "cout", "gap", "temps"]]
plt.table(
    cellText=table_data.values,
    colLabels=table_data.columns,
    cellLoc='center',
    loc='bottom',
    bbox=[0, -0.5, 1, 0.3] 
)
plt.subplots_adjust(left=0.2, bottom=0.35) 

# Affichage des paramètres globaux s’ils existent
if "temps_limite" in df.columns:
    param_text = f"Paramètres : temps_limite={df.iloc[0]['temps_limite']}s"
    plt.figtext(0.5, 0.02, param_text, ha='center', fontsize=12, bbox={"facecolor":"#eee","pad":5,"edgecolor":"none"})

# Enregistrement du graphe
plt.savefig("results/little_summary.png", bbox_inches='tight')
plt.show()