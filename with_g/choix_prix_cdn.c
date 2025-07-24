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
  float Q1 = 1.2; // valeur arbitraire
  float Q2 = 1.5; // valeur arbitraire

  float p[13][30];

  struct Market market;
  market.A = 4; // valeur arbitraire
  market.V = 200; // valeur arbitraire
  market.alph = 0.8; // valeur expérimentale (entre 0.5 et 1, plus on est proche de 1 plus la population est hétérogène)
  market.storage_cost = 0.6; // valeur arbitraire
  float qc = 0.6;
  float qf = 1;
  
  struct CDN cdn;
  cdn.Qc = 2; // valeur arbitraire
  cdn.C = 3; // valeur arbitraire
  cdn.storage_price = 0.5; // valeur arbitraire
  cdn.request_price = 1; // valeur arbitraire à trouver
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
  
  //market.A = 17;
  for (int i = 0; i < 30; i++)
  {
    printf("%d\n", i);
    market.V = 10.0+10*i;
    Rmax = 0;
    pmax = 0;
    gmax = 0;
    
    /*
    Cn = choix2(market, cdn, Q1, Q2, qc, qf);
    struct Coeffs coeff;
    if (Cn.has_1)
        {
            coeff.coeff_1 = 1;
        }else{
            coeff.coeff_1 = 0;
        }
        if (Cn.has_2)
        {
            coeff.coeff_2 = 1;
        }else{
            coeff.coeff_2 = 0;
        }
    struct Revenu revenu = rcdn(Cn.c1, Cn.c2, Q1, Q2, market, cdn, Cn.p1, Cn.p2, coeff, qc, qf);
    */
    /*
    cdn.storage_price = 5+0.4*i;
    struct CN Cn;
    results = choix_prix_opti(market, cdn, Q1, Q2, qc, qf);
    printf("gCDN = %f - p = %f - R = %f\n", cdn.storage_price, results.Vn.p, results.Vn.r);
    
    p[0][i] = results.Vn.r;
    p[1][i] = results.Vn.r1;
    p[2][i] = results.Vn.r2;
    p[3][i] = results.Vn.p1;
    p[4][i] = results.Vn.p2;
    p[5][i] = results.Vn.c1;
    p[6][i] = results.Vn.c2;
    p[7][i] = results.Vn.m1;
    p[8][i] = results.Vn.m2;
    p[9][i] = results.Vn.q1;
    p[10][i] = results.Vn.q2;
    p[11][i] = results.Vn.p;
    */
    
    
    //TOUR 1
    for (int j = 0; j < 31; j++)
    {
      cdn.storage_price = 0 + 10*j;
      results = choix_prix_opti(market, cdn, Q1, Q2, qc, qf);
      /*
      for (int k = 0; k < scope*precision; k++)
      {
        p[j][k] = results.Rn[k];
      }
      */
      //p[0][j] = results.Vn.p;
      //p[1][j] = results.Vn.r;
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
      
      //p[i][j] = Rmax;
    }
    
    
    float start = gmax - 10/(1*1);
    //TOUR 2
    for (int j = 0; j < 10; j++)
    {
      cdn.storage_price = start + 2*j + 0;
      results = choix_prix_opti(market, cdn, Q1, Q2, qc, qf);
      /*
      for (int k = 0; k < scope*precision; k++)
      {
        p[j][k] = results.Rn[k];
      }
      */
      //p[0][j] = results.Vn.p;
      //p[1][j] = results.Vn.r;
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
      /*
      for (int k = 0; k < scope*precision; k++)
      {
        p[j][k] = results.Rn[k];
      }
      */
      //p[0][j] = results.Vn.p;
      //p[1][j] = results.Vn.r;
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
  
  //printf("Si V = %f, A = %f, alpha = %f, gamma = %f \nAlors on a l'équilibre :\n p = %f, C1 = %f, C2 = %f, p1 = %f, p2 = %f\n", market.V, market.A, market.alph, market.storage_cost, results.Vn.p, results.Vn.c1, results.Vn.c2, results.Vn.p1, results.Vn.p2);
  
  FILE *f;
  char str[4454];
  f = fopen("data.txt", "w");
 
  if(f == NULL){
    printf("Erreur lors de l'ouverture d'un fichier");
    exit(1);
  }
  
  for (int i = 0; i < (int)(30); i++)
  {
    sprintf(str, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", 10.0+10*i, p[0][i], p[1][i], p[2][i], p[3][i], p[4][i], p[5][i], p[6][i], p[7][i], p[8][i], p[9][i], p[10][i], p[11][i], p[12][i]);
    fputs(str, f);
  }
  
  fclose(f);
  system("python3 tracer_selon_alpha.py data.txt");
  
  //for (int i = 0; i < (int)(sizeof(results.pn)/sizeof(results.pn[0])); i++)
  //{
    //sprintf(str, "%f %f %f %f %f %f %f %f\n", results.pn[i],results.Rn[i], results.c1n[i], results.c2n[i], results.p1n[i], results.p2n[i], results.r1n[i], results.r2n[i]);
    //fputs(str, f);
  //}
  
  //fclose(f);
  
  //system("python3 tracer_courbes.py data.txt");
  
  return 0;
}
