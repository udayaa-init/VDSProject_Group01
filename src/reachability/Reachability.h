#ifndef VDSPROJECT_REACHABILITY_H
#define VDSPROJECT_REACHABILITY_H

#include "ReachabilityInterface.h"

namespace ClassProject {

   class Reachability : public ReachabilityInterface {

    // Adding place holder for pure virtual functions
    public:
    Reachability(int param) : ReachabilityInterface(param) {
        // Derived class constructor body
    }

    std::vector<BDD_ID> &getStates() const override {
        throw std::runtime_error("pureVirtualFunction is not implemented yet.");
    }

    std::vector<BDD_ID> &getInputs() const override {
        throw std::runtime_error("pureVirtualFunction is not implemented yet.");
    }

    bool isReachable(const std::vector<bool> &stateVector) override {
        throw std::runtime_error("pureVirtualFunction is not implemented yet.");
    }

    int stateDistance(const std::vector<bool> &stateVector) override {
        throw std::runtime_error("pureVirtualFunction is not implemented yet.");
    }

    void setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions) override {
        throw std::runtime_error("pureVirtualFunction is not implemented yet.");
    }

    void setInitState(const std::vector<bool> &stateVector) override {
        throw std::runtime_error("pureVirtualFunction is not implemented yet.");
    }

   };

}
#endif
