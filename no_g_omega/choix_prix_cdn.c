#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "choix_des_ci.h"

struct AllPrices
{
  float p;
  float r;
  float c1;
  float c2;
  float p1;
  float p2;
  double r1;
  double r2;
  float m1;
  float m2;
  float q1;
  float q2;
};

#define scope 5
#define precision 100

struct ToTrace
{
  struct AllPrices Vn;
  float pn[scope*precision];
  double Rn[scope*precision];
  float p1n[scope*precision];
  float p2n[scope*precision];
  float c1n[scope*precision];
  float c2n[scope*precision];
  float r1n[scope*precision];
  float r2n[scope*precision];
};

struct ToTrace choix_prix(struct Market market, struct CDN cdn, float Q1, float Q2, float qc, float qf)
{
    struct ToTrace all;
    all.Vn.p = 0;
    all.Vn.r = 0;
    all.Vn.c1 = 0;
    all.Vn.c2 = 0;
    all.Vn.p1 = 0;
    all.Vn.p2 = 0;
    all.Vn.r1 = 0;
    all.Vn.r2 = 0;
    double Rmax = 0;
    struct Coeffs coeff;
    struct CN Cn;
    double R;
    struct Revenu revenu;
    for (int i = 0; i < scope*precision; i++)
    {
        coeff.coeff_1 = 1;
        coeff.coeff_2 = 1;
        float p = round((0.01 + i*0.01)*precision)/precision;
        //printf("NEW PRICE %f - A = %f - g = %f\n", p, market.alph, cdn.storage_price);
        cdn.request_price = p;
        Cn = choix2(market, cdn, Q1, Q2, qc, qf);
        if (!Cn.has_1)
        {
            coeff.coeff_1 = 0;
        }
        if (!Cn.has_2)
        {
            coeff.coeff_2 = 0;
        }
        revenu = rcdn(Cn.c1, Cn.c2, Q1, Q2, market, cdn, Cn.p1, Cn.p2, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            Rmax = R;
            all.Vn.p = p;
            all.Vn.r = Rmax;
            all.Vn.c1 = Cn.c1;
            all.Vn.c2 = Cn.c2;
            all.Vn.p1 = Cn.p1;
            all.Vn.p2 = Cn.p2;
            all.Vn.r1 = Cn.rcp1;
            all.Vn.r2 = Cn.rcp2;
            all.Vn.m1 = Cn.m1;
            all.Vn.m2 = Cn.m2;
            all.Vn.q1 = Cn.q1;
            all.Vn.q2 = Cn.q2;
        }
        all.pn[i] = p;
        all.Rn[i] = R;
        all.c1n[i] = Cn.c1;
        all.c2n[i] = Cn.c2;
        all.p1n[i] = Cn.p1;
        all.p2n[i] = Cn.p2;
        all.r1n[i] = Cn.rcp1;
        all.r2n[i] = Cn.rcp2;
    }
    return all;
}

struct ToTrace choix_prix_opti(struct Market market, struct CDN cdn, float Q1, float Q2, float qc, float qf)
{
    struct ToTrace all;
    all.Vn.p = 0;
    all.Vn.r = 0;
    all.Vn.c1 = 0;
    all.Vn.c2 = 0;
    all.Vn.p1 = 0;
    all.Vn.p2 = 0;
    double Rmax = 0;
    struct Coeffs coeff;
    struct CN Cn;
    double R;
    struct Revenu revenu;
    
    // TOUR 1
    for (int i = 0; i < scope*10; i++)
    {
        coeff.coeff_1 = 1;
        coeff.coeff_2 = 1;
        float p = 0.1 + i*0.1;
        //printf("NEW PRICE %f - A = %f - g = %f\n", p, market.alph, cdn.storage_price);
        cdn.request_price = p;
        Cn = choix2(market, cdn, Q1, Q2, qc, qf);
        if (!Cn.has_1)
        {
            coeff.coeff_1 = 0;
        }
        if (!Cn.has_2)
        {
            coeff.coeff_2 = 0;
        }
        revenu = rcdn(Cn.c1, Cn.c2, Q1, Q2, market, cdn, Cn.p1, Cn.p2, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            Rmax = R;
            all.Vn.p = p;
            all.Vn.r = Rmax;
            all.Vn.c1 = Cn.c1;
            all.Vn.c2 = Cn.c2;
            all.Vn.p1 = Cn.p1;
            all.Vn.p2 = Cn.p2;
            all.Vn.r1 = Cn.rcp1;
            all.Vn.r2 = Cn.rcp2;
            all.Vn.m1 = Cn.m1;
            all.Vn.m2 = Cn.m2;
            all.Vn.q1 = Cn.q1;
            all.Vn.q2 = Cn.q2;
        }
    }
    
    // TOUR 2
    float start = all.Vn.p - (scope/100)/2;
    for (int i = 0; i < scope*10; i++)
    {
        coeff.coeff_1 = 1;
        coeff.coeff_2 = 1;
        float p = round((start + i*0.01)*100)/100;
        //printf("NEW PRICE %f - A = %f - g = %f\n", p, market.alph, cdn.storage_price);
        cdn.request_price = p;
        Cn = choix2(market, cdn, Q1, Q2, qc, qf);
        if (!Cn.has_1)
        {
            coeff.coeff_1 = 0;
        }
        if (!Cn.has_2)
        {
            coeff.coeff_2 = 0;
        }
        revenu = rcdn(Cn.c1, Cn.c2, Q1, Q2, market, cdn, Cn.p1, Cn.p2, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            Rmax = R;
            all.Vn.p = p;
            all.Vn.r = Rmax;
            all.Vn.c1 = Cn.c1;
            all.Vn.c2 = Cn.c2;
            all.Vn.p1 = Cn.p1;
            all.Vn.p2 = Cn.p2;
            all.Vn.r1 = Cn.rcp1;
            all.Vn.r2 = Cn.rcp2;
            all.Vn.m1 = Cn.m1;
            all.Vn.m2 = Cn.m2;
            all.Vn.q1 = Cn.q1;
            all.Vn.q2 = Cn.q2;
        }
    }
    
    // TOUR 3
    start = all.Vn.p - (scope/1000)/2;
    for (int i = 0; i < scope*10; i++)
    {
        coeff.coeff_1 = 1;
        coeff.coeff_2 = 1;
        float p = round((start + i*0.001)*1000)/1000;
        //printf("NEW PRICE %f - A = %f - g = %f\n", p, market.alph, cdn.storage_price);
        cdn.request_price = p;
        Cn = choix2(market, cdn, Q1, Q2, qc, qf);
        if (!Cn.has_1)
        {
            coeff.coeff_1 = 0;
        }
        if (!Cn.has_2)
        {
            coeff.coeff_2 = 0;
        }
        revenu = rcdn(Cn.c1, Cn.c2, Q1, Q2, market, cdn, Cn.p1, Cn.p2, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            Rmax = R;
            all.Vn.p = p;
            all.Vn.r = Rmax;
            all.Vn.c1 = Cn.c1;
            all.Vn.c2 = Cn.c2;
            all.Vn.p1 = Cn.p1;
            all.Vn.p2 = Cn.p2;
            all.Vn.r1 = Cn.rcp1;
            all.Vn.r2 = Cn.rcp2;
            all.Vn.m1 = Cn.m1;
            all.Vn.m2 = Cn.m2;
            all.Vn.q1 = Cn.q1;
            all.Vn.q2 = Cn.q2;
        }
    }
    
    return all;
}

int main()
{
  float Q1 = 20; // valeur arbitraire
  float Q2 = 40; // valeur arbitraire

  float p[12][51];

  struct Market market;
  market.A = 5; // valeur arbitraire
  market.V = 200; // valeur arbitraire
  market.alph = 0.9; // valeur expérimentale (entre 0.5 et 1, plus on est proche de 1 plus la population est hétérogène)
  market.omeg = 0.5;
  float qc = 0.6;
  float qf = 1;
  
  struct CDN cdn;
  cdn.Qc = 80; // valeur arbitraire
  cdn.C = 3; // valeur arbitraire
  cdn.request_price = 30; // valeur arbitraire à trouver
  struct ToTrace results;
  
  float Rmax = 0;
  float pmax = 0;
  float gmax = 0;
  double Rc1max = 0;
  double Rc2max = 0;
  float pc1max = 0;
  float pc2max = 0;
  float M1max = 0;
  float M2max = 0;
  float C1max = 0;
  float C2max = 0;
  float Q1max = 0;
  float Q2max = 0;
  
  for (int i = 0; i < 51; i++)
  {
    market.omeg = 0+0.02*i;
    Rmax = 0;
    pmax = 0;
    gmax = 0;
 
    
    results = choix_prix_opti(market, cdn, Q1, Q2, qc, qf);
    printf("p = %f - A = %f : R = %f\n", results.Vn.p, market.V, results.Vn.r);
      
    p[0][i] = results.Vn.r;
    p[1][i] = results.Vn.p;
    p[2][i] = results.Vn.r1;
    p[3][i] = results.Vn.p1;
    p[4][i] = results.Vn.r2;
    p[5][i] = results.Vn.p2;
    p[6][i] = results.Vn.m1;
    p[7][i] = results.Vn.m2;
    p[8][i] = results.Vn.c1;
    p[9][i] = results.Vn.c2;
    p[10][i] = results.Vn.q1;
    p[11][i] = results.Vn.q2;
    
  }
  
  FILE *f;
  char str[4454];
  f = fopen("data.txt", "w");
 
  if(f == NULL){
    printf("Erreur lors de l'ouverture d'un fichier");
    exit(1);
  }
  
  for (int i = 0; i < (int)(51); i++)
  {
    sprintf(str, "%f %f %f %f %f %f %f %f %f %f %f %f %f\n", 0.5+0.01*i, p[0][i], p[1][i], p[2][i], p[3][i], p[4][i], p[5][i], p[6][i], p[7][i], p[8][i], p[9][i], p[10][i], p[11][i]);
    fputs(str, f);
  }
  
  fclose(f);
  system("python3 tracer_courbes.py data.txt");
  
  return 0;
}
