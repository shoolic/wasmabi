#include "Node.hpp"
#include "../helpers/VariantOstream.hpp"
namespace wasmabi {
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

std::map<UnaryExpression::Type, std::string> UnaryExpression::map = {{
    {UnaryExpression::Type::Minus, "-"},
    {UnaryExpression::Type::Not, "not"},
}};

std::map<BinaryExpression::Type, std::string> BinaryExpression::map = {{
    {BinaryExpression::Type::Add, "+"},
    {BinaryExpression::Type::Sub, "-"},
    {BinaryExpression::Type::Mul, "*"},
    {BinaryExpression::Type::Div, "/"},
    {BinaryExpression::Type::Pow, "^"},
    {BinaryExpression::Type::And, "and"},
    {BinaryExpression::Type::Or, "or"},
    {BinaryExpression::Type::Not, "not"},
    {BinaryExpression::Type::Equals, "=="},
    {BinaryExpression::Type::NotEquals, "!="},
    {BinaryExpression::Type::Greater, ">"},
    {BinaryExpression::Type::GreaterOrEqual, ">="},
    {BinaryExpression::Type::Less, "<"},
    {BinaryExpression::Type::LessOrEqual, "<="},
}};

void Program::accept(Visitor &visitor) { visitor.visit(*this); }
void Identifier::accept(Visitor &visitor) { visitor.visit(*this); }
void Literal::accept(Visitor &visitor) { visitor.visit(*this); }
void VariableType::accept(Visitor &visitor) { visitor.visit(*this); }
void FunctionType::accept(Visitor &visitor) { visitor.visit(*this); }
void Block::accept(Visitor &visitor) { visitor.visit(*this); }
void FunctionDefinition::accept(Visitor &visitor) { visitor.visit(*this); }
void FunctionDefinitionParameter::accept(Visitor &visitor) {
  visitor.visit(*this);
}
void NullExpression::accept(Visitor &visitor) { visitor.visit(*this); }
void FunctionCallExpression::accept(Visitor &visitor) { visitor.visit(*this); }
void IdentifierAsExpression::accept(Visitor &visitor) { visitor.visit(*this); }
// void ValueExpression::accept(Visitor &visitor) { visitor.visit(*this); }
void UnaryExpression::accept(Visitor &visitor) { visitor.visit(*this); }
void BinaryExpression::accept(Visitor &visitor) { visitor.visit(*this); }
void SelectExpression::accept(Visitor &visitor) { visitor.visit(*this); }
void SelectExpressionCase::accept(Visitor &visitor) { visitor.visit(*this); }
// void Statement::accept(Visitor &visitor) { visitor.visit(*this); }
void LoopStatement::accept(Visitor &visitor) { visitor.visit(*this); }
void IfStatement::accept(Visitor &visitor) { visitor.visit(*this); }
void ReturnStatement::accept(Visitor &visitor) { visitor.visit(*this); }
void PrintStatement::accept(Visitor &visitor) { visitor.visit(*this); }
void VariableDefinitionStatement::accept(Visitor &visitor) {
  visitor.visit(*this);
}
void VariableDefinitionWithAssignmentStatement::accept(Visitor &visitor) {
  visitor.visit(*this);
}
void VariableAssignmentStatement::accept(Visitor &visitor) {
  visitor.visit(*this);
}
void FunctionCallStatement::accept(Visitor &visitor) { visitor.visit(*this); }

} // namespace wasmabi
