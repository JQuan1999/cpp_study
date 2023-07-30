#include"hash.h"
#include <iostream>
#include <sstream>

// 32 位的 Fowler-Noll-Vo 哈希算法
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
uint32_t ConsistentHash::Hash(std::string key) {
    const int p = 16777619;
    uint32_t hash = 2166136261;
    for (int idx = 0; idx < key.size(); ++idx) {
        hash = (hash ^ key[idx]) * p;
    }
    hash += hash << 13;
    hash ^= hash >> 7;
    hash += hash << 3;
    hash ^= hash >> 17;
    hash += hash << 5;
    if (hash < 0) {
        hash = -hash;
    }
    return hash;
}

void ConsistentHash::Initialize()
{
    for(auto& ip: physical_nodes){
        for(int j = 0; j < virtural_node_num; j++){
            std::stringstream nodekey;
            nodekey << ip << "#" << j; // 虚拟节点string
            uint32_t partion = Hash(nodekey.str());
            server_nodes.insert({partion, ip});
        }
    }
}

// 插入一个物理节点 同时插入其关联的所有虚拟节点
void ConsistentHash::AddNewPhysicalNode(const std::string& nodeIp)
{
    for(int j = 0; j < virtural_node_num; ++j){
        std::stringstream nodekey;
        nodekey << nodeIp << "#" << j; // 虚拟节点string
        uint32_t partion = Hash(nodekey.str());
        server_nodes.insert({partion, nodeIp});
    }
}

void ConsistentHash::DeletePhysicalNode(const std::string& nodeIp)
{
    for(int j = 0; j < virtural_node_num; ++j){
        std::stringstream nodekey;
        nodekey << nodeIp << "#" << j; // 虚拟节点string
        uint32_t partion = Hash(nodekey.str());
        server_nodes.erase(partion);
    }
}

// 得到key对应的虚拟节点ip
std::string ConsistentHash::GetServerIndex(const std::string& key)
{
    uint32_t partition = Hash(key);
    auto iter = server_nodes.lower_bound(partition);
    // 沿着环找到第一个大于等于partition的虚拟节点
    if(iter == server_nodes.end()){
        if(server_nodes.empty()){
            std::cout<<"no avaiable nodes"<<std::endl;
        }
        return server_nodes.begin()->second;
    }
    return iter->second;
}

// 给区间[left, right]上的数据找到合适的存储节点
void ConsistentHash::StatisticPerf(std::string& label, int left, int right)
{
    std::map<std::string, int> cnt;
    for(int i = left; i <= right; i++){
        std::string nodeIp = GetServerIndex(std::to_string(i));
        cnt[nodeIp]++;
    }
    int total = right - left + 1;
    std::cout<<"=========="<<label<<"==========\n";
    for(auto& p: cnt){
        std::cout<<"nodeIp: "<<p.first<<" rate: "<<100 * p.second / (total * 1.0) << "%\n";
    }
}