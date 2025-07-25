import sys
import os
import numpy as np
import matplotlib.pyplot as plt
import tikzplotlib
import time
import datetime

vert4 = "#88CC22"
vert5 = "#77DD44"
vert6 = "#66DD66"
vert7 = "#55EE88"
vert8 = "#44EEAA"
vert9 = "#33EEAA"
vert10 = "#22DDAA"
vert11 = "#11BB88"
vert12 = "#009966"
vert13 = "#007744"


if __name__ == "__main__" :

    if len(sys.argv) != 2 :
        print("il faut un parametre en ligne de commande : le nom de l'instance")
        sys.exit()
    nomFic = sys.argv[1]
    if not os.path.exists(nomFic) :
        print("ERREUR Ouverture fichier " + nomFic)
        sys.exit()
    M = np.loadtxt(nomFic)
    N = M.T
    
    a = N[0]
    R = N[1]
    p = N[2]
    r1 = N[3]
    p1 = N[4]
    r2 = N[5]
    p2 = N[6]
    m1 = N[7]
    m2 = N[8]
    c1 = N[9]
    c2 = N[10]
    q1 = N[11]
    q2 = N[12]
    

    plt.style.use("ggplot")
    timenow = datetime.datetime.utcnow().replace(microsecond = 0).isoformat()
    output_dir = "results"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    plt.plot(a, R, label = "RCDN")
    plt.plot(a, p, label = "pCDN")
    plt.plot(a, r1, label = "R1")
    plt.plot(a, p1, label = "p1")
    plt.plot(a, r2, label = "R2")
    plt.plot(a, p2, label = "p2")
    plt.plot(a, m1, label = "M1")
    plt.plot(a, m2, label = "M2")
    plt.plot(a, c1, label = "C1")
    plt.plot(a, c2, label = "C2")
    plt.plot(a, q1, label = "Q1")
    plt.plot(a, q2, label = "Q2")
    plt.legend(loc = 1)
    plt.title("Résultats en fonction de V", fontdict={'family': 'serif'})
    plt.xlabel("V")
    plt.ylabel("RCDN")
    plt.ylim(-2, 130)
    plt.xlim(10, 300)
    output_file = os.path.join(output_dir, f"cdn2_big-Q_all_V_no-g_{timenow}.tex")
    tikzplotlib.save(output_file)
    plt.show()
    
