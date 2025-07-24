struct Nash;
struct Nash
{
  bool has_1;
  float value_1;
  double rCP1;
  bool has_2;
  float value_2;
  double rCP2;
};

struct Market;
struct Market
{
  float A;
  float V;
  float alph;
  float storage_cost;
  float omeg;
};

struct CDN;
struct CDN
{
  float Qc;
  float C;
  float request_price;
};
  
struct Nash nash2(float C1, float C2, float Q1, float Q2, struct Market market, struct CDN cdn);
