struct Market;
struct Market
{
  float A;
  float V;
  float alph;
  float storage_cost;
};

struct CDN;
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

struct CPtuple;
struct CPtuple
{
    float pk;
    double rCP;
    int has_1;
    float m1;
    float q1;
};

struct CPtuple maxrcp(float p, float Ci, float Qfi, struct Market market, struct CDN cdn);
