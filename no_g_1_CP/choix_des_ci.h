#include "equilibre_de_nash.h"

struct CN;
struct CN
{
  bool has_1;
  float c1;
  float p1;
  double rcp1;
  float m1;
  float q1;
};

struct Revenu;
struct Revenu
{
  double rev;
  float m1;
  float q1;
};
  
struct CN choix2(struct Market market, struct CDN cdn, float Q1, float qc, float qf);

struct Revenu rcdn(float C1, float Q1, struct Market market, struct CDN cdn, float p1, int coeff, float qc, float qf);
