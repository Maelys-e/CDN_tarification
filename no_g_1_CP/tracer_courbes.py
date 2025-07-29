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
    
    V = N[0]
    R = N[1]
    p1 = N[2]
    r1 = N[3]
    p1 = N[4]
    m1 = N[5]
    c1 = N[6]
    q1 = N[7]
    

    plt.style.use("ggplot")
    timenow = datetime.datetime.utcnow().replace(microsecond = 0).isoformat()
    output_dir = "results"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    plt.plot(V, R, label = "RCDN")
    plt.plot(V, p1, label = "pCDN")
    plt.plot(V, r1, label = "R")
    plt.plot(V, p1, label = "p")
    plt.plot(V, m1, label = "M")
    plt.plot(V, c1, label = "C")
    plt.plot(V, q1, label = "Q")
    plt.legend(loc = 1)
    plt.xlabel("V")
    plt.ylabel("RCDN")
    plt.ylim(-2, 100)
    plt.xlim(10, 300)
    
    output_file = os.path.join(output_dir, f"cdn1_big-Q_R_C1_no-g_{timenow}.tex")
    tikzplotlib.save(output_file)
    plt.show()
    
  
