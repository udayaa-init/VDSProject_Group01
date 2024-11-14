//
// Created by tobias on 21.11.16.
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include <gtest/gtest.h>
#include "../Manager.h"

using namespace ClassProject;

TEST(Manager_Test, Create_Var){

    Manager manager;
    EXPECT_EQ(manager.createVar("a"), 0);
    EXPECT_EQ(manager.createVar("b"), 1);

}

#endif
