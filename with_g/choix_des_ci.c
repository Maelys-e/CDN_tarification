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

struct Coeffs
{
  int coeff_1;
  int coeff_2;
};

struct Revenu
{
  double rev;
  float m1;
  float m2;
  float q1;
  float q2;
};


struct Revenu rcdn(float C1, float C2, float Qf1, float Qf2, struct Market market, struct CDN cdn, float p1, float p2, struct Coeffs coeff, float qc, float qf)
/*
Calcule le revenu du CDN, mais aussi les parts de marché et les qualités de chaque CP
*/
{
    float qf1 = qf;  float qf2 = qf;
    float qc1 = qc;  float qc2 = qc;
    float bet = 1-market.alph;
    float A1 = pow(C1/market.V, bet);   float A2 = pow(C2/market.V, bet);
    float Q1 = cdn.Qc*A1 + Qf1*(1-A1);  float Q2 = cdn.Qc*A2 + Qf2*(1-A2);
    float M1;                           float M2;
    
    if (coeff.coeff_1 != 0)
    {
        if (coeff.coeff_2 != 0)
        {
            M1 = fmaxf((Q1/pow(p1,2))/(1+(Q1/pow(p1,2))+(Q2/pow(p2,2))), 0);
        }else{
            M1 = fmaxf((Q1/pow(p1,2))/(1+(Q1/pow(p1,2))), 0);
        }
    }else{
        M1 = 0;
    }
    if (coeff.coeff_2 != 0)
    {
        if (coeff.coeff_1 != 0)
        {
            M2 = fmaxf((Q2/pow(p2,2))/(1+(Q1/pow(p1,2))+(Q2/pow(p2,2))), 0);
        }else{
            M2 = fmaxf((Q2/pow(p2,2))/(1+(Q2/pow(p2,2))), 0);
        }
    }else{
        M2 = 0;
    }
    
    float Da = pow(market.V, bet)/(bet);   float Db = cdn.request_price * Da/market.A;
    
    double R = cdn.storage_price*(coeff.coeff_1*C1+coeff.coeff_2*C2) + market.A * (M1 * coeff.coeff_1 * (Db - (Da-pow(C1, bet)/bet)*qf1 - (pow(C1, bet)/bet)*qc1) + M2 * coeff.coeff_2 * (Db - (Da-pow(C2, bet)/bet)*qf2 - (pow(C2, bet)/bet)*qc2)) - sCDN(market.storage_cost, cdn.C);
    struct Revenu revenu;
    revenu.rev = R;
    revenu.m1 = M1; revenu.m2 = M2;
    revenu.q1 = Q1; revenu.q2 = Q2;
    
    return revenu;
}

struct CN
{
  bool has_1;  bool has_2;
  float c1;    float c2;
  float p1;    float p2;
  double rcp1; double rcp2;
  float m1;    float m2;
  float q1;    float q2;
};

struct CN choix2(struct Market market, struct CDN cdn, float Q1, float Q2, float qc, float qf)
/*
Détermine l'allocation idéale des Ci (C1 et C2) et renvoie une structure contenant toutes les variables associées
*/
{
    double Rmax;
    Rmax = 0;
    struct CN Cn;
    Cn.has_1 = false;  Cn.has_2 = false;
    Cn.c1 = 0;         Cn.c2 = 0;
    Cn.p1 = 0;         Cn.p2 = 0;
    Cn.q1 = 0;         Cn.q2 = 0;
    Cn.rcp1 = 0;       Cn.rcp2 = 0;
    Cn.m1 = 0;         Cn.m2 = 0;
    
    float C1; float C2;
    struct Coeffs coeff;
    struct Nash result;
    double R;
    struct Revenu revenu;
    int maxI = 10*cdn.C+1;
    
    // TOUR 1
    for (int ii = 0; ii < maxI; ii++)
    {
        C1 = ii*0.1;
        C2 = cdn.C - C1;
        coeff.coeff_1 = 0; coeff.coeff_2 = 0;
        result = nash2(C1, C2, Q1, Q2, market, cdn);
        
        if (result.has_1)
        {
            coeff.coeff_1 = 1;
        }else{
            coeff.coeff_1 = 0;
        }
        if (result.has_2)
        {
            coeff.coeff_2 = 1;
        }else{
            coeff.coeff_2 = 0;
        }
        
        revenu = rcdn(C1, C2, Q1, Q2, market, cdn, result.value_1, result.value_2, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            Rmax = R;
            Cn.has_1 = result.has_1;  Cn.has_2 = result.has_2;
            Cn.c1 = C1;               Cn.c2 = C2;
            Cn.p1 = result.value_1;   Cn.p2 = result.value_2;
            Cn.rcp1 = result.rCP1;    Cn.rcp2 = result.rCP2;
            Cn.m1 = revenu.m1;        Cn.m2 = revenu.m2;
            Cn.q1 = revenu.q1;        Cn.q2 = revenu.q2;
        }
      
    }
    
    // TOUR 2
    float start = Cn.c1 - (cdn.C/10)/2;
    for (int ii = 0; ii < maxI; ii++)
    {
        C1 = start + ii*0.01;
        C2 = cdn.C - C1;
        coeff.coeff_1 = 0;  coeff.coeff_2 = 0;
        result = nash2(C1, C2, Q1, Q2, market, cdn);
        
        if (result.has_1)
        {
            coeff.coeff_1 = 1;
        }else{
            coeff.coeff_1 = 0;
        }
        if (result.has_2)
        {
            coeff.coeff_2 = 1;
        }else{
            coeff.coeff_2 = 0;
        }
        
        revenu = rcdn(C1, C2, Q1, Q2, market, cdn, result.value_1, result.value_2, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            Rmax = R;
            Cn.has_1 = result.has_1;  Cn.has_2 = result.has_2;
            Cn.c1 = C1;               Cn.c2 = C2;
            Cn.p1 = result.value_1;   Cn.p2 = result.value_2;
            Cn.rcp1 = result.rCP1;    Cn.rcp2 = result.rCP2;
            Cn.m1 = revenu.m1;        Cn.m2 = revenu.m2;
            Cn.q1 = revenu.q1;        Cn.q2 = revenu.q2;
        }
    }
    
    // TOUR 3
    start = Cn.c1 - (cdn.C/100)/2;
    for (int ii = 0; ii < maxI; ii++)
    {
        C1 = start + ii*0.001;
        C2 = cdn.C - C1;
        coeff.coeff_1 = 0;  coeff.coeff_2 = 0;
        result = nash2(C1, C2, Q1, Q2, market, cdn);
        
        if (result.has_1)
        {
            coeff.coeff_1 = 1;
        }else{
            coeff.coeff_1 = 0;
        }
        if (result.has_2)
        {
            coeff.coeff_2 = 1;
        }else{
            coeff.coeff_2 = 0;
        }
        
        revenu = rcdn(C1, C2, Q1, Q2, market, cdn, result.value_1, result.value_2, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            Rmax = R;
            Cn.has_1 = result.has_1;  Cn.has_2 = result.has_2;
            Cn.c1 = C1;               Cn.c2 = C2;
            Cn.p1 = result.value_1;   Cn.p2 = result.value_2;
            Cn.rcp1 = result.rCP1;    Cn.rcp2 = result.rCP2;
            Cn.m1 = revenu.m1;        Cn.m2 = revenu.m2;
            Cn.q1 = revenu.q1;        Cn.q2 = revenu.q2;
        }
    }
    
    return Cn;
}

/*
int main()
{
    float Q1 = 1.2; // valeur arbitraire
    float Q2 = 1.5; // valeur arbitraire
    float qc = 0.6;
    float qf = 1;

    struct Market market;
    market.A = 4; // valeur arbitraire
    market.V = 200; // valeur arbitraire
    market.alph = 0.8; // valeur expérimentale (entre 0.5 et 1, plus on est proche de 1 plus la population est hétérogène)
    market.storage_cost = 0.6; // valeur arbitraire
  
    struct CDN cdn;
    cdn.Qc = 2; // valeur arbitraire
    cdn.C = 3; // valeur arbitraire
    cdn.storage_price = 5; // valeur arbitraire
    cdn.request_price = 1; // valeur arbitraire
    
    struct CN Cn;
    Cn = choix2(market, cdn, Q1, Q2, qc, qf);
    printf("Cn = %f, %f\n", Cn.c1, Cn.c2);
    
    
    FILE *f;
    char str[4454];
    f = fopen("data.txt", "w");
 
    if(f == NULL){
      printf("Erreur lors de l'ouverture d'un fichier");
      exit(1);
    }
    
    float Qfi[3] = {0, Q1, Q2};     
    float C1;
    float C2;
    struct Coeffs coeff;
    struct Nash result;
    double R;
    double r1;
    double r2;
    float m1;
    float m2;
    struct Revenu revenu;
        
    for (int i = 0; i < (int)(301); i++)
    {
      C1 = i*0.01;
      C2 = cdn.C - C1;
      coeff.coeff_1 = 0;
      coeff.coeff_2 = 0;
      result = nash2(C1, C2, Q1, Q2, market, cdn);
      if (result.has_1)
      {
          coeff.coeff_1 = 1;
      }else{
          coeff.coeff_1 = 0;
      }
      if (result.has_2)
      {
          coeff.coeff_2 = 1;
      }else{
          coeff.coeff_2 = 0;
      }
      revenu = rcdn(C1, C2, Q1, Q2, market, cdn, result.value_1, result.value_2, coeff, qc, qf);
      R = revenu.rev;
      r1 = result.rCP1;
      r2 = result.rCP2;
      m1 = revenu.m1;
      m2 = revenu.m2;
      
      sprintf(str, "%f %f %f %f %f %f\n", C1, R, r1, r2, m1, m2);
      fputs(str, f);
    }
  
    fclose(f);
    system("python3 tracer_courbes.py data.txt");
    
    return 0;
}
*/
