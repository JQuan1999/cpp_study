#include"hash.h"
int main()
{
    ConsistentHash h1(1);
    h1.Initialize();
    std::string label = "No virtual node";
    h1.StatisticPerf(label, 0, 65535);

    ConsistentHash h2(32);
    h2.Initialize();
    label = "virtual nodes num equal 32";
    h2.StatisticPerf(label, 0, 65535);
    return 0;
}