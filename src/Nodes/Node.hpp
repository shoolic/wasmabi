#ifndef WASMABI_NODE_HPP
#define WASMABI_NODE_HPP
#include <iostream>
#include <map>
#include <memory>
#include <variant>
#include <vector>

#include "../Visitor/Visitor.hpp"
#include "llvm/IR/IRBuilder.h"

namespace wasmabi {

struct Program;
// struct Identifier;
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

class Visitor;
class Generator;

#define ACCEPT_VISITOR void accept(Visitor &visitor) override;

#define ACCEPT_GENERATOR llvm::Value *gen(Generator &generator) override;

#define ACCEPT_GENERATOR_NULL_RETURN                                           \
  llvm::Value *gen(Generator &generator) override;

struct Node {
  virtual void accept(Visitor &visit) = 0;

  virtual llvm::Value *gen(Generator &gen) = 0;
};

struct Program : public Node {
  std::vector<std::unique_ptr<FunctionDefinition>> functionDefintions;

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct FunctionDefinition : public Node {
  std::string identifier;
  std::vector<std::unique_ptr<FunctionDefinitionParameter>> parameters;
  std::unique_ptr<Block> block;
  std::unique_ptr<FunctionType> returnedType;

  ACCEPT_VISITOR
  ACCEPT_GENERATOR_NULL_RETURN
};

struct FunctionDefinitionParameter : public Node {
  std::string identifier;
  std::unique_ptr<VariableType> type;

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct Block : public Node {
  std::vector<std::variant<std::unique_ptr<Block>, std::unique_ptr<Statement>>>
      instructions;

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct Statement : public Node {};

struct LoopStatement : public Statement {
  std::unique_ptr<ValueExpression> condition;
  std::unique_ptr<Block> block;

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct IfStatement : public Statement {
  std::unique_ptr<ValueExpression> condition;
  std::unique_ptr<Block> block;

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct ReturnStatement : public Statement {
  std::unique_ptr<ValueExpression> value;

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct PrintStatement : public Statement {
  std::unique_ptr<ValueExpression> value;

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct VariableDefinitionStatement : public Statement {
  std::string identifier;
  std::unique_ptr<VariableType> type;
  std::unique_ptr<ValueExpression> value;

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct VariableAssignmentStatement : public Statement {
  std::string identifier;
  std::unique_ptr<ValueExpression> value;

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct FunctionCallStatement : public Statement {
  std::unique_ptr<FunctionCallExpression> functionCallExpression;

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct VariableType : public Node {
  enum Type {
    Int,
    Float,
    String,
  };

  Type type;
  VariableType(Type type_);

  ACCEPT_VISITOR
  ACCEPT_GENERATOR_NULL_RETURN
};

struct FunctionType : public Node {
  enum class Type { Int, Float, String, Void };
  Type type;
  FunctionType(Type type_);

  ACCEPT_VISITOR
  ACCEPT_GENERATOR_NULL_RETURN
};

struct ValueExpression : public Node {
  // virtual void print() const = 0;
};

struct IdentifierAsExpression : public ValueExpression {
  std::string identifier;

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct FunctionCallExpression : public ValueExpression {
  std::string identifier;
  std::vector<std::unique_ptr<ValueExpression>> parameters;

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct SelectExpression : public ValueExpression {
  std::vector<std::unique_ptr<SelectExpressionCase>> cases;
  std::unique_ptr<ValueExpression> otherwiseCaseValue;

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct SelectExpressionCase : public Node {
  std::unique_ptr<ValueExpression> condition;
  std::unique_ptr<ValueExpression> value;

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct UnaryExpression : public ValueExpression {
  enum class Type {
    Minus,
    Not,
  };
  static std::map<Type, std::string> map;
  Type type;
  std::unique_ptr<ValueExpression> operand;
  UnaryExpression(Type type_);
  UnaryExpression(Type type_, std::unique_ptr<ValueExpression> operand_);

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct BinaryExpression : public ValueExpression {
  enum class Type {
    Add,
    Sub,
    Mul,
    Div,
    Pow,
    And,
    Or,
    // Not,
    Equals,
    NotEquals,
    Greater,
    GreaterOrEqual,
    Less,
    LessOrEqual,
  };
  static std::map<Type, std::string> map;

  Type type;
  std::unique_ptr<ValueExpression> leftOperand;
  std::unique_ptr<ValueExpression> rightOperand;
  BinaryExpression(Type type_);
  BinaryExpression(Type type_, std::unique_ptr<ValueExpression> leftOperand_,
                   std::unique_ptr<ValueExpression> rightOperand_);

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

struct Literal : public ValueExpression {
  enum class Type { Int, Float, String };
  Type type;
  std::variant<std::string, int, float> value;
  Literal(Type type_, std::variant<std::string, int, float> value_);
  Literal(Type type_);

  ACCEPT_VISITOR
  ACCEPT_GENERATOR
};

} // namespace wasmabi

#endif // WASMABI_NODE_HPP
