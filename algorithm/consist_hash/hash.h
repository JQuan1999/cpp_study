#ifndef CONSISTENT_HASH_H
#define CONSISTENT_HASH_H

#include<stdio.h>
#include<map>
#include<string>
#include<set>

class ConsistentHash
{
public:
    ConsistentHash(int vnum): virtural_node_num(vnum)
    {
        physical_nodes.insert("192.168.1.0");
        physical_nodes.insert("192.168.1.1");
        physical_nodes.insert("192.168.1.2");
        physical_nodes.insert("192.168.1.3");
    }
    ~ConsistentHash(){}
    static uint32_t Hash(std::string key);
    void Initialize();
    void AddNewPhysicalNode(const std::string& nodeIp);
    void DeletePhysicalNode(const std::string& nodeIp);
    std::string GetServerIndex(const std::string& nodeIp);
    void StatisticPerf(std::string& label, int left, int right);
private:
    std::map<uint32_t, std::string> server_nodes;
    std::set<std::string> physical_nodes;
    int virtural_node_num;
};

#endif