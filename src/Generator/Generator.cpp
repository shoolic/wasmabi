#include "Generator.hpp"
#include "../Errors/ErrorHandler.hpp"
#include "../Nodes/Node.hpp"
#include "llvm/Support/raw_os_ostream.h"

namespace wasmabi {

Generator::Generator(std::ostream &output_, std::string moduleName_,
                     std::string sourceFilePath_)
    : output(output_), moduleName(moduleName_), sourceFilePath(sourceFilePath_),
      module(std::make_unique<llvm::Module>(moduleName, context)) {}

void Generator::gen(Program &node) {

  for (auto &funDef : node.functionDefintions) {
    genFunHeader(*funDef);
  }

  for (auto &funDef : node.functionDefintions) {
    genFunBody(*funDef);
  }

  module->setSourceFileName(sourceFilePath);
  llvm::raw_os_ostream ostream(output);
  module->print(ostream, nullptr);
}

llvm::FunctionCallee Generator::printFun() {
  return module->getOrInsertFunction(
      "printf",
      llvm::FunctionType::get(
          llvm::IntegerType::getInt32Ty(context),
          llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0), true));
}

llvm::FunctionCallee Generator::powfFun() {
  return module->getOrInsertFunction(
      "powf", llvm::FunctionType::get(
                  builder.getFloatTy(),
                  {builder.getFloatTy(), builder.getFloatTy()}, false));
}

llvm::Value *Generator::powi(llvm::Value *base, llvm::Value *exponent) {
  base = builder.CreateCast(llvm::CastInst::SIToFP, base, builder.getFloatTy());
  exponent = builder.CreateCast(llvm::CastInst::SIToFP, exponent,
                                builder.getFloatTy());

  llvm::Value *powValue = builder.CreateCall(powfFun(), {base, exponent});

  powValue = builder.CreateCast(llvm::CastInst::FPToSI, powValue,
                                builder.getInt32Ty());
  return powValue;
}

void Generator::genFunHeader(FunctionDefinition &node) {
  std::vector<llvm::Type *> params;
  for (auto &param : node.parameters) {
    params.push_back(gen(*param->type));
  }

  llvm::FunctionType *functionDeclaration =
      llvm::FunctionType::get(gen(*node.returnedType), params, false);

  llvm::Function *function = llvm::Function::Create(
      functionDeclaration, llvm::Function::ExternalLinkage, node.identifier,
      module.get());
}

void Generator::genFunBody(FunctionDefinition &funDef) {
  const auto function = module->getFunction(funDef.identifier);
  llvm::BasicBlock *functionBlock =
      llvm::BasicBlock::Create(context, "entry", function);
  builder.SetInsertPoint(functionBlock);

  values.push_back(std::map<std::string, llvm::Value *>{});

  genFunParams(funDef);

  gen(*(funDef.block));

  genFunRet(funDef);

  llvm::verifyFunction(*function);

  values.pop_back();
}

void Generator::genFunParams(FunctionDefinition &funDef) {
  const auto function = module->getFunction(funDef.identifier);

  unsigned idx = 0;
  for (auto &arg : function->args()) {
    llvm::AllocaInst *alloca = builder.CreateAlloca(
        arg.getType(), 0, funDef.parameters[idx]->identifier.c_str());

    builder.CreateStore(static_cast<llvm::Value *>(&arg), alloca);
    insertVar(funDef.parameters[idx]->identifier, alloca);
    idx++;
  }
}

llvm::Value *Generator::genFunRet(FunctionDefinition &funDef) {

  switch (funDef.returnedType->type) {
  case FunctionType::Type::Float:
    return builder.CreateRet(llvm::ConstantFP::get(builder.getFloatTy(), 0.0));

  case FunctionType::Type::Int:
    return builder.CreateRet(builder.getInt32(0));
  case FunctionType::Type::String:
    return builder.CreateRet(getStringLiteral(""));
  case FunctionType::Type::Void:
    return builder.CreateRetVoid();
  default:
    return nullptr;
  }
}

llvm::Value *Generator::gen(Literal &node) {
  switch (node.type) {
  case Literal::Type::Float:
    return llvm::ConstantFP::get(context,
                                 llvm::APFloat(std::get<float>(node.value)));
  case Literal::Type::Int:
    return llvm::ConstantInt::get(context,
                                  llvm::APInt(32, (std::get<int>(node.value))));
  case Literal::Type::String:
    return getStringLiteral(std::get<std::string>(node.value));
  default:
    return nullptr;
  }
}

llvm::Constant *Generator::getStringLiteral(std::string str) {
  const auto it = globalStrings.find(str);
  if (it != globalStrings.end()) {
    return it->second;
  }

  const auto charType = llvm::Type::getInt8Ty(context);
  const auto stringType = llvm::ArrayType::get(charType, str.size() + 1);

  const auto stringInitializer =
      llvm::ConstantDataArray::getString(context, str.c_str(), true);

  const auto stringLiteral = new llvm::GlobalVariable(
      *module, stringType, true, llvm::GlobalVariable::PrivateLinkage,
      stringInitializer, str);

  const auto stringConst = llvm::ConstantExpr::getBitCast(
      stringLiteral, llvm::Type::getInt8PtrTy(context));

  globalStrings[str] = stringConst;

  return stringConst;
}

llvm::Type *Generator::gen(VariableType &node) {
  switch (node.type) {
  case VariableType::Type::Float:
    return llvm::Type::getFloatTy(context);
  case VariableType::Type::Int:
    return llvm::Type::getInt32Ty(context);
  case VariableType::Type::String:
    return llvm::Type::getInt8PtrTy(context);
  default:
    return nullptr;
  }
}

llvm::Type *Generator::gen(FunctionType &node) {
  switch (node.type) {
  case FunctionType::Type::Float:
    return llvm::Type::getFloatTy(context);
  case FunctionType::Type::Int:
    return llvm::Type::getInt32Ty(context);
  case FunctionType::Type::String:
    return llvm::Type::getInt8PtrTy(context);
  case FunctionType::Type::Void:
    return llvm::Type::getVoidTy(context);
  default:
    return nullptr;
  }
}

llvm::Value *Generator::gen(Block &node) {
  for (auto &instr : node.instructions) {
    if (instr.index() == 0) {
      values.push_back(std::map<std::string, llvm::Value *>{});
      gen(*std::get<std::unique_ptr<Block>>(instr));
      values.pop_back();
    }

    if (instr.index() == 1) {
      (std::get<std::unique_ptr<Statement>>(instr))->gen(*this);
    }
  }

  return builder.GetInsertBlock();
}

llvm::Value *Generator::gen(FunctionCallExpression &node) {
  std::vector<llvm::Value *> params;

  for (auto &param : node.parameters) {
    params.push_back(param->gen(*this));
  }

  const auto function = module->getFunction(node.identifier);

  if (!function) {
    throw UndefinedFunctionError(node.identifier);
  }

  return builder.CreateCall(function, params);
}

llvm::Value *Generator::gen(IdentifierAsExpression &node) {
  return builder.CreateLoad(getVar(node.identifier), node.identifier.c_str());
}

llvm::Value *Generator::gen(UnaryExpression &node) {

  auto value = node.operand->gen(*this);

  if (isString(value)) {
    throw GeneratorError("Cannot use string value as operand.");
  }

  switch (node.type) {
  case UnaryExpression::Type::Not:
    value = makeBoolFromValue(value);
    value = builder.CreateNot(value);
    value = makeInt32FromValue(value);
    return value;
    break;
  case UnaryExpression::Type::Minus:
    return builder.CreateNeg(value);
  default:
    break;
  }
  return nullptr;
}

llvm::Value *Generator::gen(BinaryExpression &node) {

  auto leftValue = node.leftOperand->gen(*this);
  auto rightValue = node.rightOperand->gen(*this);

  if (isString(leftValue) || isString(rightValue)) {
    throw StringAsOperandError();
  }

  if (isFloat(leftValue) && isFloat(rightValue)) {
    return genFloatBinExpr(node, leftValue, rightValue);
  }

  if (isInt(leftValue) && isInt(rightValue)) {
    return genIntBinExpr(node, leftValue, rightValue);
  }

  if (isInt(leftValue)) {

    leftValue = builder.CreateCast(llvm::CastInst::SIToFP, leftValue,
                                   builder.getFloatTy());
  } else if (isInt(rightValue)) {
    rightValue = builder.CreateCast(llvm::CastInst::SIToFP, rightValue,
                                    builder.getFloatTy());
  }

  return genFloatBinExpr(node, leftValue, rightValue);
}

llvm::Value *Generator::genFloatBinExpr(BinaryExpression &node,
                                        llvm::Value *leftValue,
                                        llvm::Value *rightValue) {

  llvm::Value *tmp;

  switch (node.type) {
  case BinaryExpression::Type::Add:
    return builder.CreateFAdd(leftValue, rightValue);
  case BinaryExpression::Type::Sub:
    return builder.CreateFSub(leftValue, rightValue);
  case BinaryExpression::Type::Mul:
    return builder.CreateFMul(leftValue, rightValue);
  case BinaryExpression::Type::Div:
    return builder.CreateFDiv(leftValue, rightValue);
  case BinaryExpression::Type::Pow:
    return builder.CreateCall(powfFun(), {leftValue, rightValue});
  case BinaryExpression::Type::And:
    leftValue = makeBoolFromValue(leftValue);
    rightValue = makeBoolFromValue(rightValue);
    tmp = builder.CreateAnd(leftValue, rightValue);
    tmp = makeInt32FromValue(tmp);
    return tmp;
  case BinaryExpression::Type::Or:
    leftValue = makeBoolFromValue(leftValue);
    rightValue = makeBoolFromValue(rightValue);
    tmp = builder.CreateOr(leftValue, rightValue);
    tmp = makeInt32FromValue(tmp);
    return tmp;

  case BinaryExpression::Type::Equals:
    tmp = builder.CreateFCmpUEQ(leftValue, rightValue);
    return builder.CreateZExt(tmp, builder.getInt32Ty());

  case BinaryExpression::Type::NotEquals:
    tmp = builder.CreateFCmpUNE(leftValue, rightValue);
    return builder.CreateZExt(tmp, builder.getInt32Ty());

  case BinaryExpression::Type::Greater:
    tmp = builder.CreateFCmpUGT(leftValue, rightValue);
    return builder.CreateZExt(tmp, builder.getInt32Ty());

  case BinaryExpression::Type::GreaterOrEqual:
    tmp = builder.CreateFCmpUGE(leftValue, rightValue);
    return builder.CreateZExt(tmp, builder.getInt32Ty());

  case BinaryExpression::Type::Less:
    tmp = builder.CreateFCmpULT(leftValue, rightValue);
    return builder.CreateZExt(tmp, builder.getInt32Ty());

  case BinaryExpression::Type::LessOrEqual:
    tmp = builder.CreateFCmpULE(leftValue, rightValue);
    return builder.CreateZExt(tmp, builder.getInt32Ty());

  default:
    break;
  }

  return nullptr;
}

llvm::Value *Generator::genIntBinExpr(BinaryExpression &node,
                                      llvm::Value *leftValue,
                                      llvm::Value *rightValue) {

  llvm::Value *tmp;
  switch (node.type) {
  case BinaryExpression::Type::Add:
    return builder.CreateAdd(leftValue, rightValue);
  case BinaryExpression::Type::Sub:
    return builder.CreateSub(leftValue, rightValue);
  case BinaryExpression::Type::Mul:
    return builder.CreateMul(leftValue, rightValue);
  case BinaryExpression::Type::Div:
    return builder.CreateSDiv(leftValue, rightValue);
  case BinaryExpression::Type::Pow:
    return powi(leftValue, rightValue);
  case BinaryExpression::Type::And:
    leftValue = makeBoolFromValue(leftValue);
    rightValue = makeBoolFromValue(rightValue);
    tmp = builder.CreateAnd(leftValue, rightValue);
    tmp = makeInt32FromValue(tmp);
    return tmp;
  case BinaryExpression::Type::Or:
    leftValue = makeBoolFromValue(leftValue);
    rightValue = makeBoolFromValue(rightValue);
    tmp = builder.CreateOr(leftValue, rightValue);
    tmp = makeInt32FromValue(tmp);
    return tmp;
  case BinaryExpression::Type::Equals:
    tmp = builder.CreateICmpEQ(leftValue, rightValue);
    return builder.CreateZExt(tmp, builder.getInt32Ty());

  case BinaryExpression::Type::NotEquals:
    tmp = builder.CreateICmpNE(leftValue, rightValue);
    return builder.CreateZExt(tmp, builder.getInt32Ty());

  case BinaryExpression::Type::Greater:
    tmp = builder.CreateICmpUGT(leftValue, rightValue);
    return builder.CreateZExt(tmp, builder.getInt32Ty());

  case BinaryExpression::Type::GreaterOrEqual:
    tmp = builder.CreateICmpUGE(leftValue, rightValue);
    return builder.CreateZExt(tmp, builder.getInt32Ty());

  case BinaryExpression::Type::Less:
    tmp = builder.CreateICmpULT(leftValue, rightValue);
    return builder.CreateZExt(tmp, builder.getInt32Ty());

  case BinaryExpression::Type::LessOrEqual:
    tmp = builder.CreateICmpULE(leftValue, rightValue);
    return builder.CreateZExt(tmp, builder.getInt32Ty());

  default:
    break;
  }

  return nullptr;
}

llvm::Value *Generator::gen(SelectExpression &node) {
  std::vector<llvm::BasicBlock *> selectCases;
  std::vector<llvm::Value *> selectCaseValues;
  std::vector<llvm::BasicBlock *> selectCaseBlocks;
  const auto function = builder.GetInsertBlock()->getParent();
  const auto currentBlock = builder.GetInsertBlock();

  for (auto &selectCase : node.cases) {
    const auto selectcase = llvm::BasicBlock::Create(context, "selectcase");
    const auto selectcaseblock =
        llvm::BasicBlock::Create(context, "selectcaseblock");

    selectCases.push_back(selectcase);
    selectCaseBlocks.push_back(selectcaseblock);
  }

  const auto otherwiseblock =
      llvm::BasicBlock::Create(context, "selectotherwiseblock");
  selectCases.push_back(otherwiseblock);
  selectCaseBlocks.push_back(otherwiseblock);

  const auto selectcont = llvm::BasicBlock::Create(context, "selectcont");

  builder.CreateBr(selectCases[0]);

  std::size_t idx = 0;
  for (auto &selectCase : node.cases) {

    builder.SetInsertPoint(selectCases[idx]);
    function->getBasicBlockList().push_back(selectCases[idx]);
    llvm::Value *conditionValue =
        makeBoolFromValue(selectCase->condition->gen(*this));

    builder.CreateCondBr(conditionValue, selectCaseBlocks[idx],
                         selectCases[idx + 1]);

    builder.SetInsertPoint(selectCaseBlocks[idx]);
    function->getBasicBlockList().push_back(selectCaseBlocks[idx]);

    llvm::Value *selectCaseValue = selectCase->value->gen(*this);
    selectCaseValues.push_back(selectCaseValue);
    builder.CreateBr(selectcont);

    idx++;
  }

  builder.SetInsertPoint(otherwiseblock);
  function->getBasicBlockList().push_back(otherwiseblock);

  llvm::Value *otherwiseValue = node.otherwiseCaseValue->gen(*this);
  builder.CreateBr(selectcont);
  selectCaseValues.push_back(otherwiseValue);

  builder.SetInsertPoint(selectcont);
  function->getBasicBlockList().push_back(selectcont);

  const auto PHINode = builder.CreatePHI(otherwiseValue->getType(),
                                         selectCases.size(), "selecttmp");

  for (std::size_t i = 0; i < selectCases.size(); i++) {
    PHINode->addIncoming(selectCaseValues[i], selectCaseBlocks[i]);
  }

  return PHINode;
}

llvm::Value *Generator::gen(LoopStatement &node) {
  const auto function = builder.GetInsertBlock()->getParent();

  llvm::BasicBlock *loop = llvm::BasicBlock::Create(context, "loop");

  llvm::BasicBlock *loopBlock = llvm::BasicBlock::Create(context, "loopblock");

  llvm::BasicBlock *loopContinueBlock =
      llvm::BasicBlock::Create(context, "loopcont");

  builder.CreateBr(loop);
  function->getBasicBlockList().push_back(loop);

  builder.SetInsertPoint(loop);
  const auto conditionValue = makeBoolFromValue(node.condition->gen(*this));
  builder.CreateCondBr(conditionValue, loopBlock, loopContinueBlock);

  builder.SetInsertPoint(loopBlock);
  function->getBasicBlockList().push_back(loopBlock);

  const auto loopBlockVal = gen(*node.block);
  builder.CreateBr(loop);

  builder.SetInsertPoint(loopContinueBlock);
  function->getBasicBlockList().push_back(loopContinueBlock);

  return loopContinueBlock;
}

llvm::Value *Generator::gen(IfStatement &node) {
  const auto function = builder.GetInsertBlock()->getParent();

  const auto conditionValue = makeBoolFromValue(node.condition->gen(*this));
  llvm::BasicBlock *ifBlock =
      llvm::BasicBlock::Create(context, "ifblock", function);

  llvm::BasicBlock *ifContinueBlock =
      llvm::BasicBlock::Create(context, "ifcont");

  builder.CreateCondBr(conditionValue, ifBlock, ifContinueBlock);
  builder.SetInsertPoint(ifBlock);

  const auto ifBlockValue = gen(*node.block);

  builder.SetInsertPoint(ifContinueBlock);
  function->getBasicBlockList().push_back(ifContinueBlock);

  return ifContinueBlock;
}

llvm::Value *Generator::gen(ReturnStatement &node) {
  const auto function = builder.GetInsertBlock()->getParent();

  if (node.value) {
    const auto retVal = node.value->gen(*this);
    if (function->getReturnType() != retVal->getType()) {
      throw FunctionReturnValueMismatchError();
    }
    return builder.CreateRet(retVal);
  }

  if (function->getType() != llvm::Type::getVoidTy(context)) {
    throw VoidFunretValueError();
  }

  return builder.CreateRetVoid();
}

llvm::Value *Generator::gen(PrintStatement &node) {
  const auto value = node.value->gen(*this);

  if (value->getType() == llvm::Type::getInt8PtrTy(context)) {
    std::vector<llvm::Value *> args{getStringLiteral("%s\n"), value};
    return builder.CreateCall(printFun(), args);
  }

  if (value->getType() == llvm::Type::getInt32Ty(context)) {
    std::vector<llvm::Value *> args{getStringLiteral("%d\n"), value};
    return builder.CreateCall(printFun(), args);
  }

  if (value->getType() == llvm::Type::getFloatTy(context)) {
    std::vector<llvm::Value *> args{getStringLiteral("%f\n"), value};
    return builder.CreateCall(printFun(), args);
  }

  throw UnpritableValueError();
}

llvm::Value *Generator::gen(VariableDefinitionStatement &node) {
  llvm::Value *rValue = node.value->gen(*this);
  llvm::AllocaInst *allocVar =
      builder.CreateAlloca(gen(*node.type), 0, node.identifier);

  const auto *rValuePtr = rValue->getType()->getPointerTo();

  if (rValuePtr != allocVar->getType()) {
    throw VarAssignTypeMismatch();
  }

  llvm::StoreInst *fullValue = builder.CreateStore(rValue, allocVar);

  insertVar(node.identifier, allocVar);

  return fullValue;
}

llvm::Value *Generator::gen(VariableAssignmentStatement &node) {
  llvm::Value *rValue = node.value->gen(*this);

  llvm::Value *allocVar = getVar(node.identifier);

  if (allocVar->getType() != rValue->getType()->getPointerTo()) {
    throw VarAssignTypeMismatch();
  }

  const auto fullValue = builder.CreateStore(rValue, allocVar);

  return fullValue;
}

llvm::Value *Generator::gen(FunctionCallStatement &node) {
  return node.functionCallExpression->gen(*this);
}

bool Generator::isString(llvm::Value *value) {
  return value->getType() == builder.getInt8PtrTy();
}

bool Generator::isInt(llvm::Value *value) {
  return value->getType() == builder.getInt32Ty();
}

bool Generator::isFloat(llvm::Value *value) {
  return value->getType() == builder.getFloatTy();
}

bool Generator::isBool(llvm::Value *value) {
  return value->getType() == builder.getInt8Ty();
}

bool Generator::sameType(llvm::Value *value1, llvm::Value *value2) {
  return value1->getType() == value2->getType();
}

llvm::Value *Generator::getVar(std::string name) const {

  for (auto it = values.rbegin(); it != values.rend(); it++) {
    const auto binding = it->find(name);

    if (binding != it->end()) {
      return binding->second;
    }
  }

  throw UndefinedVariableError(name);
}

void Generator::insertVar(std::string name, llvm::Value *value) {
  auto &map = values.back();
  const auto it = map.find(name);

  if (it != map.end()) {
    throw VariableRedefintionError(name);
  }

  map[name] = value;
}

llvm::Value *Generator::makeBoolFromValue(llvm::Value *value) {

  if (isString(value)) {
    throw StringAsConditionError();
  } else if (isInt(value)) {
    return builder.CreateIntCast(value, builder.getInt1Ty(), true);
  } else {
    return builder.CreateCast(llvm::CastInst::FPToUI, value,
                              builder.getInt1Ty());
  }
}

llvm::Value *Generator::makeInt32FromValue(llvm::Value *value) {
  return builder.CreateIntCast(value, builder.getInt32Ty(), false);
}

} // namespace wasmabi