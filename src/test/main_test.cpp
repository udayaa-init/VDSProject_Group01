//
// Created by ludwig on 22.11.16.
//


#include "Tests.h"

using namespace ClassProject;

class ManagerTest : public :: testing::Test {
    protected:
    // Sets up ID 0 and 1
    Manager manager;   

    // Sets up ID 2-5
    virtual void SetUp()
    { 
        manager.createVar("a");
        manager.createVar("b");
        manager.createVar("c");
        manager.createVar("d");
    }

    // Sets up ID 6-9
    void SetExample()
    {
        manager.createVar("a+b", 2, 1, 3);
        manager.createVar("c*d", 4, 5, 0);
        manager.createVar("b*c*d", 3, 7, 0);
        manager.createVar("f", 2, 7, 8);
    }
};

TEST_F(ManagerTest, CreateVar){
    
    EXPECT_EQ(manager.createVar("test_var1"), 6);
    EXPECT_EQ(manager.createVar("test_var2"), 7);

}

TEST_F(ManagerTest, IsConstant) {
  EXPECT_TRUE(manager.isConstant(0));
  EXPECT_TRUE(manager.isConstant(1));
  EXPECT_FALSE(manager.isConstant(2));
  EXPECT_FALSE(manager.isConstant(3));
  EXPECT_FALSE(manager.isConstant(4));
  EXPECT_FALSE(manager.isConstant(5));
}

TEST_F(ManagerTest, IsVariable) {
  EXPECT_FALSE(manager.isVariable(0));
  EXPECT_FALSE(manager.isVariable(1));
  EXPECT_TRUE(manager.isVariable(2));
  EXPECT_TRUE(manager.isVariable(3));
  EXPECT_TRUE(manager.isVariable(4));
  EXPECT_TRUE(manager.isVariable(5));
}

TEST_F(ManagerTest, True) { EXPECT_EQ(manager.True(), 1); }

TEST_F(ManagerTest, False) { EXPECT_EQ(manager.False(), 0); }

TEST_F(ManagerTest, UniqueTableSize) {
    EXPECT_EQ(manager.uniqueTableSize(),6); 
}

TEST_F(ManagerTest, TopVar){
    EXPECT_EQ(manager.topVar(manager.False()), 0);
    EXPECT_EQ(manager.topVar(manager.True()), 1);
    EXPECT_EQ(manager.topVar(2), 2);
    EXPECT_EQ(manager.topVar(3), 3);
    EXPECT_EQ(manager.topVar(4), 4);
    EXPECT_EQ(manager.topVar(5), 5);
}

TEST_F(ManagerTest, topVarName) { 
    EXPECT_EQ(manager.getTopVarName(0), "False"); 
    EXPECT_EQ(manager.getTopVarName(1), "True");
}

TEST_F(ManagerTest, ExistingcoFactorFalse) { 
    SetExample();

    EXPECT_EQ(manager.coFactorFalse(0), 0); 
    EXPECT_EQ(manager.coFactorFalse(1), 1);
    EXPECT_EQ(manager.coFactorFalse(2), 0); 
    EXPECT_EQ(manager.coFactorFalse(3), 0);
    EXPECT_EQ(manager.coFactorFalse(4), 0); 
    EXPECT_EQ(manager.coFactorFalse(5), 0);
    EXPECT_EQ(manager.coFactorFalse(6), 3); 
    EXPECT_EQ(manager.coFactorFalse(7), 0);
    EXPECT_EQ(manager.coFactorFalse(8), 0); 
    EXPECT_EQ(manager.coFactorFalse(9), 8);
}

TEST_F(ManagerTest, ExistingcoFactorTrue) { 
    SetExample();

    EXPECT_EQ(manager.coFactorTrue(0), 0); 
    EXPECT_EQ(manager.coFactorTrue(1), 1);
    EXPECT_EQ(manager.coFactorTrue(2), 1); 
    EXPECT_EQ(manager.coFactorTrue(3), 1);
    EXPECT_EQ(manager.coFactorTrue(4), 1); 
    EXPECT_EQ(manager.coFactorTrue(5), 1);
    EXPECT_EQ(manager.coFactorTrue(6), 1); 
    EXPECT_EQ(manager.coFactorTrue(7), 5);
    EXPECT_EQ(manager.coFactorTrue(8), 7); 
    EXPECT_EQ(manager.coFactorTrue(9), 7);
}

TEST_F(ManagerTest, iteTerminalCase) {
    // I == true
    EXPECT_EQ(manager.ite(1, 2, 3),2); 
    // I == false
    EXPECT_EQ(manager.ite(0, 2, 3),3);
    // T == E
    EXPECT_EQ(manager.ite(2, 3, 3),3);
    // if T == 1 and E ==  0
    EXPECT_EQ(manager.ite(2, 1, 0),2);
}

TEST_F(ManagerTest, coFactorTrueTerminalCase) {
    // f is constant
    EXPECT_EQ(manager.coFactorTrue(1, 3),1); 
    // x is constant
    EXPECT_EQ(manager.coFactorTrue(3, 1),3);
    // x is the top var for f
    EXPECT_EQ(manager.coFactorTrue(3, 3),1);
    // f doesnt depend on x
    EXPECT_EQ(manager.coFactorTrue(3, 2),3);
}

TEST_F(ManagerTest, coFactorFalseTerminalCase) {
    // f is constant
    EXPECT_EQ(manager.coFactorFalse(0, 3),0); 
    // x is constant
    EXPECT_EQ(manager.coFactorFalse(3, 0),3);
    // x is the top var for f
    EXPECT_EQ(manager.coFactorFalse(3, 3),0);
    // f doesnt depend on x
    EXPECT_EQ(manager.coFactorFalse(3, 2),3);
}

TEST_F(ManagerTest, coFactoriteExampleTest) {
    // f = (a + b) ∗c ∗d

    // (a + b)
    manager.ite(2,1,3);
    // c * d
    manager.ite(4,5,0);
    // (a + b) ∗c ∗d
    manager.ite(6,7,0);

    // Verfying the resultant tree
    EXPECT_EQ(manager.coFactorFalse(6), 3); 
    EXPECT_EQ(manager.coFactorFalse(7), 0);
    EXPECT_EQ(manager.coFactorFalse(8), 0); 
    EXPECT_EQ(manager.coFactorFalse(9), 8);

    EXPECT_EQ(manager.coFactorTrue(6), 1); 
    EXPECT_EQ(manager.coFactorTrue(7), 5);
    EXPECT_EQ(manager.coFactorTrue(8), 7); 
    EXPECT_EQ(manager.coFactorTrue(9), 7);

}

TEST_F(ManagerTest, and2) {
    // Setup() in the ManagerTest (Test Fixture) has already added a,b,c,d to the NodeTable

    // Now we will call and2() with the ids for the variables a (=2) and b (=3), it will return the id of the newly created node
    BDD_ID newNodeID = manager.and2(2,3);

    // The newly added node should have top variable = a (2), high = b (3), low = False (0)
    EXPECT_EQ(manager.topVar(newNodeID), 2);
    EXPECT_EQ(manager.coFactorTrue(newNodeID), 3);
    EXPECT_EQ(manager.coFactorFalse(newNodeID), 0);
}

TEST_F(ManagerTest, neg){
    // Setup() in the ManagerTest has already added a,b,c,d to the nodeTable 
    //Now we will call neg() with the ids for the variable b (=3), it will return the id of the newly created node
    BDD_ID newNodeID = manager.neg(3);
    //The newly added node should have top variable = b (3), high = False (0), low = True(1);
    EXPECT_EQ(manager.topVar(newNodeID), 3);
    EXPECT_EQ(manager.coFactorTrue(newNodeID), 0);
    EXPECT_EQ(manager.coFactorFalse(newNodeID), 1);
}

TEST_F(ManagerTest, or2){
    // Setup() in the ManagerTest has already added a,b,c,d to the nodeTable 
    //Now we will call or2() with the ids for the variables a (=2) and b (=3), it will return the id of the newly created node
    BDD_ID newNodeID = manager.or2(2,3);

    //The newly added node should have top variable = a (2), high = True (1), low = b (3);
    EXPECT_EQ(manager.topVar(newNodeID), 2);
    EXPECT_EQ(manager.coFactorTrue(newNodeID), 1);
    EXPECT_EQ(manager.coFactorFalse(newNodeID), 3);
}

TEST_F(ManagerTest, nor2){
    // Setup() in the ManagerTest has already added a,b,c,d to the nodeTable 
    //Now we will call nor2() with the ids for the variables a (=2) and b (=3), it will return the id of the newly created node
    BDD_ID newNodeID = manager.nor2(2,3);

    //The newly added node should have top variable = a (2), high = False (0), low = b_not (6);
    EXPECT_EQ(manager.topVar(newNodeID), 2);
    EXPECT_EQ(manager.coFactorTrue(newNodeID), 0);
    EXPECT_EQ(manager.coFactorFalse(newNodeID), (newNodeID - 1));

    // A new node for b_not should also be created
    EXPECT_EQ(manager.topVar(newNodeID - 1), 3);
    EXPECT_EQ(manager.coFactorTrue(newNodeID - 1), 0);
    EXPECT_EQ(manager.coFactorFalse(newNodeID - 1), 1);

}

TEST_F(ManagerTest, nand2){
    // Setup() in the ManagerTest has already added a,b,c,d to the nodeTable 
    //Now we will call nand2() with the ids for the variables a (=2) and b (=3), it will return the id of the newly created node
    BDD_ID newNodeID = manager.nand2(2,3);
    
    //The newly added node should have top variable = a (2), high = b_not (6), low = True (1);
    EXPECT_EQ(manager.topVar(newNodeID), 2);
    EXPECT_EQ(manager.coFactorTrue(newNodeID), (newNodeID - 1));
    EXPECT_EQ(manager.coFactorFalse(newNodeID), 1);

    // A new node for b_not should also be created
    EXPECT_EQ(manager.topVar(newNodeID - 1), 3);
    EXPECT_EQ(manager.coFactorTrue(newNodeID - 1), 0);
    EXPECT_EQ(manager.coFactorFalse(newNodeID - 1), 1);    
}

TEST_F(ManagerTest, xor2){
    // Setup() in the ManagerTest has already added a,b,c,d to the nodeTable 
    //Now we will call nand2() with the ids for the variables a (=2) and b (=3), it will return the id of the newly created node
    BDD_ID newNodeID = manager.xor2(2,3);

    //The newly added node should have top variable = a (2), high = b_not (6), low = b (3);
    EXPECT_EQ(manager.topVar(newNodeID), 2);
    EXPECT_EQ(manager.coFactorTrue(newNodeID), (newNodeID - 1));
    EXPECT_EQ(manager.coFactorFalse(newNodeID), 3);

    // A new node for b_not should also be created
    EXPECT_EQ(manager.topVar(newNodeID - 1), 3);
    EXPECT_EQ(manager.coFactorTrue(newNodeID - 1), 0);
    EXPECT_EQ(manager.coFactorFalse(newNodeID - 1), 1);  
}

TEST_F(ManagerTest, xnor2){
    // Setup() in the ManagerTest has already added a,b,c,d to the nodeTable 
    //Now we will call nand2() with the ids for the variables a (=2) and b (=3), it will return the id of the newly created node
    BDD_ID newNodeID = manager.xnor2(2,3);

    //The newly added node should have top variable = a (2), high = b (3), low = b_not(6);
    EXPECT_EQ(manager.topVar(newNodeID), 2);
    EXPECT_EQ(manager.coFactorTrue(newNodeID), 3);
    EXPECT_EQ(manager.coFactorFalse(newNodeID), (newNodeID - 1));

    // A new node for b_not should also be created
    EXPECT_EQ(manager.topVar(newNodeID - 1), 3);
    EXPECT_EQ(manager.coFactorTrue(newNodeID - 1), 0);
    EXPECT_EQ(manager.coFactorFalse(newNodeID - 1), 1);  
}


TEST_F(ManagerTest, findNodes){
    std::set<BDD_ID> nodes_a;
    std::set<BDD_ID> expected_nodes_a = {0, 1, 2};
    manager.findNodes(2, nodes_a);
    
    std::set<BDD_ID> nodes_aorb;
    std::set<BDD_ID> expected_nodes_aorb = {0, 1, 2, 3, 6};
    manager.findNodes(manager.or2(2,3),expected_nodes_aorb);
}

TEST_F(ManagerTest, findVars){
    std::set<BDD_ID> vars_a;
    std::set<BDD_ID> expected_vars_a = {2};
    manager.findNodes(2, vars_a);
    
    std::set<BDD_ID> vars_aorb;
    std::set<BDD_ID> expected_vars_aorb = {2, 3};
    manager.findNodes(manager.or2(2,3),expected_vars_aorb);
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}