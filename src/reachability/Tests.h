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
#endif