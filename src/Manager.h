// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Created by Markus Wedler 2014

#ifndef VDSPROJECT_MANAGER_H
#define VDSPROJECT_MANAGER_H

#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include "ManagerInterface.h"

namespace ClassProject
{
    typedef std::tuple<BDD_ID, BDD_ID, BDD_ID> key;
    struct Node
    {
        BDD_ID high;
        BDD_ID low;
        BDD_ID top_var;
        BDD_ID id;
        std::string var_name;

        Node(int id, int top_var, int high, int low, std::string var_name)
            : id(id), top_var(top_var), high(high), low(low), var_name(var_name) {}
    };

    struct Hasher
    {
        std::size_t operator()(const key &key) const
        {
            std::size_t hash_key = 0;

            boost::hash_combine(hash_key, boost::hash_value(std::get<0>(key)));
            boost::hash_combine(hash_key, boost::hash_value(std::get<1>(key)));
            boost::hash_combine(hash_key, boost::hash_value(std::get<2>(key)));

            return hash_key;
        }
    };

    class Manager : public ManagerInterface
    {
    private:
        std::vector<Node> nodeTable;

        // LRU Cache for unique table
        size_t uniqueTableCapacity = 10000000; // Set capacity for uniqueTable
        std::list<key> uniqueAccessOrder;  // Access order (most recently used at front)
        std::unordered_map<key, std::pair<BDD_ID, std::list<key>::iterator>, Hasher> uniqueTable;

        // LRU Cache for compute table
        size_t computeTableCapacity = 10000000; // Set capacity for computeTable
        std::list<key> computeAccessOrder;  // Access order (most recently used at front)
        std::unordered_map<key, std::pair<BDD_ID, std::list<key>::iterator>, Hasher> computeTable;

    public:
        Manager();

        BDD_ID createVar(const std::string &label) override;

        BDD_ID createNode(const std::string &label, const BDD_ID &top,
                          const BDD_ID &high, const BDD_ID &low);

        // Adding place holder to compile it
        const BDD_ID &True() override;

        const BDD_ID &False() override;

        bool isConstant(BDD_ID f) override;

        bool isVariable(BDD_ID x) override;

        BDD_ID topVar(BDD_ID f) override;

        BDD_ID ite(BDD_ID i, BDD_ID t, BDD_ID e) override;

        BDD_ID coFactorTrue(BDD_ID f, BDD_ID x) override;

        BDD_ID coFactorFalse(BDD_ID f, BDD_ID x) override;

        BDD_ID coFactorTrue(BDD_ID f) override;

        BDD_ID coFactorFalse(BDD_ID f) override;

        BDD_ID and2(BDD_ID a, BDD_ID b) override;

        BDD_ID or2(BDD_ID a, BDD_ID b) override;

        BDD_ID xor2(BDD_ID a, BDD_ID b) override;

        BDD_ID neg(BDD_ID a) override;

        BDD_ID nand2(BDD_ID a, BDD_ID b) override;

        BDD_ID nor2(BDD_ID a, BDD_ID b) override;

        BDD_ID xnor2(BDD_ID a, BDD_ID b) override;

        std::string getTopVarName(const BDD_ID &root) override;

        void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) override;

        void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) override;

        size_t uniqueTableSize() override;

        void visualizeBDD(std::string filepath, BDD_ID &root) override { return; };

        BDD_ID find_or_add_unique_table(BDD_ID, BDD_ID r_low, BDD_ID r_high, std::string te);

        // Insert into unique table with LRU eviction
        void insertUniqueTable(const key &k, BDD_ID id)
        {
            // If key already exists, update access order
            if (uniqueTable.find(k) != uniqueTable.end())
            {
                uniqueAccessOrder.erase(uniqueTable[k].second);
            }
            else if (uniqueTable.size() >= uniqueTableCapacity)
            {
                // Evict the least recently used item
                const auto &lru_key = uniqueAccessOrder.back();
                uniqueTable.erase(lru_key);
                uniqueAccessOrder.pop_back();
            }

            // Insert or update the item and move to the front of access order
            uniqueAccessOrder.push_front(k);
            uniqueTable[k] = {id, uniqueAccessOrder.begin()};
        }

        // Insert into compute table with LRU eviction
        void insertComputeTable(const key &k, BDD_ID id)
        {
            if (computeTable.find(k) != computeTable.end())
            {
                computeAccessOrder.erase(computeTable[k].second);
            }
            else if (computeTable.size() >= computeTableCapacity)
            {
                const auto &lru_key = computeAccessOrder.back();
                computeTable.erase(lru_key);
                computeAccessOrder.pop_back();
            }

            computeAccessOrder.push_front(k);
            computeTable[k] = {id, computeAccessOrder.begin()};
        }

        // Find in unique table
        std::optional<BDD_ID> findUniqueTable(const key &k)
        {
            if (uniqueTable.find(k) == uniqueTable.end())
            {
                return std::nullopt; // Not found
            }

            // Update access order (move to front)
            uniqueAccessOrder.erase(uniqueTable[k].second);
            uniqueAccessOrder.push_front(k);
            uniqueTable[k].second = uniqueAccessOrder.begin();
            return uniqueTable[k].first;
        }

        // Find in compute table
        std::optional<BDD_ID> findComputeTable(const key &k)
        {
            if (computeTable.find(k) == computeTable.end())
            {
                return std::nullopt; // Not found
            }

            computeAccessOrder.erase(computeTable[k].second);
            computeAccessOrder.push_front(k);
            computeTable[k].second = computeAccessOrder.begin();
            return computeTable[k].first;
        }
    };

}

#endif
