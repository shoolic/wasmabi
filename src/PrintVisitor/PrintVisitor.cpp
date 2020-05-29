#include "PrintVisitor.hpp"
#include "../helpers/VariantOstream.hpp"

namespace wasmabi {

PrintVisitor::PrintVisitor() : nest(0) {}

// void PrintVisitor::visit(Node &node) {}

void PrintVisitor::visit(Program &node) {
  printIndent();
  std::cout << "Program" << std::endl;

  nest += 1;
  for (auto &x : node.functionDefintions) {
    x->accept(*this);
  }
  nest += -1;

  std::cout << std::endl;
}

void PrintVisitor::visit(Identifier &node) {
  printIndent();
  std::cout << "Identifier " << node.name << std::endl;
}

void PrintVisitor::visit(Literal &node) {
  printIndent();
  std::cout << "Literal " << node.value << std::endl;
}

void PrintVisitor::visit(VariableType &node) {
  printIndent();
  std::cout << "VariableType "
            << static_cast<std::underlying_type<VariableType::Type>::type>(
                   node.type)
            << std::endl;
}

void PrintVisitor::visit(FunctionType &node) {
  printIndent();
  std::cout << "FunctionType "
            << static_cast<std::underlying_type<FunctionType::Type>::type>(
                   node.type)
            << std::endl;
}

void PrintVisitor::visit(Block &node) {
  printIndent();
  std::cout << "Block" << std::endl;
  nest += 1;
  for (auto &x : node.instructions) {
    if (x.index() == 0) {
      std::get<std::unique_ptr<Block>>(x)->accept(*this);
    } else if (x.index() == 1) {
      std::get<std::unique_ptr<Statement>>(x)->accept(*this);
    }
  }
  nest -= 1;
}

void PrintVisitor::visit(FunctionDefinition &node) {
  printIndent();
  std::cout << "FunctionDefinition" << std::endl;

  nest += 1;
  printIndent();
  std::cout << "Identifier " << node.identifier << std::endl;

  node.returnedType->accept(*this);

  printIndent();
  std::cout << "FunctionDefinitionParameters" << std::endl;

  nest += 1;
  for (auto &x : node.parameters) {
    x->accept(*this);
  }
  nest += -1;

  node.block->accept(*this);

  nest += -1;
}

void PrintVisitor::visit(FunctionDefinitionParameter &node) {
  printIndent();
  std::cout << "FunctionDefinitionParameter" << std::endl;

  nest += 1;

  visit(*node.identifier);
  visit(*node.type);

  nest -= 1;
}

void PrintVisitor::visit(NullExpression &node) {
  printIndent();
  std::cout << "null";
}

void PrintVisitor::visit(FunctionCallExpression &node) {
  printIndent();
  std::cout << "FunctionCallExpression" << std::endl;
  nest += 1;
  printIndent();
  std::cout << "Parameters" << std::endl;
  nest += 1;

  for (auto &x : node.parameters) {
    printIndent();
    std::cout << "Parameter" << std::endl;
    nest += 1;
    x->accept(*this);
    nest -= 1;
  }

  nest -= 2;
}

void PrintVisitor::visit(IdentifierAsExpression &node) {
  node.identifier->accept(*this);
}
// void PrintVisitor::visit(ValueExpression &node) {}
void PrintVisitor::visit(UnaryExpression &node) {
  printIndent();
  std::cout << "UnaryExpression "
            << static_cast<std::underlying_type<UnaryExpression::Type>::type>(
                   node.type)
            << std::endl;

  nest += 1;
  printIndent();
  std::cout << "Operand" << std::endl;

  nest += 1;
  node.operand->accept(*this);
  nest -= 2;
}

void PrintVisitor::visit(BinaryExpression &node) {
  printIndent();
  std::cout << "BinaryExpression "
            << static_cast<std::underlying_type<BinaryExpression::Type>::type>(
                   node.type)
            << std::endl;

  nest += 1;
  printIndent();

  std::cout << "LeftOperand" << std::endl;

  nest += 1;
  node.leftOperand->accept(*this);
  nest -= 1;

  printIndent();
  std::cout << "RightOperand" << std::endl;
  nest += 1;
  node.rightOperand->accept(*this);
  nest -= 2;
}

void PrintVisitor::visit(SelectExpression &node) {
  printIndent();
  std::cout << "SelectExpression" << std::endl;

  nest += 1;
  printIndent();
  std::cout << "Cases" << std::endl;

  nest += 1;
  for (auto &x : node.cases) {
    x->accept(*this);
  }
  nest -= 1;

  printIndent();
  std::cout << "OtherwiseCaseValue" << std::endl;

  nest += 1;

  node.otherwiseCaseValue->accept(*this);
  nest -= 2;
}
void PrintVisitor::visit(SelectExpressionCase &node) {
  printIndent();
  std::cout << "SelectExpressionCase" << std::endl;

  nest += 1;
  printIndent();
  std::cout << "Value" << std::endl;

  nest += 1;
  node.value->accept(*this);
  nest -= 1;

  nest += 1;
  printIndent();
  std::cout << "Condition" << std::endl;

  nest += 1;
  node.condition->accept(*this);
  nest -= 2;
}

// void PrintVisitor::visit(Statement &node) {}
void PrintVisitor::visit(LoopStatement &node) {
  printIndent();
  std::cout << "LoopStatement" << std::endl;

  nest += 1;
  printIndent();
  std::cout << "Condition" << std::endl;
  node.condition->accept(*this);
  node.block->accept(*this);
  nest -= 1;
}

void PrintVisitor::visit(IfStatement &node) {
  printIndent();
  std::cout << "IfStatement" << std::endl;

  nest += 1;
  printIndent();
  std::cout << "condition" << std::endl;

  nest += 1;
  node.condition->accept(*this);
  nest -= 1;

  node.block->accept(*this);
  nest -= 1;
}

void PrintVisitor::visit(ReturnStatement &node) {
  printIndent();
  std::cout << "ReturnStatement" << std::endl;
  nest += 1;
  printIndent();
  std::cout << "Value" << std::endl;
  nest += 1;
  node.value->accept(*this);
  nest -= 2;
}

void PrintVisitor::visit(PrintStatement &node) {
  printIndent();
  std::cout << "PrintStatement" << std::endl;
  nest += 1;
  printIndent();

  std::cout << "Value" << std::endl;
  nest += 1;

  node.value->accept(*this);
  nest -= 2;
}

void PrintVisitor::visit(VariableDefinitionStatement &node) {
  printIndent();
  std::cout << "VariableDefinitionStatement" << std::endl;
  nest += 1;
  node.type->accept(*this);
  nest -= 1;
}
void PrintVisitor::visit(VariableDefinitionWithAssignmentStatement &node) {
  printIndent();
  std::cout << "VariableDefinitionWithAssignmentStatement" << std::endl;
  nest += 1;
  node.identifier->accept(*this);
  node.type->accept(*this);

  std::cout << "Value" << std::endl;
  nest += 1;

  node.value->accept(*this);
  nest -= 2;
}

void PrintVisitor::visit(VariableAssignmentStatement &node) {
  printIndent();
  std::cout << "VariableAssignmentStatement" << std::endl;

  nest += 1;

  node.identifier->accept(*this);
  nest += 1;

  node.value->accept(*this);
  nest -= 2;
}
void PrintVisitor::visit(FunctionCallStatement &node) {
  printIndent();
  std::cout << "FunctionCallStatement" << std::endl;
  nest += 1;
  node.functionCallExpression->accept(*this);
  nest -= 1;
}

void PrintVisitor::printIndent() const {
  int tmpNest = nest;
  while (tmpNest--) {
    std::cout << "  ";
  }
}
} // namespace wasmabi