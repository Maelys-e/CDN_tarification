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
  float omeg;
};

struct CDN
{
  float Qc;
  float C;
  float request_price;
};

double rcp(float pa, float pb, float Ci[], float Qfi[], struct Market market, struct CDN cdn, int i)
{
    float bet = 1-market.alph;
    float Qfa = Qfi[i];                 float Qfb = Qfi[3-i];
    float Ca = Ci[i];                   float Cb = Ci[3-i];
    float Aa = pow(Ca/market.V, bet);   float Ab = pow(Cb/market.V, bet);
    float Qa = cdn.Qc*Aa + Qfa*(1-Aa);  float Qb = cdn.Qc*Ab + Qfb*(1-Ab);
    float Ma;
  
    if (pb != 0)
    {
        float Ba = (Qa/pow(pa, 2))+((Qa+Qb)*((1-market.omeg)/2)/pow(pa+pb, 2));
        Ma = fmaxf(pa*Ba/(1+Ba+(Qb/pow(pb, 2))), 0);
    }else{
        Ma = fmaxf((Qa/pa)/(1+(Qa/pow(pa, 2))), 0);
    }
  
    float D = market.A*(pow(market.V, bet)/bet);
    double rcpa = Ma * D * (1-(cdn.request_price/pa));
    return rcpa;
}

struct CPtuple
{
    float pk;
    double R;
};

struct CPtuple maxrcp(float p, float Ci[], float Qfi[], struct Market market, struct CDN cdn, int i) 
{
    double R;
    float pk = 0;
    double Max = 0;
    float pmax = 0;

    // TOUR 1
    for (int k = 0; k < (int)(prixMax * 10); k++)
    {
        pk = round((0.1 + k*0.1) * 10)/10;
        R = rcp(pk, p, Ci, Qfi, market, cdn, i);
        if (R > Max)
        {
            pmax = pk;
            Max = R;
        }
    }
  
    // TOUR 2
    float start = pmax - (prixMax/10)/2;
    for (int k = 0; k < (int)(prixMax * 10); k++)
    {
        pk = round((start + k*0.01) * 100)/100;
        R = rcp(pk, p, Ci, Qfi, market, cdn, i);
        if (R > Max)
        {
            pmax = pk;
            Max = R;
        }
    }
  
    // TOUR 3
    start = pmax - (prixMax/100)/2;
    for (int k = 0; k < (int)(prixMax * 10); k++)
    {
        pk = round((start + k*0.001) * 1000)/1000;
        R = rcp(pk, p, Ci, Qfi, market, cdn, i);
        if (R > Max)
        {
            pmax = pk;
            Max = R;
        }
    }
  
    // TOUR 4
    start = pmax - (prixMax/1000)/2;
    for (int k = 0; k < (int)(prixMax * 10); k++)
    {
        pk = round((start + k*0.0001) * 10000)/10000;
        R = rcp(pk, p, Ci, Qfi, market, cdn, i);
        if (R > Max)
        {
            pmax = pk;
            Max = R;
        }
    }
    
    struct CPtuple tab;
    tab.pk = pmax;
    tab.R = Max;
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
    
struct Nash nash2(float C1, float C2, float Q1, float Q2, struct Market market, struct CDN cdn)
{
    float p1 = 1;
    float p1_before[20] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    float p2 = 1;
    float p2_before[20] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  
    struct CPtuple tab;
    float Ci[3] = {0, C1, C2};
    float Qfi[3] = {0, Q1, Q2};
    int n = 1;
    float S;
  
    while (n)
    {
        for (int i = 19; i > 0; i--)
        {
          p2_before[i] = p2_before[i-1];
        }
        p2_before[0] = p2;
        for (int i = 19; i > 0; i--)
        {
          p1_before[i] = p1_before[i-1];
        }
        p1_before[0] = p1;
      
        tab = maxrcp(p1, Ci, Qfi, market, cdn, 2);
        p2 = tab.pk;
        tab = maxrcp(p2, Ci, Qfi, market, cdn, 1);
        p1 = tab.pk;
        
        for (int j = 0; j < 20; j++)
        {
          if (round(fmaxf(p1 - p1_before[j], p1_before[j]-p1)*1000)/1000 < 0.01 && round(fmaxf(p2 - p2_before[j], p2_before[j]-p2)*1000)/1000 < 0.01) // test au centième près
          {
            n = 0;
            S = 0;
            for (int k = 0; k < j+1; k++)
            {
              S += p1_before[k];
            }
            p1 = round((S/(j+1))*1000)/1000;
            S = 0;
            for (int k = 0; k < j+1; k++)
            {
              S += p2_before[k];
            }
            p2 = round((S/(j+1)) * 1000)/1000;
            break;
          }
        }
    }
  
    struct Nash values;
    values.rCP1 = rcp(p1, p2, Ci, Qfi, market, cdn, 1);
    values.rCP2 = rcp(p2, p1, Ci, Qfi, market, cdn, 2);
    float SR;
    SR = 0; // seuil de rentabilité (0 si inutilisé)
  
    if (p1 != 0 && values.rCP1 > SR) // si le CP1 n'est pas rentable
    {
        values.has_1 = true;
        values.value_1 = p1;
    }else{
        values.has_1 = false;
        values.value_1 = 0;
    }
    if (p2 != 0 && values.rCP2 > SR) // si le CP2 n'est pas rentable
    {
        values.has_2 = true;
        values.value_2 = p2;
    }else{
        values.has_2 = false;
        values.value_2 = 0;
    }
  
    return values;
}
    
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
    system("python3 tracer_courbes.py");
    
    return 0;
}
*/

