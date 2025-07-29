import sys
import os
import numpy as np
import matplotlib.pyplot as plt
import tikzplotlib
import time
import datetime


if __name__ == "__main__" :

    if len(sys.argv) != 2 :
        print("il faut un parametre en ligne de commande : le nom de l'instance")
        sys.exit()

    # on récupère les résultats de simulation dans data.txt
    nomFic = sys.argv[1]
    if not os.path.exists(nomFic) :
        print("ERREUR Ouverture fichier " + nomFic)
        sys.exit()
    M = np.loadtxt(nomFic)
    N = M.T

    # on sépare chaque variable dans une liste
    V = N[0]
    R = N[1]
    p = N[2]
    g = N[3]
    r1 = N[4]
    p1 = N[5]
    r2 = N[6]
    p2 = N[7]
    m1 = N[8]
    m2 = N[9]
    C1 = N[10]
    C2 = N[11]
    q1 = N[12]
    q2 = N[13]
    
    # on récupère l'heure et la date pour nommer le fichier final
    plt.style.use("ggplot")
    timenow = datetime.datetime.utcnow().replace(microsecond = 0).isoformat()
    output_dir = "results"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # on trace les courbes et on met en forme le graphe
    plt.plot(V, R, label = "RCDN")
    plt.plot(V, r1, label = "R1")
    plt.plot(V, r2, label = "R2")
    plt.plot(V, g, label = "gCDN")
    plt.plot(V, p, label = "pCDN")
    plt.plot(V, p1, label = "p1")
    plt.plot(V, p2, label = "p2")
    plt.plot(V, C1, label = "C1")
    plt.plot(V, C2, label = "C2")
    plt.plot(V, m1, label = "M1")
    plt.plot(V, m2, label = "M2")
    plt.plot(V, q1, label = "Q1")
    plt.plot(V, q2, label = "Q2")
    plt.legend(loc = 1)
    # à partir d'ici, lignes à modifier pour changer de variable de référence
    plt.title("Résultats en fonction de V", fontdict={'family': 'serif'})
    plt.xlabel("V")
    plt.ylabel("RCDN")
    # lignes à changer pour déplacer la fenêtre de simulation
    plt.ylim(-10, 100)
    plt.xlim(0.5, 1)

    # on enregistre le graphe au formet .tex
    output_file = os.path.join(output_dir, f"vanilla_all_V_{timenow}.tex")
    tikzplotlib.save(output_file)
    plt.show()
