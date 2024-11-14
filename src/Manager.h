// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Created by Markus Wedler 2014

#ifndef VDSPROJECT_MANAGER_H
#define VDSPROJECT_MANAGER_H

#include "ManagerInterface.h"

namespace ClassProject {

    class Manager: public ManagerInterface{

        public:
            BDD_ID createVar(const std::string &label) override;

            // Adding place holder to compile it
            const BDD_ID &True() override {return NULL;};

            const BDD_ID &False() override {return NULL;};

            bool isConstant(BDD_ID f) override {return 0;};

            bool isVariable(BDD_ID x) override {return 0;};

            BDD_ID topVar(BDD_ID f) override {return 0;};

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

            std::string getTopVarName(const BDD_ID &root) override {return 0;};

            void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) override {return ;};

            void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) override {return ;};

            size_t uniqueTableSize() override {return 0;};

            void visualizeBDD(std::string filepath, BDD_ID &root) override {return ;};

    };

}

#endif
