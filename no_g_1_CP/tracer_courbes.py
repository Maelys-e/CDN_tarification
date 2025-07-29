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
    
    a = N[0]
    R = N[1]
    r1 = N[2]
    #r1 = N[3]
    #p1 = N[4]
    #m1 = N[5]
    #c1 = N[6]
    #q1 = N[7]
    

    plt.style.use("ggplot")
    timenow = datetime.datetime.utcnow().replace(microsecond = 0).isoformat()
    output_dir = "results"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    plt.plot(a, R, label = "RCDN")
    #plt.plot(a, p, label = "pCDN")
    plt.plot(a, r1, label = "R")
    #plt.plot(a, p1, label = "p")
    #plt.plot(a, m1, label = "M")
    #plt.plot(a, c1, label = "C")
    #plt.plot(a, q1, label = "Q")
    plt.legend(loc = 1)
    plt.xlabel("C1")
    plt.ylabel("RCDN")
    plt.ylim(-2, 50)
    plt.xlim(0, 3)
    output_file = os.path.join(output_dir, f"cdn1_big-Q_R_C1_no-g_{timenow}.tex")
    tikzplotlib.save(output_file)
    plt.show()
    
  
