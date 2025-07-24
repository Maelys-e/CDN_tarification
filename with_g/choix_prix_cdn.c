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
        printf("NEW PRICE %f - A = %f - g = %f\n", p, market.alph, cdn.storage_price);
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
        printf("coeffs : %d %d\n", coeff.coeff_1, coeff.coeff_2);
        revenu = rcdn(Cn.c1, Cn.c2, Q1, Q2, market, cdn, Cn.p1, Cn.p2, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            //printf("Mise à jour\n");
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
        //printf("coeffs : %d %d\n", coeff.coeff_1, coeff.coeff_2);
        revenu = rcdn(Cn.c1, Cn.c2, Q1, Q2, market, cdn, Cn.p1, Cn.p2, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            //printf("Mise à jour\n");
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
        //printf("coeffs : %d %d\n", coeff.coeff_1, coeff.coeff_2);
        revenu = rcdn(Cn.c1, Cn.c2, Q1, Q2, market, cdn, Cn.p1, Cn.p2, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            //printf("Mise à jour\n");
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
        //printf("coeffs : %d %d\n", coeff.coeff_1, coeff.coeff_2);
        revenu = rcdn(Cn.c1, Cn.c2, Q1, Q2, market, cdn, Cn.p1, Cn.p2, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            //printf("Mise à jour\n");
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
  float Q1 = 1.2; // valeur arbitraire - qualité du service de CP1 sans passer par le serveur proche du CDN
  float Q2 = 1.5; // valeur arbitraire - qualité du service de CP2 sans passer par le serveur proche du CDN
  float qc = 0.6; // valeur arbitraire - coût de transfert pour le CDN d'une requête qui passe par son serveur proche
  float qf = 1; // valur arbitraire - coût de transfert pour le CDN d'une requête qui ne passe pas par son serveur proche
  
  struct Market market;
  market.A = 4; // valeur arbitraire
  market.V = 200; // valeur arbitraire
  market.alph = 0.8; // valeur expérimentale (entre 0.5 et 1, plus on est proche de 1 plus la population est hétérogène)
  market.storage_cost = 0.6; // valeur arbitraire - coût de stockage des données pour le CDN
  
  struct CDN cdn;
  cdn.Qc = 2; // valeur arbitraire - qualité de service d'une requête tranferrée par le serveur proche du CDN
  cdn.C = 3; // valeur arbitraire
  cdn.storage_price = 0.5; // valeur arbitraire - variable de décision - prix du stockage de données
  cdn.request_price = 1; // valeur arbitraire - variable de décision - prix d'une requête
  
  struct ToTrace results;

  #define lissage 30
  float p[13][lissage]; // tableau qui rassemble les résultats
  
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
  
  for (int i = 0; i < lissage; i++)
  {
    printf("%d\n", i);
    market.V = 10.0+10*i;  // choisir n'importe quelle valeur et adapter les lignes 281 et 395 à la fenêtre souhaitée
    Rmax = 0;
    pmax = 0;
    gmax = 0;
    
  
    //TOUR 1
    for (int j = 0; j < 31; j++)
    {
      cdn.storage_price = 0 + 10*j;
      results = choix_prix_opti(market, cdn, Q1, Q2, qc, qf);
      printf("p = %f - V = %f - g = %f : R = %f\n", results.Vn.p, market.V, cdn.storage_price, results.Vn.r);
      
      if (results.Vn.r > Rmax)
      {
        gmax = cdn.storage_price;
        pmax = results.Vn.p;
        Rmax = results.Vn.r;
        pc1max = results.Vn.p1;
        Rc1max = results.Vn.r1;
        pc2max = results.Vn.p2;
        Rc2max = results.Vn.r2;
        M1max = results.Vn.m1;
        M2max = results.Vn.m2;
        C1max = results.Vn.c1;
        C2max = results.Vn.c2;
        Q1max = results.Vn.q1;
        Q2max = results.Vn.q2;
      }
    }
    
    float start = gmax - 10/(1*1);
    //TOUR 2
    for (int j = 0; j < 10; j++)
    {
      cdn.storage_price = start + 2*j + 0;
      results = choix_prix_opti(market, cdn, Q1, Q2, qc, qf);
      printf("p = %f - V = %f - g = %f : R = %f\n", results.Vn.p, market.V, cdn.storage_price, results.Vn.r);
      
      if (results.Vn.r > Rmax)
      {
        gmax = cdn.storage_price;
        pmax = results.Vn.p;
        Rmax = results.Vn.r;
        pc1max = results.Vn.p1;
        Rc1max = results.Vn.r1;
        pc2max = results.Vn.p2;
        Rc2max = results.Vn.r2;
        M1max = results.Vn.m1;
        M2max = results.Vn.m2;
        C1max = results.Vn.c1;
        C2max = results.Vn.c2;
        Q1max = results.Vn.q1;
        Q2max = results.Vn.q2;
      }
    }
    
    start = gmax - 2/(1*1);
    //TOUR 3
    for (int j = 0; j < 10; j++)
    {
      cdn.storage_price = start + 0.4*j + 0;
      results = choix_prix_opti(market, cdn, Q1, Q2, qc, qf);
      printf("p = %f - V = %f - g = %f : R = %f\n", results.Vn.p, market.V, cdn.storage_price, results.Vn.r);
      
      if (results.Vn.r > Rmax)
      {
        gmax = cdn.storage_price;
        pmax = results.Vn.p;
        Rmax = results.Vn.r;
        pc1max = results.Vn.p1;
        Rc1max = results.Vn.r1;
        pc2max = results.Vn.p2;
        Rc2max = results.Vn.r2;
        M1max = results.Vn.m1;
        M2max = results.Vn.m2;
        C1max = results.Vn.c1;
        C2max = results.Vn.c2;
        Q1max = results.Vn.q1;
        Q2max = results.Vn.q2;
      }
    }
    
  
    p[0][i] = Rmax;
    p[1][i] = pmax;
    p[2][i] = gmax;
    p[3][i] = Rc1max;
    p[4][i] = pc1max;
    p[5][i] = Rc2max;
    p[6][i] = pc2max;
    p[7][i] = M1max;
    p[8][i] = M2max;
    p[9][i] = C1max;
    p[10][i] = C2max;
    p[11][i] = Q1max;
    p[12][i] = Q2max;
    
    
  }
  printf("###\nRevenu maximum = %f pour pCDN = %f et gCDN = %f\n###\n", Rmax, pmax, gmax);

  
  FILE *f;
  char str[4454];
  f = fopen("data.txt", "w");
 
  if(f == NULL){
    printf("Erreur lors de l'ouverture d'un fichier");
    exit(1);
  }
  
  for (int i = 0; i < (int)(lissage); i++)
  {
    sprintf(str, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", 10.0+10*i, p[0][i], p[1][i], p[2][i], p[3][i], p[4][i], p[5][i], p[6][i], p[7][i], p[8][i], p[9][i], p[10][i], p[11][i], p[12][i]);
    fputs(str, f);
  }
  
  fclose(f);
  system("python3 tracer_courbes.py data.txt");
  
  return 0;
}
