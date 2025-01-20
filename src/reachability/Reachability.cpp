#include "Reachability.h"

namespace ClassProject{

    Reachability::Reachability(unsigned int stateSize, unsigned int inputSize): 
      states(stateSize, 0),
      inputs(inputSize, 0),
      nextStates(stateSize, 0),
      initState(stateSize, 0),
      transitionFunctions(stateSize, 0),
      ReachabilityInterface::ReachabilityInterface(stateSize, inputSize){
        if(stateSize <= 0)
            throw std::runtime_error(" !!!!!stateSize must be greater than zero !!!!!!");
        // create current state, next state, and input variables
        for (unsigned int i = 0; i < stateSize; i++) {
            transitionFunctions[i] = states[i] = createVar(std::to_string(i));
            nextStates[i] = createVar(std::to_string(i));
        }

        for (unsigned int i = 0; i < inputSize; i++) {
            inputs[i] = createVar(std::to_string(i));
        }
    }    

    const std::vector<BDD_ID> &Reachability::getStates() const{
        return states;
    }

    const std::vector<BDD_ID> &Reachability::getInputs() const{
        return inputs;
    }

    bool Reachability::isReachable(const std::vector<bool> &stateVector){
        throw std::runtime_error("pureVirtualFunction is not implemented yet.");
    }

    int Reachability::stateDistance(const std::vector<bool> &stateVector){
        throw std::runtime_error("pureVirtualFunction is not implemented yet.");
    }
    void Reachability::setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions){
        throw std::runtime_error("pureVirtualFunction is not implemented yet.");
    }

    void Reachability::setInitState(const std::vector<bool> &stateVector){
        if(stateVector.size() != states.size())
            throw std::runtime_error("Vector size does not match state variables");

        for(int i=0; i<stateVector.size(); i++){
            this->initState[i] = stateVector[i] ? True() : False();
    }
}