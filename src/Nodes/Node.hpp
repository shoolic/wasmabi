#ifndef WASMABI_NODE_HPP
#define WASMABI_NODE_HPP
#include <iostream>
#include <map>
#include <memory>
#include <variant>
#include <vector>

#include "../Visitor/Visitor.hpp"

namespace wasmabi {

struct Program;
struct Identifier;
struct Literal;
struct VariableType;
struct FunctionType;
struct Block;
struct FunctionDefinition;
struct FunctionDefinitionParameter;
struct NullExpression;
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
struct VariableDefinitionWithAssignmentStatement;
struct VariableAssignmentStatement;
struct FunctionCallStatement;

class Visitor;

struct Node {
  virtual void accept(Visitor &visit) = 0;
};

struct Program : public Node {
  std::vector<std::unique_ptr<FunctionDefinition>> functionDefintions;

  void accept(Visitor &visitor) override;
};

struct FunctionDefinition : public Node {
  std::string identifier;
  std::vector<std::unique_ptr<FunctionDefinitionParameter>> parameters;
  std::unique_ptr<Block> block;
  std::unique_ptr<FunctionType> returnedType;

  void accept(Visitor &visitor) override;
};

struct FunctionDefinitionParameter : public Node {
  std::unique_ptr<Identifier> identifier;
  std::unique_ptr<VariableType> type;

  void accept(Visitor &visitor) override;
};

struct Block : public Node {
  std::vector<std::variant<std::unique_ptr<Block>, std::unique_ptr<Statement>>>
      instructions;

  void accept(Visitor &visitor) override;
};

struct Statement : public Node {};

struct LoopStatement : public Statement {
  std::unique_ptr<ValueExpression> condition;
  std::unique_ptr<Block> block;

  void accept(Visitor &visitor) override;
};

struct IfStatement : public Statement {
  std::unique_ptr<ValueExpression> condition;
  std::unique_ptr<Block> block;

  void accept(Visitor &visitor) override;
};

struct ReturnStatement : public Statement {
  std::unique_ptr<ValueExpression> value;

  void accept(Visitor &visitor) override;
};

struct PrintStatement : public Statement {
  std::unique_ptr<ValueExpression> value;

  void accept(Visitor &visitor) override;
};

struct VariableDefinitionStatement : public Statement {
  std::unique_ptr<Identifier> identifier;
  std::unique_ptr<VariableType> type;

  void accept(Visitor &visitor) override;
};

struct VariableDefinitionWithAssignmentStatement
    : public VariableDefinitionStatement {
  std::unique_ptr<ValueExpression> value;

  void accept(Visitor &visitor) override;
};

struct VariableAssignmentStatement : public Statement {
  std::unique_ptr<Identifier> identifier;
  std::unique_ptr<ValueExpression> value;

  void accept(Visitor &visitor) override;
};

struct FunctionCallStatement : public Statement {
  std::unique_ptr<FunctionCallExpression> functionCallExpression;

  void accept(Visitor &visitor) override;
};

struct Identifier : public Node {
  std::string name;
  Identifier(std::string name_) : name(name_){};

  void accept(Visitor &visitor) override;
};

struct VariableType : public Node {
  enum class Type {
    Int,
    Float,
    String,
  };

  Type type;
  VariableType(Type type_);

  void accept(Visitor &visitor) override;
};

struct FunctionType : public Node {
  enum class Type { Int, Float, String, Void };
  Type type;
  FunctionType(Type type_);

  void accept(Visitor &visitor) override;
};

struct ValueExpression : public Node {
  // virtual void print() const = 0;
};

struct IdentifierAsExpression : public ValueExpression {
  std::unique_ptr<Identifier> identifier;

  void accept(Visitor &visitor) override;
};

struct FunctionCallExpression : public ValueExpression {
  std::unique_ptr<Identifier> identifier;
  std::vector<std::unique_ptr<ValueExpression>> parameters;

  void accept(Visitor &visitor) override;
};

struct SelectExpression : public ValueExpression {
  std::vector<std::unique_ptr<SelectExpressionCase>> cases;
  std::unique_ptr<ValueExpression> otherwiseCaseValue;

  void accept(Visitor &visitor) override;
};

struct SelectExpressionCase : public Node {
  std::unique_ptr<ValueExpression> condition;
  std::unique_ptr<ValueExpression> value;

  void accept(Visitor &visitor) override;
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

  void accept(Visitor &visitor) override;
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
    Not,
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

  void accept(Visitor &visitor) override;
};

struct Literal : public ValueExpression {
  enum class Type { Int, Float, String };
  Type type;
  std::variant<std::string, int, float> value;
  Literal(Type type_, std::variant<std::string, int, float> value_);
  Literal(Type type_);

  void accept(Visitor &visitor) override;
};

struct NullExpression : public ValueExpression {

  void accept(Visitor &visitor) override;
};

} // namespace wasmabi

#endif // WASMABI_NODE_HPP
