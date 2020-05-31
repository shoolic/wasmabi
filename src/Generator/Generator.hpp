#ifndef WASMABI_GENERATOR_HPP
#define WASMABI_GENERATOR_HPP

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"

// #include "../Nodes/Node.hpp"

namespace wasmabi {

struct Program;
struct Identifier;
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

class Generator {
public:
  Generator(std::ostream &output_);
  llvm::Value *gen(Program &node);
  llvm::Function *gen(FunctionDefinition &node);
  llvm::Value *gen(Literal &node);
  llvm::Type *gen(VariableType &node);
  llvm::Type *gen(FunctionType &node);
  llvm::Value *gen(Block &node);
  llvm::Value *gen(FunctionCallExpression &node);
  llvm::Value *gen(IdentifierAsExpression &node);
  llvm::Value *gen(UnaryExpression &node);
  llvm::Value *gen(BinaryExpression &node);
  llvm::Value *gen(SelectExpression &node);
  llvm::Value *gen(LoopStatement &node);
  llvm::Value *gen(IfStatement &node);
  llvm::Value *gen(ReturnStatement &node);
  llvm::Value *gen(PrintStatement &node);
  llvm::Value *gen(VariableDefinitionStatement &node);
  llvm::Value *gen(VariableAssignmentStatement &node);
  llvm::Value *gen(FunctionCallStatement &node);

private:
  std::ostream &output;
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder{context};
  std::unique_ptr<llvm::Module> module;

  std::vector<std::map<std::string, llvm::Value *>> values;
  std::map<std::string, llvm::Constant *> globalStrings;

  llvm::Value *genFloatBinExpr(BinaryExpression &node, llvm::Value *leftValue,
                               llvm::Value *rightValue);
  llvm::Value *genIntBinExpr(BinaryExpression &node, llvm::Value *leftValue,
                             llvm::Value *rightValue);

  llvm::Constant *getStringLiteral(std::string str);

  llvm::FunctionCallee printFun();

  bool isString(llvm::Value *value);
  bool isInt(llvm::Value *value);
  bool isFloat(llvm::Value *value);
  bool isBool(llvm::Value *value);
  bool sameType(llvm::Value *value1, llvm::Value *value2);

  llvm::Value *getVar(std::string name);
  void insertVar(std::string name, llvm::Value *value);

  llvm::Value *makeConditionFromValue(llvm::Value* value);
};

} // namespace wasmabi

#endif // WASMABI_GENERATOR_HPP
