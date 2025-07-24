#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <omp.h>
#include "equilibre_opti_de_nash.h"


float sCDN(float storage_cost, float capacity)
{
    return storage_cost * capacity;
}

struct Revenu
{
  double rev;
  float m1;
  float q1;
};


struct Revenu rcdn(float C1, float Qf1, struct Market market, struct CDN cdn, float p1, int coeff, float qc, float qf)
{
    //printf(" COEFFS %d, %d\n", coeff.coeff_1, coeff.coeff_2);
    float qf1 = qf;
    float qc1 = qc;
    float bet = 1-market.alph;
    float Da = pow(market.V, bet)/(bet);
    float A1 = pow(C1/market.V, bet);
    float Q1 = cdn.Qc*A1 + Qf1*(1-A1);
    float M1;
    if (coeff != 0)
    {
        M1 = fmaxf((Q1/pow(p1,2))/(1+(Q1/pow(p1,2))), 0);
    }else{
        M1 = 0;
    }
    
    float Db = cdn.request_price * Da/market.A;
    double R = market.A * (M1 * coeff * (Db - (Da-pow(C1, bet)/bet)*qf1 - (pow(C1, bet)/bet)*qc1));
    struct Revenu revenu;
    revenu.rev = R;
    revenu.m1 = M1;
    revenu.q1 = Q1;
    return revenu;
}

struct CN
{
  bool has_1;
  float c1;
  float p1;
  double rcp1;
  float m1;
  float q1;
};

struct CN choix2(struct Market market, struct CDN cdn, float Q1, float qc, float qf)
{
    //printf("choix2");
    double Rmax;
    Rmax = 0;
    struct CN Cn;
    Cn.has_1 = false;
    Cn.c1 = 0;
    Cn.p1 = 0;
    Cn.m1 = 0;
    Cn.q1 = 0;
    Cn.rcp1 = 0;
    float C1;
    int coeff;
    struct CPtuple result;
    double R;
    struct Revenu revenu;
    int maxI = 10*cdn.C+1;
    
    // TOUR 1
    for (int ii = 0; ii < maxI; ii++)
    {
        C1 = ii*0.1;
        coeff = 0;
        result = maxrcp(cdn.request_price, C1, Q1, market, cdn);
        if (result.has_1)
        {
            coeff = 1;
        }else{
            coeff = 0;
        }
        
        revenu = rcdn(C1, Q1, market, cdn, result.pk, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            Rmax = R;
            Cn.has_1 = coeff;
            Cn.c1 = C1;
            Cn.p1 = result.pk;
            Cn.rcp1 = result.rCP;
            Cn.m1 = revenu.m1;
            Cn.q1 = revenu.q1;
        }
      
    }
    
    // TOUR 2
    float start = Cn.c1 - (cdn.C/10)/2;
    for (int ii = 0; ii < maxI; ii++)
    {
        C1 = start + ii*0.01;
        coeff = 0;
        result = maxrcp(cdn.request_price, C1, Q1, market, cdn);
        if (result.has_1)
        {
            coeff = 1;
        }else{
            coeff = 0;
        }
        revenu = rcdn(C1, Q1, market, cdn, result.pk, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            Rmax = R;
            Cn.has_1 = result.has_1;
            Cn.c1 = C1;
            Cn.p1 = result.pk;
            Cn.rcp1 = result.rCP;
            Cn.m1 = revenu.m1;
            Cn.q1 = revenu.q1;
        }
    }
    
    // TOUR 3
    start = Cn.c1 - (cdn.C/100)/2;
    for (int ii = 0; ii < maxI; ii++)
    {
        C1 = start + ii*0.001;
        coeff = 0;
        result = maxrcp(cdn.request_price, C1, Q1, market, cdn);
        if (result.has_1)
        {
            coeff = 1;
        }else{
            coeff = 0;
        }
        revenu = rcdn(C1, Q1, market, cdn, result.pk, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            //printf("Mise à jour");
            Rmax = R;
            Cn.has_1 = result.has_1;
            Cn.c1 = C1;
            Cn.p1 = result.pk;
            Cn.rcp1 = result.rCP;
            Cn.m1 = revenu.m1;
            Cn.q1 = revenu.q1;
        }
    }
    
    //printf("end\n");
    return Cn;
}


int main()
{
    float Q1 = 20; // valeur arbitraire
    float qc = 0.6;
    float qf = 1;

    struct Market market;
    market.A = 5; // valeur arbitraire
    market.V = 200; // valeur arbitraire
    market.alph = 0.9; // valeur expérimentale (entre 0.5 et 1, plus on est proche de 1 plus la population est hétérogène)
    //market.storage_cost = 0.6; // valeur arbitraire
  
    struct CDN cdn;
    cdn.Qc = 80; // valeur arbitraire
    cdn.C = 3; // valeur arbitraire
    cdn.request_price = 30; // valeur arbitraire
    
    struct CN Cn;
    Cn = choix2(market, cdn, Q1, qc, qf);
    printf("Cn = %f\n", Cn.c1);
    
    
    FILE *f;
    char str[4454];
    f = fopen("data0.txt", "w");
 
    if(f == NULL){
      printf("Erreur lors de l'ouverture d'un fichier");
      exit(1);
    }
    
    //float Qfi[3] = {0, Q1};     
    float C1;
    int coeff;
    struct CPtuple result;
    double R;
    double r1;
    float m1;
    struct Revenu revenu;
    //float p[6][301];
        
   
    for (int i = 0; i < (int)(301); i++)
    {
      //printf("%d\n", i);
      C1 = i*0.01;
      coeff = 0;
      result = maxrcp(cdn.request_price, C1, Q1, market, cdn);
      if (result.has_1)
      {
          coeff = 1;
      }else{
          coeff = 0;
      }
      revenu = rcdn(C1,Q1, market, cdn, result.pk, coeff, qc, qf);
      R = revenu.rev;
      r1 = result.rCP;
      m1 = revenu.m1;
      
      sprintf(str, "%f %f %f %f\n", C1, R, r1, m1);
      fputs(str, f);
    }
  
    fclose(f);
    system("python3 tracer_courbes.py data0.txt");
    
    return 0;
}


