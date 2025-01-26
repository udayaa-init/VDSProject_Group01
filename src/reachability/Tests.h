#ifndef VDSPROJECT_REACHABILITY_TESTS_H
#define VDSPROJECT_REACHABILITY_TESTS_H

#include <gtest/gtest.h>
#include "Reachability.h"

using namespace ClassProject;

struct ReachabilityTest : testing::Test {

    std::unique_ptr<ClassProject::ReachabilityInterface> fsm2 = std::make_unique<ClassProject::Reachability>(2,0);

    std::vector<BDD_ID> stateVars2 = fsm2->getStates();
    std::vector<BDD_ID> transitionFunctions;

};

TEST_F(ReachabilityTest, HowTo_Example) { /* NOLINT */

    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);

    transitionFunctions.push_back(fsm2->neg(s0)); // s0' = not(s0)
    transitionFunctions.push_back(fsm2->neg(s1)); // s1' = not(s1)
    fsm2->setTransitionFunctions(transitionFunctions);

    fsm2->setInitState({false,false});

    ASSERT_TRUE(fsm2->isReachable({false, false}));
    ASSERT_FALSE(fsm2->isReachable({false, true}));
    ASSERT_FALSE(fsm2->isReachable({true, false}));
    ASSERT_TRUE(fsm2->isReachable({true, true}));
}

TEST_F(ReachabilityTest, feedbackTest) { /* NOLINT */
    std::unique_ptr<ClassProject::ReachabilityInterface> fsm1 = std::make_unique<ClassProject::Reachability>(1, 0);
    //when no transition function is set and only iniital state is set
    ASSERT_NO_THROW(fsm1->setInitState({true}));
    ASSERT_FALSE(fsm1->isReachable({false}));
    ASSERT_TRUE(fsm1->isReachable({true}));
}

TEST_F(ReachabilityTest, constructorTest) { /* NOLINT */
    // should throw exception when state siz eis 0 or less than zero
    EXPECT_THROW(std::make_unique<ClassProject::Reachability>(0, 0), std::runtime_error);
    EXPECT_THROW(std::make_unique<ClassProject::Reachability>(-2, 0), std::runtime_error);
    EXPECT_THROW(std::make_unique<ClassProject::Reachability>(1, -1), std::runtime_error);
}

TEST_F(ReachabilityTest, exceptionTest) {
    std::unique_ptr<ClassProject::ReachabilityInterface> fsm2 =
        std::make_unique<ClassProject::Reachability>(2,0);
    EXPECT_THROW(fsm2->setTransitionFunctions({true,true,true}),
                std::runtime_error);
    EXPECT_THROW(fsm2->setInitState({true, true, true}), std::runtime_error);
    EXPECT_THROW(fsm2->isReachable({true, true, true}), std::runtime_error);
    EXPECT_THROW(fsm2->stateDistance({true, true, true}), std::runtime_error);
}

TEST_F(ReachabilityTest, defaultState) { /* NOLINT */
    std::unique_ptr<ClassProject::ReachabilityInterface> fsm2 =
        std::make_unique<ClassProject::Reachability>(2,0);
    ASSERT_TRUE(fsm2->isReachable({false,false}));
    ASSERT_FALSE(fsm2->isReachable({true,false}));
    ASSERT_FALSE(fsm2->isReachable({false,true}));
    ASSERT_FALSE(fsm2->isReachable({true,true}));
}

TEST_F(ReachabilityTest, inputTest) {
    std::unique_ptr<ClassProject::ReachabilityInterface> fsm2_1 =
    std::make_unique<ClassProject::Reachability>(2,1);
    std::vector<BDD_ID> inputVars = fsm2_1->getInputs();
    std::vector<BDD_ID> stateVars = fsm2_1->getStates();
    std::vector<BDD_ID> transitionFunctions;
    auto s1 = stateVars.at(0);
    auto s2 = stateVars.at(1);
    auto x1 = inputVars.at(0);
    // 2 bit counter  example up to 3 (00,01,10 but not 11)
    //d1 =  x1.(s1 xnor s2) next d1 is 1 when s0 and s1 are same
    //d2 = x1. s1 second bit copied the first bit in next step
    transitionFunctions.push_back(fsm2_1->and2(x1, fsm2_1->xnor2(s1, s2)));
    transitionFunctions.push_back(fsm2_1->and2(x1, s1));

    fsm2_1->setTransitionFunctions(transitionFunctions);
    fsm2_1->setInitState({false, false});

    ASSERT_TRUE(fsm2_1->isReachable({false, false}));
    ASSERT_EQ(fsm2_1->stateDistance({false, false}), 0);

    ASSERT_TRUE(fsm2_1->isReachable({true, false}));
    ASSERT_EQ(fsm2_1->stateDistance({true, false}), 1);

    ASSERT_TRUE(fsm2_1->isReachable({false, true}));
    ASSERT_EQ(fsm2_1->stateDistance({false, true}), 2);
    
    ASSERT_FALSE(fsm2_1->isReachable({true, true}));
}

TEST_F(ReachabilityTest, lectureExample) {
    std::unique_ptr<ClassProject::ReachabilityInterface> fsm2_2 =
    std::make_unique<ClassProject::Reachability>(2,2);
    std::vector<BDD_ID> inputVars = fsm2_2->getInputs();
    std::vector<BDD_ID> stateVars = fsm2_2->getStates();
    std::vector<BDD_ID> transitionFunctions;
    auto s1 = stateVars.at(0);
    auto s2 = stateVars.at(1);
    auto x1 = inputVars.at(0);
    auto x2 = inputVars.at(1);
    // Lecture example
    //d1 = !x1 . (s1+s2)
    //d2 = x2 .(x1+s1+s2) 
    transitionFunctions.push_back(fsm2_2->and2(fsm2_2->neg(x1), fsm2_2->or2(s1, s2)));
    transitionFunctions.push_back(fsm2_2->and2(x2, fsm2_2->or2(x1, fsm2_2->or2(s1,s2))));

    fsm2_2->setInitState({false, false});
    fsm2_2->setTransitionFunctions(transitionFunctions);

    ASSERT_TRUE(fsm2_2->isReachable({false, false}));
    ASSERT_EQ(fsm2_2->stateDistance({false, false}), 0);
    ASSERT_TRUE(fsm2_2->isReachable({false, true}));
    ASSERT_EQ(fsm2_2->stateDistance({false, true}), 1);
    ASSERT_TRUE(fsm2_2->isReachable({true, false}));
    ASSERT_TRUE(fsm2_2->isReachable({true, true}));
}

TEST_F(ReachabilityTest, complexFSM) {
    // Create a 4-state-bit FSM with 3 input bits
    std::unique_ptr<ClassProject::ReachabilityInterface> fsm4_1 =
        std::make_unique<ClassProject::Reachability>(4, 1);

    // Retrieve input and state variables
    std::vector<BDD_ID> inputVars = fsm4_1->getInputs();
    std::vector<BDD_ID> stateVars = fsm4_1->getStates();

    std::vector<BDD_ID> transitionFunctions;

    // Assign state variables
    auto s1 = stateVars.at(0);
    auto s2 = stateVars.at(1);
    auto s3 = stateVars.at(2);
    auto s4 = stateVars.at(3);

    // Assign input variables
    auto x1 = inputVars.at(0);
    
    // Define transition functions
    // Example transitions:
    // d1 = 0
    // d2 = s2 XOR x1
    // d3 = s3 XOR (s2 AND x1)
    // d4 = s4 XOR (s3 AND (s2 AND x1))

    // Define transition functions
    transitionFunctions.push_back(fsm4_1->False());  // s1' = false
    transitionFunctions.push_back(fsm4_1->xor2(s2, x1));  // s2' = XOR(s2, x)
    transitionFunctions.push_back(fsm4_1->xor2(s3, fsm4_1->and2(s2, x1)));  // s3' = XOR(s3, AND(s2, x))
    transitionFunctions.push_back(fsm4_1->xor2(s4, fsm4_1->and2(s3, fsm4_1->and2(s2, x1))));  // s4' = XOR(s4, AND(s3, AND(s2, x)))

    // Set the initial state to {false, false, false, false} (0000)
    fsm4_1->setInitState({false, false, false, false});

    // Assign the transition functions to the FSM
    fsm4_1->setTransitionFunctions(transitionFunctions);

    // Assertions for reachability and distance

    // 0 0 0 0
    ASSERT_TRUE(fsm4_1->isReachable({false, false, false, false}));
    ASSERT_EQ(fsm4_1->stateDistance({false, false, false, false}), 0);

    // 0 0 1 0
    ASSERT_TRUE(fsm4_1->isReachable({false, true, false, false}));
    ASSERT_EQ(fsm4_1->stateDistance({false, true, false, false}), 1);    

    // 0 1 0 0
    ASSERT_TRUE(fsm4_1->isReachable({false, false, true, false}));
    ASSERT_EQ(fsm4_1->stateDistance({false, false, true, false}), 2);

    // 0 1 1 0
    ASSERT_TRUE(fsm4_1->isReachable({false, true, true, false}));
    ASSERT_EQ(fsm4_1->stateDistance({false, true, true, false}), 3);

    // 1 0 0 0
    ASSERT_TRUE(fsm4_1->isReachable({false, false, false, true}));
    ASSERT_EQ(fsm4_1->stateDistance({false, false, false, true}), 4);

    // 1 0 1 0
    ASSERT_TRUE(fsm4_1->isReachable({false, true, false, true}));
    ASSERT_EQ(fsm4_1->stateDistance({false, true, false, true}), 5);

    // 1 1 0 0
    ASSERT_TRUE(fsm4_1->isReachable({false, false, true, true}));
    ASSERT_EQ(fsm4_1->stateDistance({false, false, true, true}), 6);    

    // 1 1 1 0
    ASSERT_TRUE(fsm4_1->isReachable({false, true, true, true}));
    ASSERT_EQ(fsm4_1->stateDistance({false, true, true, true}), 7);

    // Unreachable Tests
    // 0 0 0 1
    ASSERT_FALSE(fsm4_1->isReachable({true, false, false, false}));

    // 0 0 1 1
    ASSERT_FALSE(fsm4_1->isReachable({true, true, false, false}));

    // 0 1 0 1
    ASSERT_FALSE(fsm4_1->isReachable({true, false, true, false}));

    // 0 1 1 1
    ASSERT_FALSE(fsm4_1->isReachable({true, true, true, false}));

    // 1 0 0 1
    ASSERT_FALSE(fsm4_1->isReachable({true, false, false, true}));

    // 1 0 1 1
    ASSERT_FALSE(fsm4_1->isReachable({true, true, false, true}));

    // 1 1 0 1
    ASSERT_FALSE(fsm4_1->isReachable({true, false, true, true}));

    // 1 1 1 1
    ASSERT_FALSE(fsm4_1->isReachable({true, true, true, true}));

}

#endif