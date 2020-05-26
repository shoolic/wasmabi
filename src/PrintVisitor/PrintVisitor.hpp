#ifndef WASMABI_PRINT_VISITOR_HPP
#define WASMABI_PRINT_VISITOR_HPP

#include "../Nodes/Node.hpp"
#include "../Visitor/Visitor.hpp"

namespace wasmabi {

class PrintVisitor : public Visitor {
public:
  PrintVisitor();
  // void visit(Node &node);
  void visit(Program &node);
  void visit(Identifier &node);
  void visit(Literal &node);
  void visit(VariableType &node);
  void visit(FunctionType &node);
  void visit(Block &node);
  void visit(FunctionDefinition &node);
  void visit(FunctionDefinitionParameter &node);
  void visit(NullExpression &node);
  void visit(FunctionCallExpression &node);
  void visit(IdentifierAsExpression &node);
  // void visit(ValueExpression &node);
  void visit(UnaryExpression &node);
  void visit(BinaryExpression &node);
  void visit(SelectExpression &node);
  void visit(SelectExpressionCase &node);
  // void visit(Statement &node);
  void visit(LoopStatement &node);
  void visit(IfStatement &node);
  void visit(ReturnStatement &node);
  void visit(PrintStatement &node);
  void visit(VariableDefinitionStatement &node);
  void visit(VariableDefinitionWithAssignmentStatement &node);
  void visit(VariableAssignmentStatement &node);
  void visit(FunctionCallStatement &node);

private:
  void printIndent() const;
  int nest;
};
} // namespace wasmabi
#endif // WASMABI_PRINT_VISITOR_HPP
