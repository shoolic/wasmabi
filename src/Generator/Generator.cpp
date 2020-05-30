#include "Generator.hpp"
#include "../Nodes/Node.hpp"

namespace wasmabi {

Generator::Generator(std::ostream &output_)
    : output(output_), /* builder(context),*/
      module(std::make_unique<llvm::Module>("wasmabi", context)) {}

// llvm::Value* gen(Node &node);
llvm::Value *Generator::gen(Program &node) {

  for (auto &funDef : node.functionDefintions) {
    funDef->gen(*this);
  }

  for (auto &funDef : node.functionDefintions) {
    auto fun = module->getFunction(funDef->identifier);
    llvm::BasicBlock *functionBlock =
        llvm::BasicBlock::Create(context, "entry", fun);
    builder.SetInsertPoint(functionBlock);
    auto functionBlockValue = funDef->block->gen(*this);
    llvm::verifyFunction(*fun);
  }

  module->print(llvm::errs(), nullptr);
  return nullptr;
}

llvm::FunctionCallee Generator::printFun() {

  return module->getOrInsertFunction(
      "printf",
      llvm::FunctionType::get(
          llvm::IntegerType::getInt32Ty(context),
          llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0), true));
}

llvm::Function *Generator::gen(FunctionDefinition &node) {
  std::vector<llvm::Type *> params;
  for (auto &param : node.parameters) {
    params.push_back(gen(*param->type));
  }

  llvm::FunctionType *functionDeclaration =
      llvm::FunctionType::get(gen(*node.returnedType), params, false);

  llvm::Function *function = llvm::Function::Create(
      functionDeclaration, llvm::Function::ExternalLinkage, node.identifier,
      module.get());

  unsigned idx = 0;

  for (auto &arg : function->args()) {
    arg.setName(node.parameters[idx++]->identifier->name);
  }

  return function;
}

llvm::Value *Generator::gen(Identifier &node) { return nullptr; }

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
    throw std::exception();
  }
}

llvm::Constant *Generator::getStringLiteral(std::string str) {
  auto charType = llvm::Type::getInt8Ty(context);
  auto stringType = llvm::ArrayType::get(charType, str.size() + 1);

  auto stringLiteral = module->getOrInsertGlobal(str, stringType);
  if (stringLiteral) {
    return llvm::ConstantExpr::getBitCast(stringLiteral,
                                          llvm::Type::getInt8PtrTy(context));
  }

  std::vector<llvm::Constant *> chars(str.size());
  for (std::size_t i = 0; i < str.size(); i++) {
    chars[i] = llvm::ConstantInt::get(charType, str[i]);
  }
  chars.push_back(llvm::ConstantInt::get(charType, 0));

  auto stringInitializer = llvm::ConstantArray::get(stringType, chars);

  stringLiteral = new llvm::GlobalVariable(
      *module, stringType, true, llvm::GlobalVariable::ExternalLinkage,
      stringInitializer, str);

  return llvm::ConstantExpr::getBitCast(stringLiteral,
                                        llvm::Type::getInt8PtrTy(context));
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
    throw std::exception();
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
    throw std::exception();
  }
}

llvm::Value *Generator::gen(Block &node) {

  for (auto &instr : node.instructions) {
    if (instr.index() == 0) {
      std::get<std::unique_ptr<Block>>(instr)->gen(*this);
    }
    if (instr.index() == 1) {
      std::get<std::unique_ptr<Statement>>(instr)->gen(*this);
    }
  }

  return nullptr;
}

llvm::Value *Generator::gen(FunctionDefinitionParameter &node) {
  return nullptr;
}
llvm::Value *Generator::gen(NullExpression &node) {
  return builder.CreateRetVoid();
}

llvm::Value *Generator::gen(FunctionCallExpression &node) {
  std::vector<llvm::Value *> params;

  for (auto &param : node.parameters) {
    params.push_back(param->gen(*this));
  }

  auto function = module->getFunction(node.identifier->name);

  return builder.CreateCall(function, params);
}

llvm::Value *Generator::gen(IdentifierAsExpression &node) {
  // check if exists!!

  auto it = values.find(node.identifier->name);
  if (it == values.end()) {
    std::runtime_error("no variable with given name");
  }

  return builder.CreateLoad(it->second, node.identifier->name.c_str());
}

// llvm::Value* Generator::gen(ValueExpression &node) { return nullptr; }

llvm::Value *Generator::gen(UnaryExpression &node) {

  auto value = node.operand->gen(*this);

  if (isString(value)) {
    throw std::runtime_error("cannot use string value as operand");
  }

  switch (node.type) {
  case UnaryExpression::Type::Not:

    return builder.CreateNot(value);
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
    throw std::runtime_error("cannot use string value as operand");
  }

  if (isFloat(leftValue) && isFloat(rightValue)) {
    return genFloatBinExpr(node, leftValue, rightValue);
  }

  if (isInt(leftValue) && isInt(rightValue)) {
    return genIntBinExpr(node, leftValue, rightValue);
  }

  auto currentBlock = builder.GetInsertBlock()->getParent();

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
    throw std::runtime_error("not implemented");
    return nullptr;

  case BinaryExpression::Type::And:
    llvm::CastInst::Create(llvm::CastInst::FPToSI, leftValue,
                           builder.getInt1Ty());
    llvm::CastInst::Create(llvm::CastInst::FPToSI, rightValue,
                           builder.getInt1Ty());
    tmp = builder.CreateAnd(leftValue, rightValue);
    return builder.CreateZExt(tmp, builder.getInt32Ty());

  case BinaryExpression::Type::Or:
    llvm::CastInst::Create(llvm::CastInst::FPToSI, leftValue,
                           builder.getInt1Ty());
    llvm::CastInst::Create(llvm::CastInst::FPToSI, rightValue,
                           builder.getInt1Ty());
    tmp = builder.CreateOr(leftValue, rightValue);
    return builder.CreateZExt(tmp, builder.getInt32Ty());

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
    throw std::runtime_error("not implemented");
    return nullptr;
  case BinaryExpression::Type::And:
    throw std::runtime_error("");

  case BinaryExpression::Type::Or:
    throw std::runtime_error("");

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

  auto currentBlock = builder.GetInsertBlock()->getParent();

  for (auto &selectCase : node.cases) {
    auto selectcase =
        llvm::BasicBlock::Create(context, "selectcase", currentBlock);
    auto selectcaseblock =
        llvm::BasicBlock::Create(context, "selectcaseblock", currentBlock);

    selectCases.push_back(selectcase);
    selectCaseBlocks.push_back(selectcaseblock);
  }

  auto otherwiseblock =
      llvm::BasicBlock::Create(context, "selectotherwiseblock", currentBlock);
  selectCases.push_back(otherwiseblock);
  selectCaseBlocks.push_back(otherwiseblock);

  auto selectcont =
      llvm::BasicBlock::Create(context, "selectcont", currentBlock);

  std::size_t idx = 0;
  for (auto &selectCase : node.cases) {

    builder.SetInsertPoint(selectCases[idx]);
    llvm::Value *conditionValue = selectCase->condition->gen(*this);
    builder.CreateCondBr(conditionValue, selectCaseBlocks[idx],
                         selectCases[idx + 1]);

    builder.SetInsertPoint(selectCaseBlocks[idx]);
    llvm::Value *selectCaseValue = selectCase->value->gen(*this);
    selectCaseValues.push_back(selectCaseValue);
    builder.CreateBr(selectcont);

    idx++;
  }

  builder.SetInsertPoint(otherwiseblock);
  llvm::Value *otherwiseValue = node.otherwiseCaseValue->gen(*this);
  selectCaseValues.push_back(otherwiseValue);

  builder.SetInsertPoint(selectcont);

  auto PHINode = builder.CreatePHI(otherwiseValue->getType(),
                                   selectCases.size(), "selecttmp");

  for (std::size_t i = 0; i < selectCases.size(); i++) {
    PHINode->addIncoming(selectCaseValues[i], selectCaseBlocks[i]);
  }

  return PHINode;
}

llvm::Value *Generator::gen(SelectExpressionCase &node) { return nullptr; }
// llvm::Value* Generator::gen(Statement &node) { return nullptr; }
llvm::Value *Generator::gen(LoopStatement &node) {
  auto currentBlock = builder.GetInsertBlock()->getParent();

  llvm::BasicBlock *loopBlock =
      llvm::BasicBlock::Create(context, "loopblock", currentBlock);

  llvm::BasicBlock *loopContinueBlock =
      llvm::BasicBlock::Create(context, "loopcont");

  builder.SetInsertPoint(loopBlock);
  auto conditionValue = node.condition->gen(*this);
  builder.CreateCondBr(conditionValue, loopBlock, loopContinueBlock);
  auto ifBlockValue = node.block->gen(*this);
  builder.CreateBr(loopBlock);

  builder.SetInsertPoint(loopContinueBlock);
  currentBlock->getBasicBlockList().push_back(loopContinueBlock);

  return loopBlock;
}

llvm::Value *Generator::gen(IfStatement &node) {
  auto conditionValue = node.condition->gen(*this);
  // conditionValue = builder.CreateFCmpONE(
  //     conditionValue, llvm::ConstantFP::get(context, llvm::APFloat(0.0)));

  auto currentBlock = builder.GetInsertBlock()->getParent();
  llvm::BasicBlock *ifBlock =
      llvm::BasicBlock::Create(context, "ifblock", currentBlock);

  llvm::BasicBlock *ifContinueBlock =
      llvm::BasicBlock::Create(context, "ifcont");

  builder.CreateCondBr(conditionValue, ifBlock, ifContinueBlock);
  builder.SetInsertPoint(ifBlock);

  auto ifBlockValue = node.block->gen(*this);
  // builder.CreateBr(ifContinueBlock);

  builder.SetInsertPoint(ifContinueBlock);
  currentBlock->getBasicBlockList().push_back(ifContinueBlock);

  return nullptr;
}

llvm::Value *Generator::gen(ReturnStatement &node) {
  auto function = builder.GetInsertBlock()->getParent();

  if (node.value) {
    auto retVal = node.value->gen(*this);
    if (function->getReturnType() != retVal->getType()) {
      throw std::exception();
    }
    return builder.CreateRet(retVal);
  }

  if (function->getType() != llvm::Type::getVoidTy(context)) {
    throw std::exception();
  }

  return builder.CreateRetVoid();
}

llvm::Value *Generator::gen(PrintStatement &node) {
  auto value = node.value->gen(*this);

  if (value->getType() == llvm::Type::getInt8PtrTy(context)) {
    std::vector<llvm::Value *> args{getStringLiteral("%s"), value};
    return builder.CreateCall(printFun(), args);
  }

  if (value->getType() == llvm::Type::getInt32Ty(context)) {
    std::vector<llvm::Value *> args{getStringLiteral("%d"), value};
    return builder.CreateCall(printFun(), args);
  }

  if (value->getType() == llvm::Type::getFloatTy(context)) {
    std::vector<llvm::Value *> args{getStringLiteral("%f"), value};
    return builder.CreateCall(printFun(), args);
  }

  return nullptr;
}

llvm::Value *Generator::gen(VariableDefinitionStatement &node) {
  llvm::Constant *rValue;

  switch (node.type->type) {
  case VariableType::Type::Float:
    rValue = llvm::ConstantFP::get(context, llvm::APFloat(0.0));
  case VariableType::Type::Int:
    rValue = llvm::ConstantInt::get(context, llvm::APInt(32, 0));
  case VariableType::Type::String:
    rValue = getStringLiteral("");
  }

  llvm::AllocaInst *allocVar =
      builder.CreateAlloca(gen(*node.type), 0, node.identifier->name);

  llvm::Value *fullValue = builder.CreateStore(rValue, allocVar);

  values[node.identifier->name] = allocVar;

  return fullValue;
}

llvm::Value *Generator::gen(VariableDefinitionWithAssignmentStatement &node) {
  llvm::Value *rValue = node.value->gen(*this);
  llvm::AllocaInst *allocVar =
      builder.CreateAlloca(gen(*node.type), 0, node.identifier->name);

  const auto *rValuePtr = rValue->getType()->getPointerTo();

  if (rValuePtr != allocVar->getType()) {
    throw std::exception();
  }

  llvm::StoreInst *fullValue = builder.CreateStore(rValue, allocVar);

  values[node.identifier->name] = allocVar;

  return fullValue;
}

llvm::Value *Generator::gen(VariableAssignmentStatement &node) {
  llvm::Value *rValue = node.value->gen(*this);

  llvm::Value *allocVar = values[node.identifier->name];

  if (allocVar->getType() != rValue->getType()) {
    throw std::runtime_error("cannot assign, types differ");
  }

  auto fullValue = builder.CreateStore(rValue, allocVar);

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

} // namespace wasmabi