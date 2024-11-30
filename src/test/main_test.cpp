//
// Created by ludwig on 22.11.16.
//


#include "Tests.h"

using namespace ClassProject;

class ManagerTest : public :: testing::Test {
    protected:
    Manager manager;   

    virtual void SetUp()
    { 
        manager.createVar("a");
        manager.createVar("b");
        manager.createVar("c");
        manager.createVar("d");
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

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
