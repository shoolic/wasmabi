#include "PrintVisitor.hpp"
#include "../Helpers/VariantOstream.hpp"

namespace wasmabi {

PrintVisitor::PrintVisitor(std::ostream &output_) : output(output_), nest(0) {}

void PrintVisitor::visit(Program &node) {
  printIndent();
  output << "Program" << std::endl;

  nest += 1;
  for (auto &x : node.functionDefintions) {
    x->accept(*this);
  }
  nest += -1;

  output << std::endl;
}

void PrintVisitor::visit(Literal &node) {
  printIndent();
  output << "Literal " << node.value << std::endl;
}

void PrintVisitor::visit(VariableType &node) {
  printIndent();
  output << "VariableType "
         << static_cast<std::underlying_type<VariableType::Type>::type>(
                node.type)
         << std::endl;
}

void PrintVisitor::visit(FunctionType &node) {
  printIndent();
  output << "FunctionType "
         << static_cast<std::underlying_type<FunctionType::Type>::type>(
                node.type)
         << std::endl;
}

void PrintVisitor::visit(Block &node) {
  printIndent();
  output << "Block" << std::endl;
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
  output << "FunctionDefinition" << std::endl;

  nest += 1;
  printIndent();
  output << "Identifier " << node.identifier << std::endl;

  node.returnedType->accept(*this);

  printIndent();
  output << "FunctionDefinitionParameters" << std::endl;

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
  output << "FunctionDefinitionParameter" << std::endl;

  nest += 1;

  printIndent();
  output << "Identifier " << node.identifier << std::endl;
  visit(*node.type);

  nest -= 1;
}

void PrintVisitor::visit(FunctionCallExpression &node) {
  printIndent();
  output << "FunctionCallExpression" << std::endl;
  nest += 1;
  printIndent();
  output << "Parameters" << std::endl;
  nest += 1;

  for (auto &x : node.parameters) {
    printIndent();
    output << "Parameter" << std::endl;
    nest += 1;
    x->accept(*this);
    nest -= 1;
  }

  nest -= 2;
}

void PrintVisitor::visit(IdentifierAsExpression &node) {
  printIndent();
  output << "IdentifierAsExpression " << node.identifier << std::endl;
}

void PrintVisitor::visit(UnaryExpression &node) {
  printIndent();
  output << "UnaryExpression "
         << static_cast<std::underlying_type<UnaryExpression::Type>::type>(
                node.type)
         << std::endl;

  nest += 1;
  printIndent();
  output << "Operand" << std::endl;

  nest += 1;
  node.operand->accept(*this);
  nest -= 2;
}

void PrintVisitor::visit(BinaryExpression &node) {
  printIndent();
  output << "BinaryExpression "
         << static_cast<std::underlying_type<BinaryExpression::Type>::type>(
                node.type)
         << std::endl;

  nest += 1;
  printIndent();

  output << "LeftOperand" << std::endl;

  nest += 1;
  node.leftOperand->accept(*this);
  nest -= 1;

  printIndent();
  output << "RightOperand" << std::endl;
  nest += 1;
  node.rightOperand->accept(*this);
  nest -= 2;
}

void PrintVisitor::visit(SelectExpression &node) {
  printIndent();
  output << "SelectExpression" << std::endl;

  nest += 1;
  printIndent();
  output << "Cases" << std::endl;

  nest += 1;
  for (auto &x : node.cases) {
    x->accept(*this);
  }
  nest -= 1;

  printIndent();
  output << "OtherwiseCaseValue" << std::endl;

  nest += 1;

  node.otherwiseCaseValue->accept(*this);
  nest -= 2;
}
void PrintVisitor::visit(SelectExpressionCase &node) {
  printIndent();
  output << "SelectExpressionCase" << std::endl;

  nest += 1;
  printIndent();
  output << "Value" << std::endl;

  nest += 1;
  node.value->accept(*this);
  nest -= 1;

  nest += 1;
  printIndent();
  output << "Condition" << std::endl;

  nest += 1;
  node.condition->accept(*this);
  nest -= 2;
}

void PrintVisitor::visit(LoopStatement &node) {
  printIndent();
  output << "LoopStatement" << std::endl;

  nest += 1;
  printIndent();
  output << "Condition" << std::endl;
  node.condition->accept(*this);
  node.block->accept(*this);
  nest -= 1;
}

void PrintVisitor::visit(IfStatement &node) {
  printIndent();
  output << "IfStatement" << std::endl;

  nest += 1;
  printIndent();
  output << "condition" << std::endl;

  nest += 1;
  node.condition->accept(*this);
  nest -= 1;

  node.block->accept(*this);
  nest -= 1;
}

void PrintVisitor::visit(ReturnStatement &node) {
  printIndent();
  output << "ReturnStatement" << std::endl;
  nest += 1;
  printIndent();
  output << "Value" << std::endl;
  nest += 1;
  if (node.value) {
    node.value->accept(*this);
  }
  nest -= 2;
}

void PrintVisitor::visit(PrintStatement &node) {
  printIndent();
  output << "PrintStatement" << std::endl;
  nest += 1;
  printIndent();

  output << "Value" << std::endl;
  nest += 1;

  node.value->accept(*this);
  nest -= 2;
}

void PrintVisitor::visit(VariableDefinitionStatement &node) {
  printIndent();
  output << "VariableDefinitionStatement" << std::endl;
  nest += 1;
  printIndent();
  output << "Identifier " << node.identifier << std::endl;
  node.type->accept(*this);
  printIndent();

  output << "Value" << std::endl;
  nest += 1;

  node.value->accept(*this);
  nest -= 2;
}

void PrintVisitor::visit(VariableAssignmentStatement &node) {
  printIndent();
  output << "VariableAssignmentStatement" << std::endl;

  nest += 1;
  printIndent();
  output << "Identifier " << node.identifier << std::endl;
  nest += 1;

  node.value->accept(*this);
  nest -= 2;
}
void PrintVisitor::visit(FunctionCallStatement &node) {
  printIndent();
  output << "FunctionCallStatement" << std::endl;
  nest += 1;
  node.functionCallExpression->accept(*this);
  nest -= 1;
}

void PrintVisitor::printIndent() const {
  int tmpNest = nest;
  while (tmpNest--) {
    output << "  ";
  }
}
} // namespace wasmabi