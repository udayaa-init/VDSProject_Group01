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
    }
};

TEST_F(ManagerTest, CreateVar){
    
    EXPECT_EQ(manager.createVar("c"), 4);
    EXPECT_EQ(manager.createVar("d"), 5);

}

TEST_F(ManagerTest, IsConstant) {
  EXPECT_TRUE(manager.isConstant(0));
  EXPECT_TRUE(manager.isConstant(1));
  EXPECT_FALSE(manager.isConstant(2));
  EXPECT_FALSE(manager.isConstant(3));
}

TEST_F(ManagerTest, IsVariable) {
  EXPECT_FALSE(manager.isVariable(0));
  EXPECT_FALSE(manager.isVariable(1));
  EXPECT_TRUE(manager.isVariable(2));
  EXPECT_TRUE(manager.isVariable(3));
}

TEST_F(ManagerTest, True) { EXPECT_EQ(manager.True(), 1); }

TEST_F(ManagerTest, False) { EXPECT_EQ(manager.False(), 0); }

TEST_F(ManagerTest, UniqueTableSize) {
    manager.createVar("a");
    manager.createVar("b");
    EXPECT_EQ(manager.uniqueTableSize(), 4); 
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
