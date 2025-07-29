all : choixPrix

# pour lancer un simulation complète
choixPrix : choix_prix_cdn.o choix_des_ci.o equilibre_opti_de_nash.o
	gcc -g -fopenmp -o choix_prix_cdn choix_prix_cdn.o choix_des_ci.o equilibre_opti_de_nash.o -lm

choix_prix_cdn.o : choix_prix_cdn.c
	gcc -g -O0 -fopenmp -c -Wall choix_prix_cdn.c

# pour simuler seulement les deux derniers étages de la retro-induction (décommenter la fonction `main` de choix_des_ci.c)
juste_ci : choix_des_ci.o equilibre_opti_de_nash.o
	gcc -o choix_des_ci choix_des_ci.o equilibre_opti_de_nash.o -lm
choix_des_ci.o : choix_des_ci.c
	gcc -g -O0 -fopenmp -c -Wall choix_des_ci.c

# pour simuler seulement le dernier étage de la rétro-induction (décommenter la fonction `main` de equilibre_opti_de_nash.c)
test_opti : equilibre_opti_de_nash.o
	gcc -o equilibre_opti_de_nash equilibre_opti_de_nash.o -lm
equilibre_opti_de_nash.o : equilibre_opti_de_nash.c
	gcc -c -Wall equilibre_opti_de_nash.c

clean :
	rm -rf *.o
