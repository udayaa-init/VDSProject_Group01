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

TEST_F(ManagerTest, ExistingcoFactorTrue) { 
    SetExample();

    EXPECT_EQ(manager.coFactorTrue(0), 0); 
    EXPECT_EQ(manager.coFactorTrue(1), 1);
    EXPECT_EQ(manager.coFactorTrue(2), 0); 
    EXPECT_EQ(manager.coFactorTrue(3), 0);
    EXPECT_EQ(manager.coFactorTrue(4), 0); 
    EXPECT_EQ(manager.coFactorTrue(5), 0);
    EXPECT_EQ(manager.coFactorTrue(6), 3); 
    EXPECT_EQ(manager.coFactorTrue(7), 0);
    EXPECT_EQ(manager.coFactorTrue(8), 0); 
    EXPECT_EQ(manager.coFactorTrue(9), 8);
}

TEST_F(ManagerTest, ExistingcoFactorFalse) { 
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

// TEST_F(ManagerTest, iteTerminalCase) {
//     // I == true
//     EXPECT_EQ(manager.ite(1, 2, 3),2); 
//     // I == false
//     EXPECT_EQ(manager.ite(0, 2, 3),3);
//     // T == E
//     EXPECT_EQ(manager.ite(2, 3, 3),3);
//     // if T == 1 and E ==  0
//     EXPECT_EQ(manager.ite(2, 1, 0),2);
// }



int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
