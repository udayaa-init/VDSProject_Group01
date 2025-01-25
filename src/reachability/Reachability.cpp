#include "Reachability.h"
#include <iostream>
namespace ClassProject{

    Reachability::Reachability(int stateSize, int inputSize):
      ReachabilityInterface::ReachabilityInterface(stateSize, inputSize){
        if(stateSize <= 0 || inputSize<0)
            throw std::runtime_error(" !!!!!stateSize must be greater than zero or or inputSize must be greater than or equal to zero!!!!!!");
        // create current state, next state, and input variables
        states = std::vector<BDD_ID>(stateSize, 0);
        inputs = std::vector<BDD_ID>(inputSize, 0);
        nextStates = std::vector<BDD_ID>(stateSize, 0);
        initState = std::vector<BDD_ID>(stateSize, 0);
        transitionFunctions = std::vector<BDD_ID>(stateSize, 0);
        for (unsigned int i = 0; i < stateSize; i++) {
            transitionFunctions[i] = states[i] = createVar(std::to_string(i)); // just like creating s1,s2,s3,...
            nextStates[i] = createVar(std::to_string(i)); // just like creating r1,r2,r3,... and rest of the BDDs will be based on manipualting for these basic varaibles
        }

        for (unsigned int i = 0; i < inputSize; i++) {
            inputs[i] = createVar(std::to_string(i));
        }
        //For the default initial state, all bits are assumed to be set to false.
        setInitState(std::vector<bool>(stateSize, false));

        // default transition function for each state bit is the identity function. 
        //after calling the constructor, the only reachable state should be the initial state.
        //Each state variable has a transition function.
        setTransitionFunctions(std::vector<BDD_ID>(stateSize, 0));
    }    

    const std::vector<BDD_ID> &Reachability::getStates() const{
        return states;
    }

    const std::vector<BDD_ID> &Reachability::getInputs() const{
        return inputs;
    }

    bool Reachability::isReachable(const std::vector<bool> &stateVector){
        int val = stateDistance(stateVector);
        std::cout<<"the distance is "<<val<<std::endl;
        return val >= 0;
    }

    int Reachability::stateDistance(const std::vector<bool> &stateVector){
        if(stateVector.size() != states.size())
            throw std::runtime_error("Vector size does not match state variables");

        if(stateVector == getInitState())
            return 0;

        BDD_ID tau, cs0, cr, cr_it, nxtImg, Img;
        // compute the BDD for the transition relation
        tau = characteristicFunction(nextStates, transitionFunctions);        
        
        // compute the characteristic function of the initial state (among all the states, is the initial state is also possible among the possible states)
        cs0 = characteristicFunction(states, initState); 

        cr_it = cs0;
        int distance =0;
        //fixed point interation 
        do{
            cr = cr_it;
            nxtImg = existentialQuantification(existentialQuantification(and2(tau,cr), states), inputs); // Image in terms of the next state r1,r2,r3...

            // remane the next state to present state. (we need a mapping function from r's to s's. i.e r_i <==> s_i ---> (x_i xnor s_i)[ iff operator, if r's true, s's should be true and if r's is false, s's shoudl be false ] and later we can quantify it.)
            Img = True();
            for(int i=0; i<states.size(); i++){
               //  (we xnor the states and images (s1 xnor r1)(s2 xnor r2)(s3 xnor r3)...)
            //Xnoring and adding gives us all the variables interms of s1s2 and !s1!s2 and there anded paris with other variables.eg. s1s2r1r2 + s1r1!s2!r2+ !s1!r1s2r2 + !s1!r1!s2!r2 that's all we need
                Img = this->and2(Img, this->xnor2(this->states[i], this->nextStates[i]));
            }
            //Img = existentialQuantification(Img, nextStates); //// We need image in terms of the s1,s2,s3... here Img here( so we xnor the states and images (s1 xnor r1)(s2 xnor r2)(s3 xnor r3)...)
            Img = existentialQuantification(and2(Img, nxtImg), nextStates); // We anded both img and nextImg and removed the next state. so we all have img in term of s1,s22,s3...// existentialQuantification(existentialQuantification(and2(Img, nxtImg), nextStates),inputs);
            cr_it = or2(cr, Img);
            distance ++;

            //chack if the provided is in recheable state space. How?? we have state the is recheable from theinitial state we can xnor with the curent state 
            BDD_ID temp = Img;
            for(int i=0; i<stateVector.size(); i++){
                BDD_ID sv = stateVector[i] ? True() : False();
                temp = this->and2(temp, this->xnor2(sv, states[i]));
            }

            // existential quantification with respect to the next state variables
            temp = existentialQuantification(temp, states);    

            if(temp == True()) return distance;

        }while(cr_it != cr);
        return -1;
    }
    void Reachability::setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions){

        if(transitionFunctions.size() != states.size())
            throw std::runtime_error("Vector size does not match state variables");

        for(BDD_ID id : transitionFunctions)
            if(id < 0 || id > uniqueTableSize())
                throw std::runtime_error("Unknown ID found");

        this->transitionFunctions = transitionFunctions;
    }

    void Reachability::setInitState(const std::vector<bool> &stateVector){
        if(stateVector.size() != states.size())
            throw std::runtime_error("Vector size does not match state variables");
        
        // If initial state is set and transisiton function is not set and only recheable state should be inital state
        if(this->transitionFunctions == std::vector<BDD_ID>(transitionFunctions.size(), 0))
        {
            for(int i=0; i<transitionFunctions.size(); i++){
                this->transitionFunctions[i] = stateVector[i] ? True() : False();
            } 
        }
        for(int i=0; i<stateVector.size(); i++){
            this->initState[i] = stateVector[i] ? True() : False();
        } 
         
    }

    const std::vector<bool> Reachability::getInitState(){
         std::vector<bool> initalState = std::vector<bool>(this->initState.size(),1);
         for(unsigned int i=0;i<this->initState.size();i++ ){
            initalState[i] =( initState[i] == True()) ? true : false;
         }
         return initalState;
    }

    BDD_ID Reachability::characteristicFunction(std::vector<BDD_ID> f1, std::vector<BDD_ID> f2)
    {
        BDD_ID fn = True();
        for(int i=0; i<f1.size(); i++){
            fn = and2( fn, xnor2(f1[i], f2[i]));
        }
        return fn;
    }

    BDD_ID Reachability::existentialQuantification(const BDD_ID &f, const std::vector<BDD_ID> &v) {
    BDD_ID temp = f;
    for (int64_t i = v.size() - 1; i >= 0; --i) {
        temp = or2(coFactorTrue(temp, v[i]), coFactorFalse(temp, v[i]));
    }
    return temp;
    }
}