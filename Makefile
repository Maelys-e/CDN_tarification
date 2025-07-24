all : choixPrix

vibrations : print_pi_pi.o equilibre_de_nash.o
	gcc -o print_pi_pi print_pi_pi.o equilibre_de_nash.o -lm

market : print_M_ci.o equilibre_de_nash.o
	gcc -o print_M_ci print_M_ci.o equilibre_de_nash.o -lm

quality : print_Q_ci.o equilibre_de_nash.o
	gcc -o print_Q_ci print_Q_ci.o equilibre_de_nash.o -lm

choixPrix : choix_prix_cdn.o choix_des_ci.o equilibre_opti_de_nash.o
	gcc -g -fopenmp -o choix_prix_cdn choix_prix_cdn.o choix_des_ci.o equilibre_opti_de_nash.o -lm

choix_prix_cdn.o : choix_prix_cdn.c
	gcc -g -O0 -fopenmp -c -Wall choix_prix_cdn.c

juste_ci : choix_des_ci.o equilibre_opti_de_nash.o
	gcc -o choix_des_ci choix_des_ci.o equilibre_opti_de_nash.o -lm
choix_des_ci.o : choix_des_ci.c
	gcc -g -O0 -fopenmp -c -Wall choix_des_ci.c

equilibre_de_nash.o : equilibre_de_nash.c
	gcc -c -Wall equilibre_de_nash.c

print_pi_pi.o : print_pi_pi.c
	gcc -c -Wall print_pi_pi.c

print_M_ci.o : print_M_ci.c
	gcc -c -Wall print_M_ci.c

print_Q_ci.o : print_Q_ci.c
	gcc -c -Wall print_Q_ci.c

test_opti : equilibre_opti_de_nash.o
	gcc -o equilibre_opti_de_nash equilibre_opti_de_nash.o -lm
equilibre_opti_de_nash.o : equilibre_opti_de_nash.c
	gcc -c -Wall equilibre_opti_de_nash.c

clean :
	rm -rf *.o
