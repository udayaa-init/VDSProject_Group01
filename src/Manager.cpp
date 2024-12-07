#include "Manager.h"
#include <iostream>

namespace ClassProject{

   Manager::Manager(){
      BDD_ID falseId = createVar("False");
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
      auto & newNode = nodeTable.back();
      newNode.top_var = newNode.id;
      newNode.high = 1; 
      newNode.low = 0;
      return nodeTable[nodeTable.size()-1].id;
   }

   BDD_ID Manager::createVar(const std::string& label, const BDD_ID& top, const BDD_ID& high, const BDD_ID& low) {
      BDD_ID nodeId = createVar(label);
      nodeTable[nodeId].high= high;
      nodeTable[nodeId].low= low;
      nodeTable[nodeId].top_var= top;
      return nodeTable[nodeTable.size()-1].id;
   }

   const BDD_ID&  Manager::True() {
   return nodeTable[1].id;
   }
 
   const BDD_ID&  Manager::False() {
   return nodeTable[0].id;
   }

   size_t Manager::uniqueTableSize() {return nodeTable.size();}
  

   bool Manager::isConstant(BDD_ID f){
      auto & refNode = nodeTable[f];
      return (refNode.id == refNode.high) && (refNode.id == refNode.low) &&(refNode.id == refNode.top_var);
   }

   BDD_ID Manager::topVar(BDD_ID f){
      return nodeTable[f].top_var;
   }

   std::string Manager::getTopVarName(const BDD_ID &root){
      return nodeTable[topVar(root)].var_name;
   }

   bool Manager::isVariable(BDD_ID x){
      return !isConstant(x) && (nodeTable[x].top_var == x);
   }

   BDD_ID Manager::coFactorTrue(BDD_ID f) { return nodeTable[f].high ; }
   BDD_ID Manager::coFactorFalse(BDD_ID f) { return nodeTable[f].low; }

}
