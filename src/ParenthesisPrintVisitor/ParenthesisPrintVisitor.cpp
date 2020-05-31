#include "ParenthesisPrintVisitor.hpp"
#include "../Helpers/VariantOstream.hpp"
#include <sstream>
namespace wasmabi {

std::string
ParenthesisPrintVisitor::getString(ValueExpression &valueExpression) {
  output = "";
  valueExpression.accept(*this);

  return output;
}

void ParenthesisPrintVisitor::visit(Literal &node) {
  std::stringstream s;
  s << node.value;
  output += "(" + s.str() + ")";
}

void ParenthesisPrintVisitor::visit(FunctionCallExpression &node) {
  output += "(" + node.identifier + "(";
  std::size_t i = 0;
  for (auto &param : node.parameters) {
    param->accept(*this);
    if (i < node.parameters.size() - 1) {
      output += ",";
    }
    i++;
  }

  output += "))";
}

void ParenthesisPrintVisitor::visit(IdentifierAsExpression &node) {
  output += "(" + node.identifier + ")";
}

void ParenthesisPrintVisitor::visit(UnaryExpression &node) {
  output += "(";
  output += unaryMap[node.type];
  node.operand->accept(*this);
  output += ")";
}

void ParenthesisPrintVisitor::visit(BinaryExpression &node) {
  output += "(";
  node.leftOperand->accept(*this);
  output += binaryMap[node.type];
  node.rightOperand->accept(*this);
  output += ")";
}

void ParenthesisPrintVisitor::visit(SelectExpression &node) {
  output += "select{";
  for (auto &c : node.cases) {
    c->condition->accept(*this);
    output += "when";
    c->value->accept(*this);
    output += ",";
  }
  node.otherwiseCaseValue->accept(*this);
  output += "otherwise}";
}

std::map<UnaryExpression::Type, std::string> ParenthesisPrintVisitor::unaryMap =
    {{
        {UnaryExpression::Type::Minus, "-"},
        {UnaryExpression::Type::Not, "not"},
    }};

std::map<BinaryExpression::Type, std::string>
    ParenthesisPrintVisitor::binaryMap = {{
        {BinaryExpression::Type::Add, "+"},
        {BinaryExpression::Type::Sub, "-"},
        {BinaryExpression::Type::Mul, "*"},
        {BinaryExpression::Type::Div, "/"},
        {BinaryExpression::Type::Pow, "^"},
        {BinaryExpression::Type::And, "and"},
        {BinaryExpression::Type::Or, "or"},
        {BinaryExpression::Type::Equals, "=="},
        {BinaryExpression::Type::NotEquals, "!="},
        {BinaryExpression::Type::Greater, ">"},
        {BinaryExpression::Type::GreaterOrEqual, ">="},
        {BinaryExpression::Type::Less, "<"},
        {BinaryExpression::Type::LessOrEqual, "<="},
    }};

void ParenthesisPrintVisitor::visit(Program &node) {}
void ParenthesisPrintVisitor::visit(VariableType &node) {}
void ParenthesisPrintVisitor::visit(FunctionType &node) {}
void ParenthesisPrintVisitor::visit(Block &node) {}
void ParenthesisPrintVisitor::visit(FunctionDefinition &node) {}
void ParenthesisPrintVisitor::visit(FunctionDefinitionParameter &node) {}
void ParenthesisPrintVisitor::visit(LoopStatement &node) {}
void ParenthesisPrintVisitor::visit(IfStatement &node) {}
void ParenthesisPrintVisitor::visit(ReturnStatement &node) {}
void ParenthesisPrintVisitor::visit(PrintStatement &node) {}
void ParenthesisPrintVisitor::visit(VariableDefinitionStatement &node) {}

void ParenthesisPrintVisitor::visit(VariableAssignmentStatement &node) {}
void ParenthesisPrintVisitor::visit(FunctionCallStatement &node) {}
void ParenthesisPrintVisitor::visit(SelectExpressionCase &node) {}

} // namespace wasmabi