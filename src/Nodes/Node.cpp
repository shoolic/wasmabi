#include "Node.hpp"
#include "../helpers/VariantOstream.hpp"
namespace wasmabi {

void Node::printIndent(int nest) const {
  while (nest--) {
    std::cout << "  ";
  }
}

void Program::print(int nest) const {

  printIndent(nest);
  std::cout << "Program" << std::endl;

  for (auto &x : functionDefintions) {
    x->print(nest + 1);
  }

  std::cout << std::endl;
}
void FunctionDefinition::print(int nest) const {
  printIndent(nest);
  std::cout << "FunctionDefinition" << std::endl;

  printIndent(nest + 1);
  std::cout << "Identifier " << identifier << std::endl;

  returnedType->print(nest + 1);

  printIndent(nest + 1);
  std::cout << "FunctionDefinitionParameters" << std::endl;
  for (auto &x : parameters) {
    x->print(nest + 2);
  }

  block->print(nest + 1);
}

void FunctionDefinitionParameter::print(int nest) const {
  printIndent(nest);

  std::cout << "FunctionDefinitionParameter" << std::endl;
  identifier->print(nest + 1);
  type->print(nest + 1);
}

void Block::print(int nest) const {
  printIndent(nest);
  std::cout << "Block" << std::endl;
  for (auto &x : instructions) {
    if (x.index() == 0) {
      std::get<std::unique_ptr<Block>>(x)->print(nest + 1);
    } else if (x.index() == 1) {
      std::get<std::unique_ptr<Statement>>(x)->print(nest + 1);
    }
  }
}
void LoopStatement::print(int nest) const {
  printIndent(nest);
  std::cout << "LoopStatement" << std::endl;

  printIndent(nest + 1);
  std::cout << "condition" << std::endl;
  condition->print(nest + 2);

  block->print(nest + 1);
}
void IfStatement::print(int nest) const {
  printIndent(nest);
  std::cout << "IfStatement" << std::endl;

  printIndent(nest + 1);
  std::cout << "condition" << std::endl;
  condition->print(nest + 2);

  block->print(nest + 1);
}

void ReturnStatement::print(int nest) const {
  printIndent(nest);
  std::cout << "ReturnStatement" << std::endl;
  printIndent(nest + 1);
  std::cout << "Value" << std::endl;
  value->print(nest + 2);
}

void PrintStatement::print(int nest) const {
  printIndent(nest);
  std::cout << "PrintStatement" << std::endl;
  printIndent(nest + 1);
  std::cout << "Value" << std::endl;
  value->print(nest + 2);
}

void VariableDefinitionStatement::print(int nest) const {
  printIndent(nest);
  std::cout << "VariableDefinitionStatement" << std::endl;
  type->print(nest + 1);
}

void VariableDefinitionWithAssignmentStatement::print(int nest) const {
  printIndent(nest);
  std::cout << "VariableDefinitionWithAssignmentStatement" << std::endl;
  identifier->print(nest + 1);
  type->print(nest + 1);
  printIndent(nest + 1);
  std::cout << "Value" << std::endl;
  value->print(nest + 2);
}

void VariableAssignmentStatement::print(int nest) const {
  printIndent(nest);
  std::cout << "VariableAssignmentStatement" << std::endl;
  identifier->print(nest + 1);
  value->print(nest + 2);
}

void FunctionCallStatement::print(int nest) const {
  printIndent(nest);
  std::cout << "FunctionCallStatement" << std::endl;
  functionCallExpression->print(nest + 1);
}

void VariableType::print(int nest) const {
  printIndent(nest);
  std::cout << "VariableType "
            << static_cast<std::underlying_type<Type>::type>(type) << std::endl;
}

void FunctionType::print(int nest) const {
  printIndent(nest);
  std::cout << "FunctionType "
            << static_cast<std::underlying_type<Type>::type>(type) << std::endl;
}

void Identifier::print(int nest) const {
  printIndent(nest);
  std::cout << "Identifier " << name << std::endl;
}
void SelectExpression::print(int nest) const {
  printIndent(nest);
  std::cout << "SelectExpression" << std::endl;

  printIndent(nest + 1);
  std::cout << "Cases" << std::endl;
  for (auto &x : cases) {
    x->print(nest + 2);
  }

  printIndent(nest + 1);
  std::cout << "OtherwiseCaseValue" << std::endl;
  otherwiseCaseValue->print(nest + 2);
}
void SelectExpressionCase::print(int nest) const {
  printIndent(nest);
  std::cout << "SelectExpressionCase" << std::endl;

  printIndent(nest + 1);
  std::cout << "Value" << std::endl;
  value->print(nest + 2);

  printIndent(nest + 1);
  std::cout << "Condition" << std::endl;
  condition->print(nest + 2);
}
void IdentifierAsExpression::print(int nest) const {
  // printIndent(nest);
  // std::cout << "IdentifierAsExpression" << std::endl;
  // identifier->print(nest + 1);
  identifier->print(nest);
}
void FunctionCallExpression::print(int nest) const {
  printIndent(nest);
  std::cout << "FunctionCallExpression" << std::endl;
  printIndent(nest + 1);
  std::cout << "Parameters" << std::endl;
  for (auto &x : parameters) {
    printIndent(nest + 2);
    std::cout << "Parameter" << std::endl;
    x->print(nest + 3);
  }
}
void Literal::print(int nest) const {
  printIndent(nest);
  std::cout << "Literal " << value << std::endl;
}
void NullExpression::print(int nest) const {
  printIndent(nest);
  std::cout << "null";
}
void BinaryExpression::print(int nest) const {
  printIndent(nest);
  std::cout << "BinaryExpression "
            << static_cast<std::underlying_type<Type>::type>(type) << std::endl;

  printIndent(nest + 1);
  std::cout << "LeftOperand" << std::endl;
  leftOperand->print(nest + 2);

  printIndent(nest + 1);
  std::cout << "RightOperand" << std::endl;
  rightOperand->print(nest + 2);
}

void UnaryExpression::print(int nest) const {
  printIndent(nest);
  std::cout << "UnaryExpression "
            << static_cast<std::underlying_type<Type>::type>(type) << std::endl;
  printIndent(nest + 1);
  std::cout << "Operand" << std::endl;
  operand->print(nest + 2);
}

FunctionType::FunctionType(Type type_) : type(type_) {}

VariableType::VariableType(Type type_) : type(type_) {}

UnaryExpression::UnaryExpression(Type type_) : type(type_) {}
UnaryExpression::UnaryExpression(Type type_,
                                 std::unique_ptr<ValueExpression> operand_)
    : type(type_), operand(std::move(operand_)) {}

BinaryExpression::BinaryExpression(Type type_) : type(type_) {}

BinaryExpression::BinaryExpression(
    Type type_, std::unique_ptr<ValueExpression> leftOperand_,
    std::unique_ptr<ValueExpression> rightOperand_)
    : type(type_), leftOperand(std::move(leftOperand_)),
      rightOperand(std::move(rightOperand_)) {}

Literal::Literal(Type type_) : type(type_) {}
Literal::Literal(Type type_, std::variant<std::string, int, float> value_)
    : type(type_), value(value_) {}

} // namespace wasmabi