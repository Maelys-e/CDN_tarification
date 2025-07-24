#include "equilibre_opti_de_nash.h"

struct CN;
struct CN
{
  bool has_1;
  float c1;
  float p1;
  double rcp1;
  float m1;
  float q1;
  bool has_2;
  float c2;
  float p2;
  double rcp2;
  float m2;
  float q2;
};

struct Revenu;
struct Revenu
{
  double rev;
  float m1;
  float m2;
  float q1;
  float q2;
};

struct Coeffs;
struct Coeffs
{
  int coeff_1;
  int coeff_2;
};
  
struct CN choix2(struct Market market, struct CDN cdn, float Q1, float Q2, float qc, float qf);

struct Revenu rcdn(float C1, float C2, float Q1, float Q2, struct Market market, struct CDN cdn, float p1, float p2, struct Coeffs coeff, float qc, float qf);
