#ifndef WASMABI_PARSER_HPP
#define WASMABI_PARSER_HPP
#include "../Errors/ErrorHandler.hpp"
#include "../Lexer/Lexer.hpp"
#include "../Nodes/Node.hpp"
#include <functional>
#include <iostream>
#include <memory>
namespace wasmabi {

class Parser {
public:
  Parser(Lexer &lexer_, ErrorHandler &errorHandler_, std::ostream &output_);
  std::unique_ptr<Program> parse();

private:
  std::unique_ptr<FunctionDefinition> createFunctionDefinition();
  void parseFunctionDefinitionParameters(
      std::unique_ptr<FunctionDefinition> &functionDefinition);
  std::unique_ptr<FunctionType> createFunctionReturnedType();
  std::unique_ptr<FunctionDefinitionParameter>
  createFunctionDefinitonParameter();

  std::unique_ptr<Identifier> createIdentifier(Token t);
  std::unique_ptr<VariableType> createVariableType(Token t);
  std::unique_ptr<Block> createBlock();

  std::unique_ptr<Statement> createStatement();
  std::unique_ptr<LoopStatement> createLoopStatement();
  std::unique_ptr<IfStatement> createIfStatement();
  std::unique_ptr<ReturnStatement> createReturnStatement();
  std::unique_ptr<PrintStatement> createPrintStatement();
  std::unique_ptr<Statement> createStatementFromIdentifier();
  std::unique_ptr<VariableDefinitionStatement>
  createVariableDefinitionStatement();
  std::unique_ptr<VariableAssignmentStatement>
  createVariableAssignmentStatement(Token t);
  std::unique_ptr<FunctionCallStatement> createFunctionCallStatement(Token t);

  std::unique_ptr<IdentifierAsExpression> createIdentifierAsExpression(Token t);
  std::unique_ptr<FunctionCallExpression> createFunctionCallExpression(Token t);
  std::unique_ptr<SelectExpression> createSelectExpression();
  std::unique_ptr<ValueExpression> createValueExpression(int rbp = 0);
  std::unique_ptr<ValueExpression> nud(Token t);
  std::unique_ptr<ValueExpression> led(std::unique_ptr<ValueExpression> left,
                                       Token oper);

  bool expect(Token token, Token::Type type);

  int bindingPower(Token t);
  void skipTypeOperator();
  bool isExpressionTerminator(Token t);

  Lexer &lexer;
  ErrorHandler &errorHandler;
  std::ostream &output;

  Token next();
  Token peek();
  Token currentToken;
  bool peeked;

  std::map<Token::Type, VariableType::Type> tokenToVarTypeMap;
  std::map<Token::Type, FunctionType::Type> tokenToFunTypeMap;
  std::map<Token::Type, BinaryExpression::Type> tokenToBinExprTypeMap;
  std::map<Token::Type, UnaryExpression::Type> tokenToUnExprTypeMap;
  std::map<Token::Type, Literal::Type> tokenToLiteralTypeMap;
  std::map<Token::Type, std::function<std::unique_ptr<ValueExpression>(Token)>>
      nudMap;
  std::map<Token::Type, std::function<std::unique_ptr<Statement>()>>
      statementMap;
  static std::map<Token::Type, int> bindingPowerMap;
};

} // namespace wasmabi

#endif // WASMABI_PARSER_HPP
