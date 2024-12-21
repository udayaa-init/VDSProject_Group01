// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Created by Markus Wedler 2014

#ifndef VDSPROJECT_MANAGER_H
#define VDSPROJECT_MANAGER_H

#include <vector>
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
    
    struct Hasher {
        std::size_t operator()(const key& key) const {
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
       
        std::unordered_map<key, BDD_ID, Hasher> uniqueTable;
        std::unordered_map<key, BDD_ID, Hasher> computeTable;
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
    };

}

#endif
