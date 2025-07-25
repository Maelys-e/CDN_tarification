#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

static int prixMax = 150;
#define precision 100

struct Market
{
  float A;
  float V;
  float alph;
  float storage_cost;
};

struct CDN
{
  float Qc;
  float C;
  float request_price;
};

struct CPrev
{
  float rCP;
  float M;
  float Q;
};

struct CPrev rcp(float pa, float Ci, float Qfi, struct Market market, struct CDN cdn)
{
    float Qfa = Qfi;
    float Ca = Ci;
    float bet = 1-market.alph;
    float D = market.A*(pow(market.V, bet)/bet);
    float Aa = pow(Ca/market.V, bet);
    float Qa = cdn.Qc*Aa + Qfa*(1-Aa);
    float Ma;
    if (pa != 0)
    {
        Ma = fmaxf((Qa/pa)/(1+(Qa/pow(pa, 2))), 0);
    }else{
        Ma = 0;
    }
    double rcpa = Ma * D * (1-(cdn.request_price/pa));
    struct CPrev result;
    result.rCP = rcpa;
    result.M = Ma;
    result.Q = Qa;
    return result;
}

struct CPtuple
{
    float pk;
    double rCP;
    int has_1;
    float m1;
    float q1;
};

struct CPtuple maxrcp(float p, float Ci, float Qfi, struct Market market, struct CDN cdn) 
{
    double R;
    float pk = 0;
    double Max = 0;
    float pmax = 0;
    float mmax = 0;
    float qmax = 0;
    int coeff = 0;
    struct CPrev rev;
    
    for (int k = 0; k < (int)(prixMax * 10); k++)
    {
        pk = round((0.1 + k*0.1) * 10)/10;
        rev = rcp(pk, Ci, Qfi, market, cdn);
        R = rev.rCP;
        if (R > Max)
        {
            pmax = pk;
            Max = R;
            mmax = rev.M;
            qmax = rev.Q;
        }
    }
    
    float start = pmax - (prixMax/10)/2;
    for (int k = 0; k < (int)(prixMax * 10); k++)
    {
        pk = round((start + k*0.01) * 100)/100;
        rev = rcp(pk, Ci, Qfi, market, cdn);
        R = rev.rCP;
        if (R > Max)
        {
            pmax = pk;
            Max = R;
            mmax = rev.M;
            qmax = rev.Q;
        }
    }
    
    start = pmax - (prixMax/100)/2;
    for (int k = 0; k < (int)(prixMax * 10); k++)
    {
        pk = round((start + k*0.001) * 1000)/1000;
        rev = rcp(pk, Ci, Qfi, market, cdn);
        R = rev.rCP;
        if (R > Max)
        {
            pmax = pk;
            Max = R;
            mmax = rev.M;
            qmax = rev.Q;
        }
    }
    
    start = pmax - (prixMax/1000)/2;
    for (int k = 0; k < (int)(prixMax * 10); k++)
    {
        pk = round((start + k*0.0001) * 10000)/10000;
        rev = rcp(pk, Ci, Qfi, market, cdn);
        R = rev.rCP;
        if (R > Max)
        {
            pmax = pk;
            Max = R;
            mmax = rev.M;
            qmax = rev.Q;
        }
    }
    
    struct CPtuple tab;
    tab.pk = pmax;
    tab.rCP = Max;
    if (pmax > 0)
    {
      coeff = 1;
    }
    tab.has_1 = coeff;
    tab.m1 = mmax;
    tab.q1 = qmax;
    return(tab);
}

struct Nash
{
  bool has_1;
  float value_1;
  double rCP1;
  bool has_2;
  float value_2;
  double rCP2;
};
  
    
/*
int main()
{
    float Q1 = 1.2; // valeur arbitraire
    float Q2 = 1.5; // valeur arbitraire

    struct Market market;
    market.A = 4; // valeur arbitraire
    market.V = 200; // valeur arbitraire
    market.alph = 0.8; // valeur expérimentale (entre 0.5 et 1, plus on est proche de 1 plus la population est hétérogène)
    market.storage_cost = 0.6; // valeur arbitraire
  
    struct CDN cdn;
    cdn.Qc = 2; // valeur arbitraire
    cdn.C = 3; // valeur arbitraire
    cdn.request_price = 1; // valeur arbitraire
  
    struct Nash result = nash2(4, 4, Q1, Q2, market, cdn);
    printf("p1 : %f, p2 : %f\n", result.value_1, result.value_2);
    
    FILE *f;
    char str[4454];
    f = fopen("data0.txt", "w");
 
    if(f == NULL){
      printf("Erreur lors de l'ouverture d'un fichier");
      exit(1);
    }
    
    float Cir[3] = {0, 2, 1};
    float Cis[3] = {0, 1.5, 1.5};
    float Qfi[3] = {0, Q1, Q2};        
        
    for (int i = 0; i < (int)(301); i++)
    {
      float pa = i*0.1;
      float pb = 12;
      float R = rcp(pa, pb, Cir, Qfi, market, cdn, 1);
      float S = rcp(pa, pb, Cis, Qfi, market, cdn, 1);
      sprintf(str, "%f %f %f\n", pa, R, S);
      fputs(str, f);
    }
  
    fclose(f);
    system("python3 tracer_equilibres.py");
    
    return 0;
}
*/

