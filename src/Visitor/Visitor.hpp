#ifndef WASMABI_VISITOR_HPP
#define WASMABI_VISITOR_HPP

#include "../Nodes/Node.hpp"

namespace wasmabi {

struct Program;
struct Literal;
struct VariableType;
struct FunctionType;
struct Block;
struct FunctionDefinition;
struct FunctionDefinitionParameter;
struct FunctionCallExpression;
struct IdentifierAsExpression;
struct ValueExpression;
struct UnaryExpression;
struct BinaryExpression;
struct SelectExpression;
struct SelectExpressionCase;
struct Statement;
struct LoopStatement;
struct IfStatement;
struct ReturnStatement;
struct PrintStatement;
struct VariableDefinitionStatement;
struct VariableAssignmentStatement;
struct FunctionCallStatement;

class Visitor {
public:
  virtual void visit(Program &node) = 0;
  virtual void visit(Literal &node) = 0;
  virtual void visit(VariableType &node) = 0;
  virtual void visit(FunctionType &node) = 0;
  virtual void visit(Block &node) = 0;
  virtual void visit(FunctionDefinition &node) = 0;
  virtual void visit(FunctionDefinitionParameter &node) = 0;
  virtual void visit(FunctionCallExpression &node) = 0;
  virtual void visit(IdentifierAsExpression &node) = 0;
  virtual void visit(UnaryExpression &node) = 0;
  virtual void visit(BinaryExpression &node) = 0;
  virtual void visit(SelectExpression &node) = 0;
  virtual void visit(SelectExpressionCase &node) = 0;
  virtual void visit(LoopStatement &node) = 0;
  virtual void visit(IfStatement &node) = 0;
  virtual void visit(ReturnStatement &node) = 0;
  virtual void visit(PrintStatement &node) = 0;
  virtual void visit(VariableDefinitionStatement &node) = 0;
  virtual void visit(VariableAssignmentStatement &node) = 0;
  virtual void visit(FunctionCallStatement &node) = 0;
};

} // namespace wasmabi

#endif // WASMABI_VISITOR_HPP
