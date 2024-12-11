//
// Created by ludwig on 22.11.16.
//


#include "Tests.h"

using namespace ClassProject;

class ManagerTest : public :: testing::Test {
    protected:
    // Sets up ID 0 and 1
    Manager manager; 

    BDD_ID a_id;
    BDD_ID b_id;
    BDD_ID c_id;
    BDD_ID d_id; 

    BDD_ID aorb_id;
    BDD_ID candd_id;
    BDD_ID f_id;

    // Sets up ID 2-5
    virtual void SetUp()
    { 
        a_id = manager.createVar("a");
        b_id = manager.createVar("b");
        c_id = manager.createVar("c");
        d_id = manager.createVar("d");
    }

    // Sets up ID 6-9
    void SetExample()
    {
        aorb_id = manager.or2(a_id,b_id);
        candd_id = manager.and2(c_id,d_id);
        f_id = manager.and2(aorb_id, candd_id);
    }
};

TEST_F(ManagerTest, CreateVar){

    size_t setupSize = manager.uniqueTableSize();
    
    BDD_ID test1_id  =  manager.createVar("test_var1");
    EXPECT_EQ(test1_id, manager.uniqueTableSize()-1);
    EXPECT_EQ(manager.uniqueTableSize(), setupSize + 1);

    BDD_ID test2_id  =  manager.createVar("test_var2");
    EXPECT_EQ(test2_id, manager.uniqueTableSize()-1);
    EXPECT_EQ(manager.uniqueTableSize(), setupSize + 2);

}

TEST_F(ManagerTest, IsConstant) {
  EXPECT_TRUE(manager.isConstant(manager.False()));
  EXPECT_TRUE(manager.isConstant(manager.True()));
  EXPECT_FALSE(manager.isConstant(a_id));
  EXPECT_FALSE(manager.isConstant(b_id));
  EXPECT_FALSE(manager.isConstant(c_id));
  EXPECT_FALSE(manager.isConstant(d_id));
}

TEST_F(ManagerTest, IsVariable) {
  EXPECT_FALSE(manager.isVariable(manager.False()));
  EXPECT_FALSE(manager.isVariable(manager.True()));
  EXPECT_TRUE(manager.isVariable(a_id));
  EXPECT_TRUE(manager.isVariable(b_id));
  EXPECT_TRUE(manager.isVariable(c_id));
  EXPECT_TRUE(manager.isVariable(d_id));
}

TEST_F(ManagerTest, True) { EXPECT_EQ(manager.True(), 1); }

TEST_F(ManagerTest, False) { EXPECT_EQ(manager.False(), 0); }

// This test is possibly redundant (see CreateVar)
TEST_F(ManagerTest, UniqueTableSize) {
    EXPECT_EQ(manager.uniqueTableSize(),6); 
}

TEST_F(ManagerTest, TopVar){
    EXPECT_EQ(manager.topVar(manager.False()), manager.False());
    EXPECT_EQ(manager.topVar(manager.True()), manager.True());
    EXPECT_EQ(manager.topVar(a_id), a_id);
    EXPECT_EQ(manager.topVar(b_id), b_id);
    EXPECT_EQ(manager.topVar(c_id), c_id);
    EXPECT_EQ(manager.topVar(d_id), d_id);
    
    SetExample();
    EXPECT_EQ(manager.topVar(aorb_id), a_id);
    EXPECT_EQ(manager.topVar(candd_id), c_id);
    EXPECT_EQ(manager.topVar(f_id), a_id);

}

TEST_F(ManagerTest, topVarName) { 
    EXPECT_EQ(manager.getTopVarName(manager.False()), "False"); 
    EXPECT_EQ(manager.getTopVarName(manager.True()), "True");
    EXPECT_EQ(manager.getTopVarName(a_id), "a");
    EXPECT_EQ(manager.getTopVarName(b_id), "b");
    EXPECT_EQ(manager.getTopVarName(c_id), "c");
    EXPECT_EQ(manager.getTopVarName(d_id), "d");
    
    SetExample();
    EXPECT_EQ(manager.getTopVarName(aorb_id), "a");
    EXPECT_EQ(manager.getTopVarName(candd_id), "c");
    EXPECT_EQ(manager.getTopVarName(f_id), "a");
}

TEST_F(ManagerTest, ExistingcoFactorFalse) { 
    SetExample();

    EXPECT_EQ(manager.coFactorFalse(manager.False()), manager.False()); 
    EXPECT_EQ(manager.coFactorFalse(manager.True()), manager.True());
    EXPECT_EQ(manager.coFactorFalse(a_id), manager.False()); 
    EXPECT_EQ(manager.coFactorFalse(b_id), manager.False());
    EXPECT_EQ(manager.coFactorFalse(c_id), manager.False()); 
    EXPECT_EQ(manager.coFactorFalse(d_id), manager.False());
    EXPECT_EQ(manager.coFactorFalse(aorb_id), b_id); 
    EXPECT_EQ(manager.coFactorFalse(candd_id), manager.False());
    EXPECT_EQ(manager.coFactorFalse(f_id), f_id - 1);
}

TEST_F(ManagerTest, ExistingcoFactorTrue) { 
    SetExample();

    EXPECT_EQ(manager.coFactorTrue(manager.False()), manager.False()); 
    EXPECT_EQ(manager.coFactorTrue(manager.True()), manager.True());
    EXPECT_EQ(manager.coFactorTrue(a_id), manager.True()); 
    EXPECT_EQ(manager.coFactorTrue(b_id), manager.True());
    EXPECT_EQ(manager.coFactorTrue(c_id), manager.True()); 
    EXPECT_EQ(manager.coFactorTrue(d_id), manager.True());
    EXPECT_EQ(manager.coFactorTrue(aorb_id), manager.True()); 
    EXPECT_EQ(manager.coFactorTrue(candd_id), d_id);
    EXPECT_EQ(manager.coFactorTrue(f_id), candd_id);
}

TEST_F(ManagerTest, iteTerminalCase) {
    // I == true
    EXPECT_EQ(manager.ite(manager.True(), a_id, b_id), a_id); 
    // I == false
    EXPECT_EQ(manager.ite(manager.False(), a_id, b_id), b_id);
    // T == E
    EXPECT_EQ(manager.ite(f_id, a_id, a_id), a_id);
    // if T == 1 and E ==  0
    EXPECT_EQ(manager.ite(a_id, manager.True(), manager.False()), a_id);
}

TEST_F(ManagerTest, coFactorTrueTerminalCase) {
    // f is constant
    EXPECT_EQ(manager.coFactorTrue(manager.True(), a_id), manager.True()); 
    // x is constant
    EXPECT_EQ(manager.coFactorTrue(f_id, manager.True()),f_id);
    // x is the top var for f
    EXPECT_EQ(manager.coFactorTrue(f_id, a_id),f_id);
    // f doesnt depend on x
    EXPECT_EQ(manager.coFactorTrue(candd_id, a_id),candd_id);
}

TEST_F(ManagerTest, coFactorFalseTerminalCase) {
    // f is constant
    EXPECT_EQ(manager.coFactorFalse(manager.False(), b_id), manager.False()); 
    // x is constant
    EXPECT_EQ(manager.coFactorFalse(b_id, manager.False()), b_id);
    // x is the top var for f
    EXPECT_EQ(manager.coFactorFalse(aorb_id, a_id),b_id);
    // f doesnt depend on x
    EXPECT_EQ(manager.coFactorFalse(candd_id, a_id),candd_id);
}

TEST_F(ManagerTest, coFactorANDiteExampleTest) {
    // f = (a + b) ∗c ∗d

    // (a + b)
    BDD_ID aorb_id = manager.ite(a_id, manager.True(), b_id);
    // c * d
    BDD_ID candd_id = manager.ite(c_id, d_id ,manager.False());
    // (a + b) ∗c ∗d
    BDD_ID f_id = manager.ite(aorb_id, candd_id, manager.False());

    // Verfying the resultant tree
    EXPECT_EQ(manager.coFactorFalse(aorb_id), b_id); 
    EXPECT_EQ(manager.coFactorFalse(candd_id), manager.False());
    EXPECT_EQ(manager.coFactorFalse(f_id), f_id - 1);

    EXPECT_EQ(manager.coFactorTrue(aorb_id), manager.True()); 
    EXPECT_EQ(manager.coFactorTrue(candd_id), d_id);
    EXPECT_EQ(manager.coFactorTrue(f_id), candd_id);

}

TEST_F(ManagerTest, and2) {
    // Setup() in the ManagerTest (Test Fixture) has already added a,b,c,d to the NodeTable

    // Now we will call and2() with the ids for the variables a (=2) and b (=3), it will return the id of the newly created node
    BDD_ID newNodeID = manager.and2(a_id, b_id);

    // The newly added node should have top variable = a (2), high = b (3), low = False (0)
    EXPECT_EQ(manager.topVar(newNodeID), a_id);
    EXPECT_EQ(manager.coFactorTrue(newNodeID), b_id);
    EXPECT_EQ(manager.coFactorFalse(newNodeID), manager.False());
}

TEST_F(ManagerTest, neg){
    // Setup() in the ManagerTest has already added a,b,c,d to the nodeTable 
    //Now we will call neg() with the ids for the variable b (=3), it will return the id of the newly created node
    BDD_ID newNodeID = manager.neg(b_id);
    //The newly added node should have top variable = b (3), high = False (0), low = True(1);
    EXPECT_EQ(manager.topVar(newNodeID), b_id);
    EXPECT_EQ(manager.coFactorTrue(newNodeID), manager.False());
    EXPECT_EQ(manager.coFactorFalse(newNodeID), manager.True());
}

TEST_F(ManagerTest, or2){
    // Setup() in the ManagerTest has already added a,b,c,d to the nodeTable 
    //Now we will call or2() with the ids for the variables a (=2) and b (=3), it will return the id of the newly created node
    BDD_ID newNodeID = manager.or2(a_id, b_id);

    //The newly added node should have top variable = a (2), high = True (1), low = b (3);
    EXPECT_EQ(manager.topVar(newNodeID), a_id);
    EXPECT_EQ(manager.coFactorTrue(newNodeID), manager.True());
    EXPECT_EQ(manager.coFactorFalse(newNodeID), b_id);
}

TEST_F(ManagerTest, nor2){
    // Setup() in the ManagerTest has already added a,b,c,d to the nodeTable 
    //Now we will call nor2() with the ids for the variables a (=2) and b (=3), it will return the id of the newly created node
    BDD_ID newNodeID = manager.nor2(a_id, b_id);

    //The newly added node should have top variable = a (2), high = False (0), low = b_not (6);
    EXPECT_EQ(manager.topVar(newNodeID), a_id);
    EXPECT_EQ(manager.coFactorTrue(newNodeID), manager.False());
    EXPECT_EQ(manager.coFactorFalse(newNodeID), (newNodeID - 1));

    // A new node for b_not should also be created
    EXPECT_EQ(manager.topVar(newNodeID - 1), b_id);
    EXPECT_EQ(manager.coFactorTrue(newNodeID - 1), manager.False());
    EXPECT_EQ(manager.coFactorFalse(newNodeID - 1), manager.True());

}

TEST_F(ManagerTest, nand2){
    // Setup() in the ManagerTest has already added a,b,c,d to the nodeTable 
    //Now we will call nand2() with the ids for the variables a (=2) and b (=3), it will return the id of the newly created node
    BDD_ID newNodeID = manager.nand2(a_id, b_id);
    
    //The newly added node should have top variable = a (2), high = b_not (6), low = True (1);
    EXPECT_EQ(manager.topVar(newNodeID), a_id);
    EXPECT_EQ(manager.coFactorTrue(newNodeID), (newNodeID - 1));
    EXPECT_EQ(manager.coFactorFalse(newNodeID), manager.True());

    // A new node for b_not should also be created
    EXPECT_EQ(manager.topVar(newNodeID - 1), b_id);
    EXPECT_EQ(manager.coFactorTrue(newNodeID - 1), manager.False());
    EXPECT_EQ(manager.coFactorFalse(newNodeID - 1), manager.True());   
}

TEST_F(ManagerTest, xor2){
    // Setup() in the ManagerTest has already added a,b,c,d to the nodeTable 
    //Now we will call nand2() with the ids for the variables a (=2) and b (=3), it will return the id of the newly created node
    BDD_ID newNodeID = manager.xor2(a_id, b_id);

    //The newly added node should have top variable = a (2), high = b_not (6), low = b (3);
    EXPECT_EQ(manager.topVar(newNodeID), a_id);
    EXPECT_EQ(manager.coFactorTrue(newNodeID), (newNodeID - 1));
    EXPECT_EQ(manager.coFactorFalse(newNodeID), b_id);

    // A new node for b_not should also be created
    EXPECT_EQ(manager.topVar(newNodeID - 1), b_id);
    EXPECT_EQ(manager.coFactorTrue(newNodeID - 1), manager.False());
    EXPECT_EQ(manager.coFactorFalse(newNodeID - 1), manager.True()); 
}

TEST_F(ManagerTest, xnor2){
    // Setup() in the ManagerTest has already added a,b,c,d to the nodeTable 
    //Now we will call nand2() with the ids for the variables a (=2) and b (=3), it will return the id of the newly created node
    BDD_ID newNodeID = manager.xnor2(a_id, b_id);

    //The newly added node should have top variable = a (2), high = b (3), low = b_not(6);
    EXPECT_EQ(manager.topVar(newNodeID), a_id);
    EXPECT_EQ(manager.coFactorTrue(newNodeID), b_id);
    EXPECT_EQ(manager.coFactorFalse(newNodeID), (newNodeID - 1));

    // A new node for b_not should also be created
    EXPECT_EQ(manager.topVar(newNodeID - 1), b_id);
    EXPECT_EQ(manager.coFactorTrue(newNodeID - 1), manager.False());
    EXPECT_EQ(manager.coFactorFalse(newNodeID - 1), manager.True());
}


TEST_F(ManagerTest, findNodes){
    std::set<BDD_ID> nodes_a;
    std::set<BDD_ID> expected_nodes_a = {manager.False(), manager.True(), a_id};
    manager.findNodes(a_id, nodes_a);
    EXPECT_EQ(nodes_a, expected_nodes_a);
    
    std::set<BDD_ID> nodes_aorb;
    BDD_ID aorb_id = manager.or2(a_id,b_id);
    std::set<BDD_ID> expected_nodes_aorb = {manager.False(), manager.True(), b_id, aorb_id};
    manager.findNodes(aorb_id, nodes_aorb);
    EXPECT_EQ(nodes_aorb, expected_nodes_aorb);
}

TEST_F(ManagerTest, findVars){
    std::set<BDD_ID> vars_a;
    std::set<BDD_ID> expected_vars_a = {a_id};
    manager.findVars(a_id, vars_a);
    EXPECT_EQ(vars_a, expected_vars_a);
    
    std::set<BDD_ID> vars_aorb;
    BDD_ID aorb_id = manager.or2(a_id,b_id);
    std::set<BDD_ID> expected_vars_aorb = {a_id, b_id};
    manager.findVars(aorb_id, vars_aorb);
    EXPECT_EQ(vars_aorb, expected_vars_aorb);

}

// FEEDBACK: Failing Test
TEST_F(ManagerTest, CoFactorTrueTest)
{
    //f1 = (a+b)*c*d
    SetExample();

    EXPECT_EQ(manager.coFactorTrue(f_id, a_id), candd_id);
    EXPECT_EQ(manager.coFactorTrue(f_id, a_id), manager.coFactorTrue(f_id));
    EXPECT_EQ(manager.coFactorTrue(f_id, b_id), candd_id);
    EXPECT_EQ(manager.coFactorTrue(f_id, c_id), manager.and2(d_id, aorb_id));
    EXPECT_EQ(manager.coFactorTrue(f_id, d_id), manager.and2(c_id, aorb_id));
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}