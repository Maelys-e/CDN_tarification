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
  float p1;
  double r1;
  float m1;
  float q1;
};

#define scope 5
#define precision 100

struct ToTrace
{
  struct AllPrices Vn;
  float pn[scope*precision];
  double Rn[scope*precision];
  float p1n[scope*precision];
  float c1n[scope*precision];
  float r1n[scope*precision];
};

struct ToTrace choix_prix(struct Market market, struct CDN cdn, float Q1, float qc, float qf)
{
    struct ToTrace all;
    all.Vn.p = 0;
    all.Vn.r = 0;
    all.Vn.c1 = 0;
    all.Vn.p1 = 0;
    all.Vn.r1 = 0;
    double Rmax = 0;
  
    int coeff;
    struct CN Cn;
    double R;
    struct Revenu revenu;
  
    for (int i = 0; i < scope*precision; i++)
    {
        coeff = 1;
        float p = round((0.01 + i*0.01)*precision)/precision;
        printf("NEW PRICE %f - A = %f - g = %f\n", p, market.alph, cdn.storage_price);
        cdn.request_price = p;
        Cn = choix2(market, cdn, Q1, qc, qf);
        if (!Cn.has_1)
        {
            coeff = 0;
        }
        revenu = rcdn(Cn.c1, Q1, market, cdn, Cn.p1, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            Rmax = R;
            all.Vn.p = p;
            all.Vn.r = Rmax;
            all.Vn.c1 = Cn.c1;
            all.Vn.p1 = Cn.p1;
            all.Vn.r1 = Cn.rcp1;
            all.Vn.m1 = Cn.m1;
            all.Vn.q1 = Cn.q1;
        }
        all.pn[i] = p;
        all.Rn[i] = R;
        all.c1n[i] = Cn.c1;
        all.p1n[i] = Cn.p1;
        all.r1n[i] = Cn.rcp1;
    }
  
    return all;
}

struct ToTrace choix_prix_opti(struct Market market, struct CDN cdn, float Q1, float qc, float qf)
{
    struct ToTrace all;
    all.Vn.p = 0;
    all.Vn.r = 0;
    all.Vn.c1 = 0;
    all.Vn.p1 = 0;
    double Rmax = 0;
  
    int coeff;
    struct CN Cn;
    double R;
    struct Revenu revenu;
    
    // TOUR 1
    for (int i = 0; i < scope*10; i++)
    {
        coeff = 1;
        float p = 0.1 + i*0.1;
        //printf("NEW PRICE %f - A = %f - g = %f\n", p, market.alph, cdn.storage_price);
        cdn.request_price = p;
        Cn = choix2(market, cdn, Q1, qc, qf);
      
        if (!Cn.has_1)
        {
            coeff = 0;
        }
        revenu = rcdn(Cn.c1, Q1, market, cdn, Cn.p1, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            Rmax = R;
            all.Vn.p = p;
            all.Vn.r = Rmax;
            all.Vn.c1 = Cn.c1;
            all.Vn.p1 = Cn.p1;
            all.Vn.r1 = Cn.rcp1;
            all.Vn.m1 = Cn.m1;
            all.Vn.q1 = Cn.q1;
        }
    }
    
    // TOUR 2
    float start = all.Vn.p - (scope/100)/2;
    for (int i = 0; i < scope*10; i++)
    {
        coeff = 1;
        float p = round((start + i*0.01)*100)/100;
        //printf("NEW PRICE %f - A = %f - g = %f\n", p, market.alph, cdn.storage_price);
        cdn.request_price = p;
        Cn = choix2(market, cdn, Q1, qc, qf);
      
        if (!Cn.has_1)
        {
            coeff = 0;
        }
        revenu = rcdn(Cn.c1, Q1, market, cdn, Cn.p1, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            Rmax = R;
            all.Vn.p = p;
            all.Vn.r = Rmax;
            all.Vn.c1 = Cn.c1;
            all.Vn.p1 = Cn.p1;
            all.Vn.r1 = Cn.rcp1;
            all.Vn.m1 = Cn.m1;
            all.Vn.q1 = Cn.q1;
        }
    }
    
    // TOUR 3
    start = all.Vn.p - (scope/1000)/2;
    for (int i = 0; i < scope*10; i++)
    {
        coeff = 1;
        float p = round((start + i*0.001)*1000)/1000;
        //printf("NEW PRICE %f - A = %f - g = %f\n", p, market.alph, cdn.storage_price);
        cdn.request_price = p;
        Cn = choix2(market, cdn, Q1, qc, qf);
      
        if (!Cn.has_1)
        {
            coeff = 0;
        }
        revenu = rcdn(Cn.c1, Q1, market, cdn, Cn.p1, coeff, qc, qf);
        R = revenu.rev;
        if (R > Rmax)
        {
            Rmax = R;
            all.Vn.p = p;
            all.Vn.r = Rmax;
            all.Vn.c1 = Cn.c1;
            all.Vn.p1 = Cn.p1;
            all.Vn.r1 = Cn.rcp1;
            all.Vn.m1 = Cn.m1;
            all.Vn.q1 = Cn.q1;
        }
    }
    
    return all;
}

int main()
{
  float Q1 = 20; // valeur arbitraire

  struct Market market;
  market.A = 5; // valeur arbitraire
  market.V = 200; // valeur arbitraire
  market.alph = 0.9; // valeur expérimentale (entre 0.5 et 1, plus on est proche de 1 plus la population est hétérogène)
  float qc = 0.6;
  float qf = 1;
  
  struct CDN cdn;
  cdn.Qc = 80; // valeur arbitraire
  cdn.C = 3; // valeur arbitraire
  cdn.request_price = 1; // valeur arbitraire à trouver
  struct ToTrace results;
  
  float Rmax = 0;
  float pmax = 0;
  float gmax = 0;
  double Rc1max = 0;
  float pc1max = 0;
  float M1max = 0;
  float C1max = 0;
  float Q1max = 0;

  
  #define lissage 61
  // nombre de point de chaque courbe
  #define xmin 10.0
  // valeur minimale de la fenêtre de résultats
  #define delta_x 5
  // écart horizontal entre deux points (xmin + (lissage - 1) * deltax = xmax)
  float p[7][lissage]; // tableau qui rassemble les résultats
  
  for (int i = 0; i < lissage; i++)
  {
    market.V = xmin + delta_x*i;
    Rmax = 0;
    pmax = 0;
    gmax = 0;
    
    results = choix_prix_opti(market, cdn, Q1, qc, qf);
    printf("p = %f - A = %f : R = %f\n", results.Vn.p, market.A, results.Vn.r);
      
    p[0][i] = results.Vn.r;
    p[1][i] = results.Vn.p;
    p[2][i] = results.Vn.r1;
    p[3][i] = results.Vn.p1;
    p[4][i] = results.Vn.m1;
    p[5][i] = results.Vn.c1;
    p[6][i] = results.Vn.q1;
    
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
    sprintf(str, "%f %f %f %f %f %f %f %f\n", xmin + delta_x*i, p[0][i], p[1][i], p[2][i], p[3][i], p[4][i], p[5][i], p[6][i]);
    fputs(str, f);
  }
  
  fclose(f);
  system("python3 tracer_courbes.py data.txt");
  
  
  return 0;
}
