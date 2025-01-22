#ifndef VDSPROJECT_REACHABILITY_H
#define VDSPROJECT_REACHABILITY_H

#include "ReachabilityInterface.h"

namespace ClassProject {

   class Reachability : public ReachabilityInterface {

    private:
    std::vector<BDD_ID> states;
    std::vector<BDD_ID> inputs;
    std::vector<BDD_ID> nextStates;
    std::vector<BDD_ID> initState;
    std::vector<BDD_ID> transitionFunctions;
    // Adding place holder for pure virtual functions
    public:
    explicit Reachability(unsigned int stateSize, unsigned int inputSize = 0);
    Reachability(int param) : ReachabilityInterface(param) {
        // Derived class constructor body
    }

    const std::vector<BDD_ID> &getStates() const override ;

    const std::vector<BDD_ID> &getInputs() const override;

    bool isReachable(const std::vector<bool> &stateVector) override;

    int stateDistance(const std::vector<bool> &stateVector) override;

    void setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions) override;

    void setInitState(const std::vector<bool> &stateVector) override;

    const std::vector<bool> getInitState();
    BDD_ID characteristicFunction(std::vector<BDD_ID>, std::vector<BDD_ID>);
    BDD_ID existentialQuantification(const BDD_ID&, const std::vector<BDD_ID>&);
   };

}
#endif
