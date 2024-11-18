//
// Created by ludwig on 22.11.16.
//


#include "Tests.h"

using namespace ClassProject;

class ManagerTest : public :: testing::Test {
    protected:
    Manager manager;   
};

TEST_F(ManagerTest, CreateVar){
    
    EXPECT_EQ(manager.createVar("a"), 2);
    EXPECT_EQ(manager.createVar("b"), 3);

}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
