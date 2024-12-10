#include "Manager.h"
#include <iostream>
#include <algorithm>

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

   BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x) {  
   auto &node = nodeTable[f];
   if (isConstant(f) || isConstant(x) || node.top_var>x) return f;
   if (node.top_var == x) return node.high;

   auto high_part = coFactorTrue(node.high, x);
   auto low_part = coFactorTrue(node.low, x);
   return ite(node.top_var, high_part, low_part);
   }

   BDD_ID Manager::coFactorFalse(BDD_ID f, BDD_ID x) {  
   auto &node = nodeTable[f];
   if (isConstant(f) || isConstant(x) || node.top_var>x) return f;
   if (node.top_var == x) return node.low;

   auto high_part = coFactorFalse(node.high, x);
   auto low_part = coFactorFalse(node.low, x);
   return ite(node.top_var, high_part, low_part);
   };

   BDD_ID Manager::ite(BDD_ID i, BDD_ID t, BDD_ID e){
      //check for the terminal cases
      //Terminal cases of recursion:
      //   a) ite(1, f, g) = ite(0, g, f) = ite(f, 1, 0) = ite(g, f, f) = f
      //   b) ite(f, 0, 1) =!f  /// ite will take care of this
      if(i == True()) return t;
      if(i == False()) return e;
      if(t == e) return t;
      if(t == True() && e == False()) return i; // function variable

      // we have not added the computed_table

      // x be the top-varaible of (i,t,e)
      // we need to extract the top variable from i,t,e and order them based on the index values(assending) and top variable should not be constant
      std::vector<BDD_ID> x_vars = {topVar(i),topVar(t),topVar(e)};
      std::sort(x_vars.begin(), x_vars.end()); //induces variable ordering
      for (auto it = x_vars.begin(); it != x_vars.end(); ) {
        if (isConstant(*it)) {
            it = x_vars.erase(it); // Erase the element and update the iterator
        } else {
            ++it; // Move to the next element
        }
      }
      auto & x_node = nodeTable[x_vars.front()];
      auto r_high = ite(coFactorTrue(i, x_node.id), coFactorTrue(t, x_node.id),
                  coFactorTrue(e, x_node.id));
      auto r_low = ite(coFactorFalse(i, x_node.id), coFactorFalse(t, x_node.id),
                 coFactorFalse(e, x_node.id));

      if(r_high == r_low) return r_high;

      auto r = find_or_add_unique_table(x_node, r_low, r_high, nodeTable[t].var_name + " , " + nodeTable[e].var_name );

      return r;
   }

    BDD_ID Manager::find_or_add_unique_table(Node& x, BDD_ID r_low, BDD_ID r_high, std::string te){
      //Eliminate isomorphic 
      auto iso = std::find_if(nodeTable.begin(), nodeTable.end(), [&](Node& node) { // all variable in the outer scope can be accessed and referencing than copying
                               return node.id == x.id && node.high == r_high && node.low == r_low; });
      if(iso != nodeTable.end()) return iso->id;
      //create and add the node
      return createVar("ite( " + x.var_name + ", " +te +")",x.id, r_high, r_low);

    }

   BDD_ID Manager::and2(BDD_ID a, BDD_ID b){
      BDD_ID and_id = ite(a, b, False());
      auto & and_node = nodeTable[and_id];
      and_node.var_name = "(" + nodeTable[a].var_name +" * "+ nodeTable[b].var_name+ ")";
      return and_id;
   }

   BDD_ID Manager::or2(BDD_ID a, BDD_ID b){
      BDD_ID or_id = ite(a, True(), b);
      auto & or_node = nodeTable[or_id];
      or_node.var_name = "(" +nodeTable[a].var_name +" + "+ nodeTable[b].var_name + ")";
      return or_id;
   }

   BDD_ID Manager::neg(BDD_ID a){
      BDD_ID neg_id = ite(a, False(), True());
      auto & neg_node = nodeTable[neg_id];
      neg_node.var_name = "!" + nodeTable[a].var_name;
      return neg_id;
   }

   BDD_ID Manager::xor2(BDD_ID a, BDD_ID b){
      BDD_ID xor_id = ite(a, neg(b), b);
      auto & xor_node = nodeTable[xor_id];
      xor_node.var_name = "(" +nodeTable[a].var_name +" xor "+ nodeTable[b].var_name + ")";
      return xor_id;
   }

}
