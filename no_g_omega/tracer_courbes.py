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
    nomFic = sys.argv[1]
    if not os.path.exists(nomFic) :
        print("ERREUR Ouverture fichier " + nomFic)
        sys.exit()
    M = np.loadtxt(nomFic)
    N = M.T
    
    w = N[0]
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
    
    plt.plot(w, R, label = "RCDN")
    plt.plot(w, p, label = "pCDN")
    plt.plot(w, r1, label = "R1")
    plt.plot(w, p1, label = "p1")
    plt.plot(w, r2, label = "R2")
    plt.plot(w, p2, label = "p2")
    plt.plot(w, m1, label = "M1")
    plt.plot(w, m2, label = "M2")
    plt.plot(w, c1, label = "C1")
    plt.plot(w, c2, label = "C2")
    plt.plot(w, q1, label = "Q1")
    plt.plot(w, q2, label = "Q2")
    plt.legend(loc = 1)
    plt.xlabel("omega")
    plt.ylabel("RCDN")
    plt.ylim(-2, 200)
    plt.xlim(0, 1)
    
    output_file = os.path.join(output_dir, f"het_cdn2_big-Q_all_omega_no-g_{timenow}.tex")
    tikzplotlib.save(output_file)
    plt.show()
    
  
