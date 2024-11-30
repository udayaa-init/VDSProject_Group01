// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Created by Markus Wedler 2014

#ifndef VDSPROJECT_MANAGER_H
#define VDSPROJECT_MANAGER_H

#include <vector>
#include <string>
#include "ManagerInterface.h"


namespace ClassProject {
    struct Node{
        BDD_ID high;
        BDD_ID low;
        BDD_ID top_var;
        BDD_ID id;
        std::string var_name;

        Node(BDD_ID id, std::string name):id(id), var_name(name){};

    };
    class Manager: public ManagerInterface{
        private:
            std::vector<Node> nodeTable;
        public:

            Manager();

            BDD_ID createVar(const std::string &label) override;

            // Adding place holder to compile it
            const BDD_ID &True() override {return NULL;};

            const BDD_ID &False() override {return NULL;};

            bool isConstant(BDD_ID f) override;

            bool isVariable(BDD_ID x) override;

            BDD_ID topVar(BDD_ID f) override ;

            BDD_ID ite(BDD_ID i, BDD_ID t, BDD_ID e) override {return 0;};

            BDD_ID coFactorTrue(BDD_ID f, BDD_ID x) override {return 0;};

            BDD_ID coFactorFalse(BDD_ID f, BDD_ID x) override {return 0;};

            BDD_ID coFactorTrue(BDD_ID f) override {return 0;};

            BDD_ID coFactorFalse(BDD_ID f) override {return 0;};

            BDD_ID and2(BDD_ID a, BDD_ID b) override {return 0;};

            BDD_ID or2(BDD_ID a, BDD_ID b) override {return 0;};

            BDD_ID xor2(BDD_ID a, BDD_ID b) override {return 0;};

            BDD_ID neg(BDD_ID a) override {return 0;};

            BDD_ID nand2(BDD_ID a, BDD_ID b) override {return 0;};

            BDD_ID nor2(BDD_ID a, BDD_ID b) override {return 0;};

            BDD_ID xnor2(BDD_ID a, BDD_ID b) override {return 0;};

            std::string getTopVarName(const BDD_ID &root) override;

            void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) override {return ;};

            void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) override {return ;};

            size_t uniqueTableSize() override {return 0;};

            void visualizeBDD(std::string filepath, BDD_ID &root) override {return ;};

    };

}

#endif
