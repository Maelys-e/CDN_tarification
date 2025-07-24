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
    #plt.scatter(M[:,0], M[:,1])
    #print(M)
    N = M.T
    
    #cdn_prices = N[0]
    #print("prices :", cdn_prices)
    """
    cdn_revenu = N[1]
    c1 = N[2]
    c2 = N[3]
    p1 = N[4]
    p2 = N[5]
    r1 = N[6]
    r2 = N[7]
    """
    """
    r1 = N[1]
    r2 = N[2]
    r3 = N[3]
    r4 = N[4]
    r5 = N[5]
    r6 = N[6]
    """
    
    a = N[0]
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
    
    #for i in range(1, len(N[1])-1) :
    #    if np.abs(N[1][i] - N[1][i-1]) > 1 and np.abs(N[1][i] - N[1][i+1]) > 1 :
    #        N[1][i] = (N[1][i-1] + N[1][i+1])/2
    

    plt.style.use("ggplot")
    timenow = datetime.datetime.utcnow().replace(microsecond = 0).isoformat()
    output_dir = "results"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    plt.plot(a, R, label = "RCDN")
    plt.plot(a, r1, label = "R1")
    plt.plot(a, r2, label = "R2")
    plt.plot(a, g, label = "gCDN")
    plt.plot(a, p, label = "pCDN")
    plt.plot(a, p1, label = "p1")
    plt.plot(a, p2, label = "p2")
    plt.plot(a, C1, label = "C1")
    plt.plot(a, C2, label = "C2")
    plt.plot(a, m1, label = "M1")
    plt.plot(a, m2, label = "M2")
    plt.plot(a, q1, label = "Q1")
    plt.plot(a, q2, label = "Q2")
    plt.legend(loc = 1)
    plt.xlabel("V")
    plt.ylabel("RCDN")
    plt.ylim(-10, 100)
    plt.xlim(0.5, 1)
    output_file = os.path.join(output_dir, f"vanilla_all_V_{timenow}.tex")
    tikzplotlib.save(output_file)
    plt.show()
    
    """
    plt.xlabel("Prix du CDN", fontdict={'family': 'serif'})
    plt.ylabel("Capacité allouée", fontdict={'family': 'serif'})
    plt.plot(cdn_prices, c1, label = "C1 (/9)", linewidth = 0.5, color = "#66FF33")
    plt.plot(cdn_prices, c2, label = "C2 (/9)", linewidth = 0.5, color = "#FF9933")
    plt.legend(fontsize = 10, loc = 1)
    plt.ylim(-1, 10)
    plt.xlim(0, 11.5)
    plt.title("Répartition des capacités du CDN entre les CPs", fontdict={'family': 'serif'})
    output_file = os.path.join(output_dir, f"ci_allocation_{timenow}.tex")
    tikzplotlib.save(output_file)
    plt.show()
    
    plt.title("Prix optimaux des CPs", fontdict={'family': 'serif'})
    plt.xlabel("Prix du CDN", fontdict={'family': 'serif'})
    plt.ylabel("Prix des CPs", fontdict={'family': 'serif'})
    plt.plot(cdn_prices, p1, label = "prix CP1", linestyle = "--", color = "#006600")
    plt.plot(cdn_prices, p2, label = "prix CP2", linestyle = "--", color = "#993300")
    plt.legend(fontsize = 10, loc = 1)
    plt.ylim(-1, 31)
    plt.xlim(0, 11.5)
    plt.show()
    """
    """
    plt.title("Revenu d'un CDN seul", fontdict={'family': 'serif', 'size':15})
    plt.plot(cdn_prices, r1, label = "G = 6", linewidth = 1, color = vert4) #99cc66
    plt.plot(cdn_prices, r2, label = "G = 7", linewidth = 1, color = vert5) #cc9900
    #plt.plot(cdn_prices, p21, label = "G = 0", linewidth = 1, color = vert4) #3388ee
    plt.plot(cdn_prices, r3, label = "G = 8", linewidth = 1, color = vert7)
    plt.plot(cdn_prices, r4, label = "G = 9", linewidth = 1, color = vert9)
    #plt.plot(cdn_prices, r5, label = "G = 2", linewidth = 1, color = vert7) #99cc66
    plt.plot(cdn_prices, r5, label = "G = 10", linewidth = 1, color = vert11) #cc9900
    plt.plot(cdn_prices, r6, label = "G = 11", linewidth = 1, color = vert13) #3388ee
    #plt.plot(cdn_prices, p23, label = "G = 4", linewidth = 1, color = vert10)
    plt.legend(fontsize = 15, prop={'family': 'serif', 'size': 13}, loc = 1)
    plt.ylim(-6, 25)
    plt.xlim(0, 2)
    plt.xlabel("Prix du CDN", fontdict={'family': 'serif', 'size': 13})
    plt.ylabel("Revenu", fontdict={'family': 'serif', 'size': 13})
    #plt.savefig("graph.png")
    ax = plt.gca()
    for label in ax.get_xticklabels():
        label.set_fontproperties({'family': 'serif', 'size': 12})
    for label in ax.get_yticklabels():
        label.set_fontproperties({'family': 'serif', 'size': 12})
    output_file = os.path.join(output_dir, f"cdn_revenu_{timenow}.tex")
    tikzplotlib.save(output_file)
    plt.show()
    """
