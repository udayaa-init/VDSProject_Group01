#include "Manager.h"
#include <iostream>
#include <algorithm>

namespace ClassProject
{

   Manager::Manager()
   {
      // Add Constant False
      nodeTable.emplace_back(Node(0, 0, 0, 0, "False"));
      auto node = nodeTable.back();
      uniqueTable[std::make_tuple(node.top_var, node.high, node.low)] = 0;
      // Add Constant True
      nodeTable.emplace_back(Node(1, 1, 1, 1, "True"));
      node = nodeTable.back();
      uniqueTable[std::make_tuple(node.top_var, node.high, node.low)] = node.id;
   }

   BDD_ID Manager::createVar(const std::string &label)
   {
      nodeTable.emplace_back(Node(nodeTable.size(), nodeTable.size(), 1, 0, label));
      auto node = nodeTable.back();
      uniqueTable[std::make_tuple(node.top_var, node.high, node.low)] = node.id;
      return node.id;
   }

   BDD_ID Manager::createNode(const std::string &label, const BDD_ID &top, const BDD_ID &high, const BDD_ID &low)
   {
      nodeTable.emplace_back(Node(nodeTable.size(), top, high, low, label));
      auto node = nodeTable.back();
      uniqueTable[std::make_tuple(top, high, low)] = node.id;
      return node.id;
   }

   const BDD_ID &Manager::True()
   {
      return nodeTable[1].id;
   }

   const BDD_ID &Manager::False()
   {
      return nodeTable[0].id;
   }

   size_t Manager::uniqueTableSize() { return nodeTable.size(); }

   bool Manager::isConstant(BDD_ID f)
   {
      auto &refNode = nodeTable[f];
      return (refNode.id == refNode.high) && (refNode.id == refNode.low) && (refNode.id == refNode.top_var);
   }

   BDD_ID Manager::topVar(BDD_ID f)
   {
      return nodeTable[f].top_var;
   }

   std::string Manager::getTopVarName(const BDD_ID &root)
   {
      return nodeTable[topVar(root)].var_name;
   }

   bool Manager::isVariable(BDD_ID x)
   {
      return !isConstant(x) && (nodeTable[x].top_var == x);
   }

   BDD_ID Manager::coFactorTrue(BDD_ID f) { return nodeTable[f].high; }
   BDD_ID Manager::coFactorFalse(BDD_ID f) { return nodeTable[f].low; }

   BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x)
   {
      auto &node = nodeTable[f];
      if (isConstant(f) || isConstant(x) || node.top_var > x)
         return f;
      if (node.top_var == x)
         return node.high;
      auto high_part = coFactorTrue(node.high, x);
      auto low_part = coFactorTrue(node.low, x);
      return ite(node.top_var, high_part, low_part);
   }

   BDD_ID Manager::coFactorFalse(BDD_ID f, BDD_ID x)
   {
      auto &node = nodeTable[f];
      if (isConstant(f) || isConstant(x) || node.top_var > x)
         return f;
      if (node.top_var == x)
         return node.low;

      auto high_part = coFactorFalse(node.high, x);
      auto low_part = coFactorFalse(node.low, x);
      return ite(node.top_var, high_part, low_part);
   };

   BDD_ID Manager::ite(BDD_ID i, BDD_ID t, BDD_ID e)
   {
      // check for the terminal cases
      // Terminal cases of recursion:
      //    a) ite(1, f, g) = ite(0, g, f) = ite(f, 1, 0) = ite(g, f, f) = f
      //    b) ite(f, 0, 1) =!f  /// ite will take care of this
      if (i == True())
         return t;
      if (i == False())
         return e;
      if (t == e)
         return t;
      if (t == True() && e == False())
         return i; // function variable

      // we have not added the computed_table
      key ite_key = std::make_tuple(i,t,e);
      if (computeTable.find(ite_key) != computeTable.end()) {
         return computeTable[ite_key];
      }
      // x be the top-varaible of (i,t,e)
      // we need to extract the top variable from i,t,e and order them based on the index values(assending) and top variable should not be constant
      std::vector<BDD_ID> x_vars = {topVar(i), topVar(t), topVar(e)};
      std::sort(x_vars.begin(), x_vars.end()); // induces variable ordering
      for (auto it = x_vars.begin(); it != x_vars.end();)
      {
         if (isConstant(*it))
         {
            it = x_vars.erase(it); // Erase the element and update the iterator
         }
         else
         {
            ++it; // Move to the next element
         }
      }
      auto &x_node = nodeTable[x_vars.front()];
      BDD_ID x_id = x_node.id;
      auto r_high = ite(coFactorTrue(i, x_id), coFactorTrue(t, x_id),
                        coFactorTrue(e, x_id));
      auto r_low = ite(coFactorFalse(i, x_id), coFactorFalse(t, x_id),
                       coFactorFalse(e, x_id));

      if (r_high == r_low)
         return r_high;

      auto r = find_or_add_unique_table(x_id, r_low, r_high, "");
      computeTable[ite_key] = r;
      return r;
   }

   BDD_ID Manager::find_or_add_unique_table(BDD_ID xid, BDD_ID r_low, BDD_ID r_high, std::string te)
   {
      // Eliminate isomorphic
      // auto iso = std::find_if(nodeTable.begin(), nodeTable.end(), [&](Node &node) { // all variable in the outer scope can be accessed and referencing than copying
      //    return node.top_var == xid && node.high == r_high && node.low == r_low;
      // }); // node.id == x.id &&
      // if (iso != nodeTable.end())
      //    return iso->id;
      key vgh_tup = std::make_tuple(xid, r_high, r_low);

      if(uniqueTable.find(vgh_tup)!= uniqueTable.end()){
         computeTable[vgh_tup] = uniqueTable[vgh_tup];
         return uniqueTable[vgh_tup];
      }
      // create and add the node
      return createNode("", xid, r_high, r_low);
   }

   BDD_ID Manager::and2(BDD_ID a, BDD_ID b)
   {
      BDD_ID and_id = ite(a, b, False());
      auto &and_node = nodeTable[and_id];
      // and_node.var_name = "" + and_id;
      return and_id;
   }

   BDD_ID Manager::or2(BDD_ID a, BDD_ID b)
   {
      BDD_ID or_id = ite(a, True(), b);
      auto &or_node = nodeTable[or_id];
      // or_node.var_name = "" + or_id;
      return or_id;
   }

   BDD_ID Manager::neg(BDD_ID a)
   {
      BDD_ID neg_id = ite(a, False(), True());
      auto &neg_node = nodeTable[neg_id];
      // neg_node.var_name = "" + neg_id;
      return neg_id;
   }

   BDD_ID Manager::xor2(BDD_ID a, BDD_ID b)
   {
      BDD_ID xor_id = ite(a, neg(b), b);
      auto &xor_node = nodeTable[xor_id];
      // xor_node.var_name = "" + xor_id;
      return xor_id;
   }

   BDD_ID Manager::nand2(BDD_ID a, BDD_ID b)
   {
      BDD_ID nand_id = ite(a, neg(b), True());
      auto &nand_node = nodeTable[nand_id];
      // nand_node.var_name = "" + nand_id;
      return nand_id;
   }

   BDD_ID Manager::nor2(BDD_ID a, BDD_ID b)
   {
      BDD_ID nor_id = ite(a, False(), neg(b));
      auto &nor_node = nodeTable[nor_id];
      // nor_node.var_name = "" + nor_id;
      return nor_id;
   }

   BDD_ID Manager::xnor2(BDD_ID a, BDD_ID b)
   {
      BDD_ID xnor_id = ite(a, b, neg(b));
      auto &xnor_node = nodeTable[xnor_id];
      // xnor_node.var_name = "" + xnor_id;
      return xnor_id;
   }

   void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root)
   {
      auto &root_node = nodeTable[root];
      auto res = nodes_of_root.insert(root);
      if(!res.second) return;
      if (isConstant(root))
         return;

      findNodes(root_node.high, nodes_of_root);
      findNodes(root_node.low, nodes_of_root);
   }

   void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root)
   {
      if (isConstant(root))
         return;

      auto &root_node = nodeTable[root];
      auto res = vars_of_root.insert(root_node.top_var);
      if(!res.second) return;
      findVars(root_node.high, vars_of_root);
      findVars(root_node.low, vars_of_root);
   }

}
