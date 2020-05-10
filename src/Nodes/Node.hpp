#ifndef WASMABI_NODE_HPP
#define WASMABI_NODE_HPP
#include <iostream>
#include <memory>
#include <variant>
#include <vector>
namespace wasmabi {

struct Program;
struct ValueExpression;
struct Identifier;
struct VariableType;
struct FunctionType;
struct Block;
struct FunctionDefinition;
struct SelectExpressionCase;
struct FunctionDefinitionParameter;
struct Statement;
struct FunctionCallExpression;

struct Node {
  virtual void print(int nest) const = 0;
  void printIndent(int nest) const;
};

struct Program : public Node {
  std::vector<std::unique_ptr<FunctionDefinition>> functionDefintions;
  void print(int nest) const override;
};

struct FunctionDefinition : public Node {
  std::string identifier;
  std::vector<std::unique_ptr<FunctionDefinitionParameter>> parameters;
  std::unique_ptr<Block> block;
  std::unique_ptr<FunctionType> returnedType;
  void print(int nest) const override;
};

struct FunctionDefinitionParameter : public Node {
  std::unique_ptr<Identifier> identifier;
  std::unique_ptr<VariableType> type;
  void print(int nest) const override;
};

struct Block : public Node {
  std::vector<std::variant<std::unique_ptr<Block>, std::unique_ptr<Statement>>>
      instructions;
  void print(int nest) const override;
};

struct Statement : public Node {};

struct LoopStatement : public Statement {
  std::unique_ptr<ValueExpression> condition;
  std::unique_ptr<Block> block;
  void print(int nest) const override;
};

struct IfStatement : public Statement {
  std::unique_ptr<ValueExpression> condition;
  std::unique_ptr<Block> block;
  void print(int nest) const override;
};

struct ReturnStatement : public Statement {
  std::unique_ptr<ValueExpression> value;
  void print(int nest) const override;
};

struct PrintStatement : public Statement {
  std::unique_ptr<ValueExpression> value;
  void print(int nest) const override;
};

struct VariableDefinitionStatement : public Statement {
  std::unique_ptr<Identifier> identifier;
  std::unique_ptr<VariableType> type;
  void print(int nest) const override;
};

struct VariableDefinitionWithAssignmentStatement
    : public VariableDefinitionStatement {
  std::unique_ptr<ValueExpression> value;
  void print(int nest) const override;
};

struct VariableAssignmentStatement : public Statement {
  std::unique_ptr<Identifier> identifier;
  std::unique_ptr<ValueExpression> value;
  void print(int nest) const override;
};

struct FunctionCallStatement : public Statement {
  std::unique_ptr<FunctionCallExpression> functionCallExpression;
  void print(int nest) const override;
};

struct Identifier : public Node {
  std::string name;
  Identifier(std::string name_) : name(name_){};
  void print(int nest) const override;
};

struct VariableType : public Node {
  enum class Type {
    Int,
    Float,
    String,
  };

  Type type;
  VariableType(Type type_);
  void print(int nest) const override;
};

struct FunctionType : public Node {
  enum class Type { Int, Float, String, Void };
  Type type;
  FunctionType(Type type_);
  void print(int nest) const override;
};

struct ValueExpression : public Node {
  // virtual void print() const = 0;
};

struct IdentifierAsExpression : public ValueExpression {
  std::unique_ptr<Identifier> identifier;
  void print(int nest) const override;
};

struct FunctionCallExpression : public ValueExpression {
  std::unique_ptr<Identifier> identifier;
  std::vector<std::unique_ptr<ValueExpression>> parameters;
  void print(int nest) const override;
};

struct SelectExpression : public ValueExpression {
  std::vector<std::unique_ptr<SelectExpressionCase>> cases;
  std::unique_ptr<ValueExpression> otherwiseCaseValue;
  void print(int nest) const override;
};

struct SelectExpressionCase : public Node {
  std::unique_ptr<ValueExpression> condition;
  std::unique_ptr<ValueExpression> value;
  void print(int nest) const override;
};

struct UnaryExpression : public ValueExpression {
  enum class Type {
    Minus,
    Not,
  };
  Type type;
  std::unique_ptr<ValueExpression> operand;
  UnaryExpression(Type type_);
  UnaryExpression(Type type_, std::unique_ptr<ValueExpression> operand_);
  void print(int nest) const override;
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
  Type type;
  std::unique_ptr<ValueExpression> leftOperand;
  std::unique_ptr<ValueExpression> rightOperand;
  BinaryExpression(Type type_);
  BinaryExpression(Type type_, std::unique_ptr<ValueExpression> leftOperand_,
                   std::unique_ptr<ValueExpression> rightOperand_);
  void print(int nest) const override;
};

struct Literal : public ValueExpression {
  enum class Type { Int, Float, String };
  Type type;
  std::variant<std::string, int, float> value;
  Literal(Type type_, std::variant<std::string, int, float> value_);
  Literal(Type type_);
  void print(int nest) const override;
};

struct NullExpression : public ValueExpression {
  void print(int nest) const override;
};

} // namespace wasmabi

#endif // WASMABI_NODE_HPP
