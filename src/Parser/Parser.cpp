#include "Parser.hpp"

namespace wasmabi {

Parser::Parser(Lexer &lexer_, ErrorHandler &errorHandler_)
    : lexer(lexer_), errorHandler(errorHandler_), peeked(false),
      tokenToVarTypeMap{
          {Token::Type::IntTypename, VariableType::Type::Int},
          {Token::Type::FloatTypename, VariableType::Type::Float},
          {Token::Type::StringTypename, VariableType::Type::String},
      },
      tokenToFunTypeMap{
          {Token::Type::IntTypename, FunctionType::Type::Int},
          {Token::Type::FloatTypename, FunctionType::Type::Float},
          {Token::Type::StringTypename, FunctionType::Type::String},
          {Token::Type::VoidTypename, FunctionType::Type::Void},
      },
      tokenToBinExprTypeMap{
          {Token::Type::AddOperator, BinaryExpression::Type::Add},
          {Token::Type::SubOperator, BinaryExpression::Type::Sub},
          {Token::Type::MulOperator, BinaryExpression::Type::Mul},
          {Token::Type::DivOperator, BinaryExpression::Type::Div},
          {Token::Type::PowOperator, BinaryExpression::Type::Pow},
          {Token::Type::AndOperator, BinaryExpression::Type::And},
          {Token::Type::OrOperator, BinaryExpression::Type::Or},
          {Token::Type::Equals, BinaryExpression::Type::Equals},
          {Token::Type::NotEquals, BinaryExpression::Type::NotEquals},
          {Token::Type::Greater, BinaryExpression::Type::Greater},
          {Token::Type::GreaterOrEqual, BinaryExpression::Type::GreaterOrEqual},
          {Token::Type::Less, BinaryExpression::Type::Less},
          {Token::Type::LessOrEqual, BinaryExpression::Type::LessOrEqual},
      },
      tokenToUnExprTypeMap{
          {Token::Type::SubOperator, UnaryExpression::Type::Minus},
          {Token::Type::NotOperator, UnaryExpression::Type::Not}},
      tokenToLiteralTypeMap{
          {Token::Type::IntLiteral, Literal::Type::Int},
          {Token::Type::FloatLiteral, Literal::Type::Float},
          {Token::Type::StringLiteral, Literal::Type::String},
      },
      nudMap{
          {Token::Type::Select,
           [&](Token t) -> std::unique_ptr<ValueExpression> {
             return createSelectExpression();
           }},
          {Token::Type::Identifier,
           [&](Token t) -> std::unique_ptr<ValueExpression> {
             if (peek() == Token::Type::OpeningParenthesis) {
               return createFunctionCallExpression(t);
             }
             return createIdentifierAsExpression(t);
           }},
          {Token::Type::OpeningParenthesis,
           [&](Token t) -> std::unique_ptr<ValueExpression> {
             auto ret = createValueExpression();
             expect(next(), Token::Type::ClosingParenthesis);
             return ret;
           }},
      },
      statementMap{
          {Token::Type::Loop,

           [&]() -> std::unique_ptr<Statement> {
             return createLoopStatement();
           }},
          {Token::Type::If,

           [&]() -> std::unique_ptr<Statement> { return createIfStatement(); }},
          {Token::Type::Print,

           [&]() -> std::unique_ptr<Statement> {
             return createPrintStatement();
           }},
          {Token::Type::Return,

           [&]() -> std::unique_ptr<Statement> {
             return createReturnStatement();
           }},
          {Token::Type::Identifier,

           [&]() -> std::unique_ptr<Statement> {
             return createStatementFromIdentifier();
           }},

          {Token::Type::Let,

           [&]() -> std::unique_ptr<Statement> {
             return createVariableDefinitionStatement();
           }},

      }

{}

std::unique_ptr<Program> Parser::parse() {
  auto program = std::make_unique<Program>();
  try {
    while (peek() != Token::Type::Eof && peek() != Token::Type::Invalid) {
      program->functionDefintions.push_back(
          std::move(createFunctionDefinition()));
    }

    return program;
  } catch (UnexpectedToken &e) {
    errorHandler.registerUnexpectedTokenSyntaxError(e);
    throw;
  } catch (SyntaxError &e) {
    errorHandler.registerSyntaxError(e);
    throw;
  }
  return program;
}

bool Parser::expect(Token token, Token::Type type) {
  if (token != type) {
    throw UnexpectedToken(token, type);
  }

  return true;
}

std::unique_ptr<FunctionDefinition> Parser::createFunctionDefinition() {
  auto functionDefiniton = std::make_unique<FunctionDefinition>();

  expect(next(), Token::Type::Fn);

  Token t = next();
  expect(t, Token::Type::Identifier);
  functionDefiniton->identifier = std::get<std::string>(t.getValue());

  parseFunctionDefinitionParameters(functionDefiniton);
  functionDefiniton->returnedType = createFunctionReturnedType();

  functionDefiniton->block = createBlock();

  return functionDefiniton;
}

void Parser::parseFunctionDefinitionParameters(
    std::unique_ptr<FunctionDefinition> &functionDefinition) {

  expect(next(), Token::Type::OpeningParenthesis);
  while (peek() != Token::Type::ClosingParenthesis) {
    functionDefinition->parameters.push_back(
        std::move(createFunctionDefinitonParameter()));
    if (peek() != Token::Type::ClosingParenthesis) {
      expect(next(), Token::Type::Comma);
    }
  }

  expect(next(), Token::Type::ClosingParenthesis);
}

std::unique_ptr<FunctionType> Parser::createFunctionReturnedType() {
  skipTypeOperator();
  Token t = next();

  auto it = tokenToFunTypeMap.find(t.getType());
  if (it != tokenToFunTypeMap.end()) {
    return std::make_unique<FunctionType>(it->second);
  }

  throw UnknownFunRetType(t);
}

std::unique_ptr<Block> Parser::createBlock() {
  auto block = std::make_unique<Block>();

  expect(next(), Token::Type::OpeningBracket);

  while (peek() != Token::Type::ClosingBracket) {
    if (peek() == Token::Type::OpeningBracket) {
      block->instructions.push_back(std::move(createBlock()));
    } else {
      block->instructions.push_back(std::move(createStatement()));
    }
  }

  expect(next(), Token::Type::ClosingBracket);

  return block;
}

std::unique_ptr<FunctionDefinitionParameter>
Parser::createFunctionDefinitonParameter() {
  auto parameter = std::make_unique<FunctionDefinitionParameter>();

  parameter->identifier = createIdentifier(next());
  skipTypeOperator();
  parameter->type = createVariableType(next());

  return parameter;
}

std::string Parser::createIdentifier(Token t) {
  return std::get<std::string>(t.getValue());
}

std::unique_ptr<VariableType> Parser::createVariableType(Token t) {
  auto it = tokenToVarTypeMap.find(t.getType());
  if (it != tokenToVarTypeMap.end()) {
    return std::make_unique<VariableType>(it->second);
  }

  throw UnknownVarRetType(t);
}

void Parser::skipTypeOperator() { expect(next(), Token::Type::TypeOperator); }

std::unique_ptr<Statement> Parser::createStatement() {
  Token peekToken = peek();
  auto it = statementMap.find(peekToken.getType());
  if (it != statementMap.end()) {
    return it->second();
  }

  throw StatementError(peekToken);
}

std::unique_ptr<LoopStatement> Parser::createLoopStatement() {
  auto loopStatement = std::make_unique<LoopStatement>();

  expect(next(), Token::Type::Loop);

  loopStatement->condition = createValueExpression();
  loopStatement->block = createBlock();

  return loopStatement;
}

std::unique_ptr<IfStatement> Parser::createIfStatement() {
  auto ifStatement = std::make_unique<IfStatement>();

  expect(next(), Token::Type::If);

  ifStatement->condition = createValueExpression();
  ifStatement->block = createBlock();

  return ifStatement;
}

std::unique_ptr<ReturnStatement> Parser::createReturnStatement() {
  auto ret = std::make_unique<ReturnStatement>();

  expect(next(), Token::Type::Return);

  if (peek() == Token::Type::Semicolon) {
    ret->value = nullptr;
  } else {
    ret->value = createValueExpression();
  }

  expect(next(), Token::Type::Semicolon);
  return ret;
}

std::unique_ptr<PrintStatement> Parser::createPrintStatement() {
  auto ret = std::make_unique<PrintStatement>();
  expect(next(), Token::Type::Print);
  ret->value = createValueExpression();
  expect(next(), Token::Type::Semicolon);
  return ret;
}

std::unique_ptr<Statement> Parser::createStatementFromIdentifier() {

  Token identifier = next();
  expect(identifier, Token::Type::Identifier);

  if (peek() == Token::Type::AssignmentOperator) {
    return createVariableAssignmentStatement(identifier);
  }
  if (peek() == Token::Type::OpeningParenthesis) {
    return createFunctionCallStatement(identifier);
  }

  throw StatmentWithIdentifierError(peek());
}

std::unique_ptr<VariableDefinitionStatement>
Parser::createVariableDefinitionStatement() {
  expect(next(), Token::Type::Let);
  Token identifier = next();
  skipTypeOperator();
  Token type = next();
  Token determiner = next();
  auto variableDefinitionStatement =
      std::make_unique<VariableDefinitionStatement>();
  variableDefinitionStatement->identifier = createIdentifier(identifier);

  variableDefinitionStatement->type = createVariableType(type);
  if (determiner == Token::Type::Semicolon) {

    switch (variableDefinitionStatement->type->type) {
    case VariableType::Type::Float:
      variableDefinitionStatement->value =
          std::make_unique<Literal>(Literal::Type::Float, 0.0f);
      break;
    case VariableType::Type::Int:
      variableDefinitionStatement->value =
          std::make_unique<Literal>(Literal::Type::Int, 0);
      break;
    case VariableType::Type::String:
      variableDefinitionStatement->value =
          std::make_unique<Literal>(Literal::Type::String, "");
      break;
    }

    return variableDefinitionStatement;
  } else if (determiner == Token::Type::AssignmentOperator) {
    variableDefinitionStatement->value = createValueExpression();
    expect(next(), Token::Type::Semicolon);

    if (variableDefinitionStatement->value) {
      std::runtime_error("no value in assignment");
    }

  } else {
    throw VariableDefinitionStatementError(determiner);
  }
  return variableDefinitionStatement;
}

std::unique_ptr<VariableAssignmentStatement>
Parser::createVariableAssignmentStatement(Token t) {
  auto variableAssignmentStatement =
      std::make_unique<VariableAssignmentStatement>();
  variableAssignmentStatement->identifier = createIdentifier(t);
  expect(next(), Token::Type::AssignmentOperator);
  variableAssignmentStatement->value = createValueExpression();

  expect(next(), Token::Type::Semicolon);

  return variableAssignmentStatement;
}

std::unique_ptr<FunctionCallStatement>
Parser::createFunctionCallStatement(Token t) {
  auto functionCallStatement = std::make_unique<FunctionCallStatement>();
  functionCallStatement->functionCallExpression =
      createFunctionCallExpression(t);

  expect(next(), Token::Type::Semicolon);

  return functionCallStatement;
}

std::unique_ptr<FunctionCallExpression>
Parser::createFunctionCallExpression(Token t) {
  auto functionCallExpression = std::make_unique<FunctionCallExpression>();
  functionCallExpression->identifier = createIdentifier(t);
  t = next();

  if (peek() != Token::Type::ClosingParenthesis) {
    while (t != Token::Type::ClosingParenthesis) {
      functionCallExpression->parameters.push_back(
          std::move(createValueExpression()));
      t = next();
    }
  } else {
    next();
  }

  return functionCallExpression;
}

std::unique_ptr<IdentifierAsExpression>
Parser::createIdentifierAsExpression(Token t) {
  auto ret = std::make_unique<IdentifierAsExpression>();
  ret->identifier = createIdentifier(t);
  return ret;
}

std::unique_ptr<SelectExpression> Parser::createSelectExpression() {
  auto select = std::make_unique<SelectExpression>();

  expect(next(), Token::Type::OpeningBracket);

  while (peek() != Token::Type::ClosingBracket) {
    auto valueExpr = createValueExpression();

    Token t = next();
    if (t == Token::Type::When) {
      auto selectCase = std::make_unique<SelectExpressionCase>();
      selectCase->value = std::move(valueExpr);
      selectCase->condition = createValueExpression();
      select->cases.push_back(std::move(selectCase));
      expect(next(), Token::Type::Comma);
    } else if (t == Token::Type::Otherwise) {
      select->otherwiseCaseValue = std::move(valueExpr);
      break;
    } else {
      throw SelectExpressionError(t);
    }
  }

  expect(next(), Token::Type::ClosingBracket);

  return select;
}

std::unique_ptr<ValueExpression> Parser::createValueExpression(int rbp) {

  if (isExpressionTerminator(peek())) {
    return nullptr;
    // TODO error?
  }

  Token t = next();
  auto left = nud(t);

  while (!isExpressionTerminator(peek()) && bindingPower(peek()) > rbp) {
    left = led(std::move(left), next());
  }
  return left;
}

std::unique_ptr<ValueExpression> Parser::nud(Token t) {
  {
    auto it = tokenToUnExprTypeMap.find(t.getType());
    if (it != tokenToUnExprTypeMap.end()) {
      auto ret = std::make_unique<UnaryExpression>(it->second);
      ret->operand = nud(next());
      return ret;
    }
  }
  {
    auto it = tokenToLiteralTypeMap.find(t.getType());
    if (it != tokenToLiteralTypeMap.end()) {
      auto ret = std::make_unique<Literal>(it->second);
      ret->value = t.getValue();
      return ret;
    }
  }

  {
    auto it = nudMap.find(t.getType());
    if (it != nudMap.end()) {
      return it->second(t);
    }
  }

  throw ValueExpressionNudError(t);
}

std::unique_ptr<ValueExpression>
Parser::led(std::unique_ptr<ValueExpression> left, Token oper) {

  if (oper.getType() == Token::Type::PowOperator) {
    auto right = createValueExpression(bindingPower(oper) - 1);
    auto ret = std::make_unique<BinaryExpression>(BinaryExpression::Type::Pow);
    ret->leftOperand = std::move(left);
    ret->rightOperand = std::move(right);
    return ret;
  }

  {
    auto it = tokenToBinExprTypeMap.find(oper.getType());
    if (it != tokenToBinExprTypeMap.end()) {

      auto right = createValueExpression(bindingPower(oper));
      auto ret = std::make_unique<BinaryExpression>(it->second);
      ret->leftOperand = std::move(left);
      ret->rightOperand = std::move(right);
      return ret;
    }
  }

  throw ValueExpressionLedError(oper);
}

int Parser::bindingPower(Token t) {
  auto it = bindingPowerMap.find(t.getType());
  if (it != bindingPowerMap.end()) {
    return it->second;
  }
  throw UnexpectedTokenValExprError(t);
}

bool Parser::isExpressionTerminator(Token t) {
  return t == Token::Type::Eof || t == Token::Type::Comma ||
         t == Token::Type::Semicolon || t == Token::Type::OpeningBracket ||
         t == Token::Type::When || t == Token::Type::Otherwise;
}

Token Parser::next() {
  if (peeked) {
    peeked = false;
    return currentToken;
  }
  auto t = lexer.nextToken();
  return t;
}

Token Parser::peek() {
  if (peeked) {
    return currentToken;
  }

  peeked = true;
  currentToken = lexer.nextToken();
  return currentToken;
}

std::map<Token::Type, int> Parser::bindingPowerMap = {
    {Token::Type::ClosingParenthesis, 0},
    {Token::Type::AddOperator, 10},
    {Token::Type::SubOperator, 10},
    {Token::Type::MulOperator, 20},
    {Token::Type::DivOperator, 20},
    {Token::Type::PowOperator, 40},
    {Token::Type::Greater, 50},
    {Token::Type::GreaterOrEqual, 50},
    {Token::Type::Less, 50},
    {Token::Type::LessOrEqual, 50},
    {Token::Type::Equals, 50},
    {Token::Type::NotEquals, 60},
    {Token::Type::OrOperator, 60},
    {Token::Type::AndOperator, 60},
    {Token::Type::NotOperator, 60},
};
} // namespace wasmabi
