#include "Manager.h"
#include <iostream>

namespace ClassProject{

   Manager::Manager(){
      BDD_ID falseId = createVar("Flase");
      nodeTable[falseId].high= 0;
      nodeTable[falseId].low= 0;
      nodeTable[falseId].top_var= 0;
      BDD_ID trueId = createVar("True");
      nodeTable[trueId].high= 1;
      nodeTable[trueId].low= 1;
      nodeTable[trueId].top_var= 1;
   }

   BDD_ID Manager::createVar(const std::string &label){
      nodeTable.push_back(Node(nodeTable.size(), label));
      return nodeTable[nodeTable.size()-1].id;
   }

   bool Manager::isConstant(BDD_ID f){
      auto & refNode = nodeTable[f];
      return (refNode.id == refNode.high) && (refNode.id == refNode.low) &&(refNode.id == refNode.top_var);
   }
}