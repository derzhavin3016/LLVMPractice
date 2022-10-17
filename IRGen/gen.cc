#include "gen.hh"
namespace irgen
{
void Generator::makeGlobalPtr(std::string_view name)
{
  pModule->getOrInsertGlobal(name, builder.getInt8PtrTy());
  auto *ptr = pModule->getNamedGlobal(name);
  ptr->setAlignment(llvm::MaybeAlign(8));
  ptr->setLinkage(llvm::GlobalVariable::ExternalLinkage);
  ptr->setInitializer(ptr->getNullValue(ptr->getType()));
  ptr->setConstant(false);
}

void Generator::makeGetCell()
{
  auto *func = makeFuncDecl({builder.getInt64Ty(), builder.getInt64Ty()},
                            builder.getInt1Ty(), "getCell");
  func->addRetAttr(llvm::Attribute::ZExt);

  auto *b2 = llvm::BasicBlock::Create(context, "", func);
  builder.SetInsertPoint(b2);

  // %3 = add i64 %0, 400
  auto *i3 = builder.CreateAdd(func->getArg(0), builder.getInt64(400));
  // %4 = urem i64 %3, 400
  auto *i4 = builder.CreateURem(i3, builder.getInt64(400));
  // %5 = add i64 %1, 400
  auto *i5 = builder.CreateAdd(func->getArg(1), builder.getInt64(400));
  // %6 = urem i64 %5, 400
  auto *i6 = builder.CreateURem(i5, builder.getInt64(400));
  // %7 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  auto *i7 =
    builder.CreateLoad(builder.getInt8PtrTy(), getGlobVar("pActiveField"));
  setAlignment(i7, 8);
  // %8 = mul nuw nsw i64 %6, 400
  auto *i8 = builder.CreateMul(i6, builder.getInt64(400), "", true, true);
  // %9 = add nuw nsw i64 %8, %4
  auto *i9 = builder.CreateAdd(i8, i4, "", true, true);
  // %10 = getelementptr inbounds i8, i8* %7, i64 %9
  auto *i10 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i7, i9);
  // %11 = load i8, i8* %10, align 1, !tbaa !9, !range !11
  auto *i11 = builder.CreateLoad(builder.getInt8Ty(), i10);
  // %12 = icmp ne i8 %11, 0
  auto *i12 = builder.CreateICmpNE(i11, builder.getInt8(0));
  // ret i1 %12
  builder.CreateRet(i12);
}

void Generator::makeGenRandomBool()
{
  makeVoidFuncDecl(builder.getInt1Ty(), "genRandomBool")
    ->addRetAttr(llvm::Attribute::ZExt);
}

void Generator::makeFillField()
{
  auto *func = makeVoidFuncDecl(builder.getVoidTy(), "fillField");

  auto *b0 = llvm::BasicBlock::Create(context, "", func);
  auto *b1 = llvm::BasicBlock::Create(context, "", func);
  builder.SetInsertPoint(b0);

  // br label %1
  builder.CreateBr(b1);

  builder.SetInsertPoint(b1);
  // %2 = phi i64 [ 0, %0 ], [ %6, %5 ]
  auto *i2 = builder.CreatePHI(builder.getInt64Ty(), 2);
  i2->addIncoming(builder.getInt64(0), b0);
  // %3 = mul nuw nsw i64 %2, 400
  auto *i3 = builder.CreateMul(i2, builder.getInt64(400), "", true, true);
  /**/
  auto *b4 = llvm::BasicBlock::Create(context, "", func);
  builder.SetInsertPoint(b4);
  // ret void
  builder.CreateRetVoid();

  auto *b5 = llvm::BasicBlock::Create(context, "", func);
  builder.SetInsertPoint(b5);
  // %6 = add nuw nsw i64 %2, 1
  auto *i6 = builder.CreateAdd(i2, builder.getInt64(1), "", true, true);
  i2->addIncoming(i6, b5);
  // %7 = icmp eq i64 %6, 400
  auto *i7 = builder.CreateICmpEQ(i6, builder.getInt64(400));
  // br i1 %7, label %4, label %1, !llvm.loop !12
  builder.CreateCondBr(i7, b4, b1);

  auto *b8 = llvm::BasicBlock::Create(context, "", func);
  builder.SetInsertPoint(b1);
  // br label %8 -> From b1
  builder.CreateBr(b8);
  /**/
  builder.SetInsertPoint(b8);

  // %9 = phi i64 [ 0, %1 ], [ %15, %8 ]
  auto *i9 = builder.CreatePHI(builder.getInt64Ty(), 2);
  i9->addIncoming(builder.getInt64(0), b1);
  // %10 = tail call zeroext i1 @genRandomBool() #7
  auto *i10 = builder.CreateCall(getFunc("genRandomBool"));
  i10->setTailCall();
  // %11 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  auto *i11 =
    builder.CreateLoad(builder.getInt8PtrTy(), getGlobVar("pActiveField"));
  setAlignment(i11, 8);
  // %12 = add nuw nsw i64 %3, %9
  auto *i12 = builder.CreateAdd(i3, i9, "", true, true);
  // %13 = getelementptr inbounds i8, i8* %11, i64 %12
  auto *i13 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i11, i12);
  // %14 = zext i1 %10 to i8
  auto *i14 = builder.CreateZExt(i10, builder.getInt8Ty());
  // store i8 %14, i8* %13, align 1, !tbaa !9
  builder.CreateStore(i14, i13);
  // %15 = add nuw nsw i64 %9, 1
  auto *i15 = builder.CreateAdd(i9, builder.getInt64(1), "", true, true);
  i9->addIncoming(i15, b8);
  // %16 = icmp eq i64 %15, 400
  auto *i16 = builder.CreateICmpEQ(i15, builder.getInt64(400));
  // br i1 %16, label %5, label %8
  builder.CreateCondBr(i16, b5, b8);
}

void Generator::makeGetNeighbours()
{
  auto *func = makeFuncDecl({builder.getInt32Ty(), builder.getInt32Ty()},
                            builder.getInt8Ty(), "getNeighbours");
  func->addRetAttr(llvm::Attribute::ZExt);
  auto *mainBB = llvm::BasicBlock::Create(context, "", func);
  builder.SetInsertPoint(mainBB);

  // %3 = zext i32 %0 to i64
  auto *i3 = builder.CreateZExt(func->getArg(0), builder.getInt64Ty());
  // %4 = zext i32 %1 to i64
  auto *i4 = builder.CreateZExt(func->getArg(1), builder.getInt64Ty());
  // %5 = add nuw nsw i64 %3, 399
  auto *i5 = builder.CreateAdd(i3, builder.getInt64(399), "", true, true);
  // %6 = urem i64 %5, 400
  auto *i6 = builder.CreateURem(i5, builder.getInt64(400));
  // %7 = add nuw nsw i64 %4, 399
  auto *i7 = builder.CreateAdd(i4, builder.getInt64(399), "", true, true);
  // %8 = urem i64 %7, 400
  auto *i8 = builder.CreateURem(i7, builder.getInt64(400));
  // %9 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  auto *i9 =
    builder.CreateLoad(builder.getInt8PtrTy(), getGlobVar("pActiveField"));
  setAlignment(i9, 8);
  // %10 = mul nuw nsw i64 %8, 400
  auto *i10 = builder.CreateMul(i8, builder.getInt64(400), "", true, true);
  // %11 = add nuw nsw i64 %10, %6
  auto *i11 = builder.CreateAdd(i10, i6, "", true, true);
  // %12 = getelementptr inbounds i8, i8* %9, i64 %11
  auto *i12 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i9, i11);
  // %13 = load i8, i8* %12, align 1, !tbaa !9, !range !11
  auto *i13 = builder.CreateLoad(builder.getInt8Ty(), i12);
  // %14 = add nuw nsw i64 %3, 400
  auto *i14 = builder.CreateAdd(i3, builder.getInt64(400), "", true, true);
  // %15 = urem i64 %14, 400
  auto *i15 = builder.CreateURem(i14, builder.getInt64(400));
  // %16 = add nuw nsw i64 %10, %15
  auto *i16 = builder.CreateAdd(i10, i15, "", true, true);
  // %17 = getelementptr inbounds i8, i8* %9, i64 %16
  auto *i17 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i9, i16);
  // %18 = load i8, i8* %17, align 1, !tbaa !9, !range !11
  auto *i18 = builder.CreateLoad(builder.getInt8Ty(), i17);
  // %19 = add nuw nsw i8 %18, %13
  auto *i19 = builder.CreateAdd(i18, i13, "", true, true);
  // %20 = add nuw nsw i64 %3, 401
  auto *i20 = builder.CreateAdd(i3, builder.getInt64(401), "", true, true);
  // %21 = urem i64 %20, 400
  auto *i21 = builder.CreateURem(i20, builder.getInt64(400));
  // %22 = add nuw nsw i64 %10, %21
  auto *i22 = builder.CreateAdd(i10, i21, "", true, true);
  // %23 = getelementptr inbounds i8, i8* %9, i64 %22
  auto *i23 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i9, i22);
  // %24 = load i8, i8* %23, align 1, !tbaa !9, !range !11
  auto *i24 = builder.CreateLoad(builder.getInt8Ty(), i23);
  // %25 = add nuw nsw i8 %19, %24
  auto *i25 = builder.CreateAdd(i19, i24, "", true, true);
  // %26 = add nuw nsw i64 %4, 400
  auto *i26 = builder.CreateAdd(i4, builder.getInt64(400), "", true, true);
  // %27 = urem i64 %26, 400
  auto *i27 = builder.CreateURem(i26, builder.getInt64(400));
  // %28 = mul nuw nsw i64 %27, 400
  auto *i28 = builder.CreateMul(i27, builder.getInt64(400), "", true, true);
  // %29 = add nuw nsw i64 %28, %21
  auto *i29 = builder.CreateAdd(i28, i21, "", true, true);
  // %30 = getelementptr inbounds i8, i8* %9, i64 %29
  auto *i30 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i9, i29);
  // %31 = load i8, i8* %30, align 1, !tbaa !9, !range !11
  auto *i31 = builder.CreateLoad(builder.getInt8Ty(), i30);
  // %32 = add nuw nsw i8 %25, %31
  auto *i32 = builder.CreateAdd(i25, i31, "", true, true);
  // %33 = add nuw nsw i64 %4, 401
  auto *i33 = builder.CreateAdd(i4, builder.getInt64(401), "", true, true);
  // %34 = urem i64 %33, 400
  auto *i34 = builder.CreateURem(i33, builder.getInt64(400));
  // %35 = mul nuw nsw i64 %34, 400
  auto *i35 = builder.CreateMul(i34, builder.getInt64(400), "", true, true);
  // %36 = add nuw nsw i64 %35, %21
  auto *i36 = builder.CreateAdd(i35, i21, "", true, true);
  // %37 = getelementptr inbounds i8, i8* %9, i64 %36
  auto *i37 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i9, i36);
  // %38 = load i8, i8* %37, align 1, !tbaa !9, !range !11
  auto *i38 = builder.CreateLoad(builder.getInt8Ty(), i37);
  // %39 = add nuw nsw i8 %32, %38
  auto *i39 = builder.CreateAdd(i32, i38, "", true, true);
  // %40 = add nuw nsw i64 %35, %15
  auto *i40 = builder.CreateAdd(i35, i15, "", true, true);
  // %41 = getelementptr inbounds i8, i8* %9, i64 %40
  auto *i41 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i9, i40);
  // %42 = load i8, i8* %41, align 1, !tbaa !9, !range !11
  auto *i42 = builder.CreateLoad(builder.getInt8Ty(), i41);
  // %43 = add nuw nsw i8 %39, %42
  auto *i43 = builder.CreateAdd(i39, i42, "", true, true);
  // %44 = add nuw nsw i64 %35, %6
  auto *i44 = builder.CreateAdd(i35, i6, "", true, true);
  // %45 = getelementptr inbounds i8, i8* %9, i64 %44
  auto *i45 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i9, i44);
  // %46 = load i8, i8* %45, align 1, !tbaa !9, !range !11
  auto *i46 = builder.CreateLoad(builder.getInt8Ty(), i45);
  // %47 = add nuw nsw i8 %43, %46
  auto *i47 = builder.CreateAdd(i43, i46, "", true, true);
  // %48 = add nuw nsw i64 %28, %6
  auto *i48 = builder.CreateAdd(i28, i6, "", true, true);
  // %49 = getelementptr inbounds i8, i8* %9, i64 %48
  auto *i49 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i9, i48);
  // %50 = load i8, i8* %49, align 1, !tbaa !9, !range !11
  auto *i50 = builder.CreateLoad(builder.getInt8Ty(), i49);
  // %51 = add nuw nsw i8 %47, %50
  auto *i51 = builder.CreateAdd(i47, i50, "", true, true);
  // ret i8 %51
  builder.CreateRet(i51);
}

void Generator::makeNewGeneration()
{
  auto *func = makeVoidFuncDecl(builder.getVoidTy(), "newGeneration");
  auto *b0 = llvm::BasicBlock::Create(context, "", func);
  builder.SetInsertPoint(b0);

  // %1 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  auto *i1 =
    builder.CreateLoad(builder.getInt8PtrTy(), getGlobVar("pActiveField"));
  setAlignment(i1, 8);
  // %2 = load i8*, i8** @pNextField, align 8, !tbaa !5
  auto *i2 =
    builder.CreateLoad(builder.getInt8PtrTy(), getGlobVar("pNextField"));
  setAlignment(i2, 8);
  // br label %3
  auto *b3 = llvm::BasicBlock::Create(context, "", func);
  builder.CreateBr(b3);

  builder.SetInsertPoint(b3);
  // %4 = phi i64 [ 0, %0 ], [ %17, %16 ]
  auto *i4 = builder.CreatePHI(builder.getInt64Ty(), 2);
  i4->addIncoming(builder.getInt64(0), b0);
  // %5 = add nuw nsw i64 %4, 399
  auto *i5 = builder.CreateAdd(i4, builder.getInt64(399), "", true, true);
  // %6 = urem i64 %5, 400
  auto *i6 = builder.CreateURem(i5, builder.getInt64(400));
  // %7 = mul nuw nsw i64 %6, 400
  auto *i7 = builder.CreateMul(i6, builder.getInt64(400), "", true, true);
  // %8 = add nuw nsw i64 %4, 400
  auto *i8 = builder.CreateAdd(i4, builder.getInt64(400), "", true, true);
  // %9 = urem i64 %8, 400
  auto *i9 = builder.CreateURem(i8, builder.getInt64(400));
  // %10 = mul nuw nsw i64 %9, 400
  auto *i10 = builder.CreateMul(i9, builder.getInt64(400), "", true, true);
  // %11 = add nuw nsw i64 %4, 401
  auto *i11 = builder.CreateAdd(i4, builder.getInt64(401), "", true, true);
  // %12 = urem i64 %11, 400
  auto *i12 = builder.CreateURem(i11, builder.getInt64(400));
  // %13 = mul nuw nsw i64 %12, 400
  auto *i13 = builder.CreateMul(i12, builder.getInt64(400), "", true, true);
  // %14 = mul nuw nsw i64 %4, 400
  auto *i14 = builder.CreateMul(i4, builder.getInt64(400), "", true, true);

  /**/
  auto *b15 = llvm::BasicBlock::Create(context, "", func);
  builder.SetInsertPoint(b15);
  // ret void
  builder.CreateRetVoid();

  auto *b16 = llvm::BasicBlock::Create(context, "", func);
  builder.SetInsertPoint(b16);
  // %17 = add nuw nsw i64 %4, 1
  auto *i17 = builder.CreateAdd(i4, builder.getInt64(1), "", true, true);
  i4->addIncoming(i17, b16);
  // %18 = icmp eq i64 %17, 400
  auto *i18 = builder.CreateICmpEQ(i17, builder.getInt64(400));
  // br i1 %18, label %15, label %3, !llvm.loop !15
  builder.CreateCondBr(i18, b15, b3);

  auto *b19 = llvm::BasicBlock::Create(context, "", func);
  builder.SetInsertPoint(b3);
  // br label %19
  builder.CreateBr(b19);
  /**/

  builder.SetInsertPoint(b19);
  // %20 = phi i64 [ 0, %3 ], [ %69, %19 ]
  auto *i20 = builder.CreatePHI(builder.getInt64Ty(), 2);
  i20->addIncoming(builder.getInt64(0), b3);
  // %21 = add nuw nsw i64 %20, 399
  auto *i21 = builder.CreateAdd(i20, builder.getInt64(399), "", true, true);
  // %22 = urem i64 %21, 400
  auto *i22 = builder.CreateURem(i21, builder.getInt64(400));
  // %23 = add nuw nsw i64 %22, %7
  auto *i23 = builder.CreateAdd(i22, i7, "", true, true);
  // %24 = getelementptr inbounds i8, i8* %1, i64 %23
  auto *i24 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i1, i23);
  // %25 = load i8, i8* %24, align 1, !tbaa !9, !range !11
  auto *i25 = builder.CreateLoad(builder.getInt8Ty(), i24);
  // %26 = add nuw nsw i64 %20, 400
  auto *i26 = builder.CreateAdd(i20, builder.getInt64(400), "", true, true);
  // %27 = urem i64 %26, 400
  auto *i27 = builder.CreateURem(i26, builder.getInt64(400));
  // %28 = add nuw nsw i64 %27, %7
  auto *i28 = builder.CreateAdd(i27, i7, "", true, true);
  // %29 = getelementptr inbounds i8, i8* %1, i64 %28
  auto *i29 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i1, i28);
  // %30 = load i8, i8* %29, align 1, !tbaa !9, !range !11
  auto *i30 = builder.CreateLoad(builder.getInt8Ty(), i29);
  // %31 = add nuw nsw i8 %30, %25
  auto *i31 = builder.CreateAdd(i30, i25, "", true, true);
  // %32 = add nuw nsw i64 %20, 401
  auto *i32 = builder.CreateAdd(i20, builder.getInt64(401), "", true, true);
  // %33 = urem i64 %32, 400
  auto *i33 = builder.CreateURem(i32, builder.getInt64(400));
  // %34 = add nuw nsw i64 %33, %7
  auto *i34 = builder.CreateAdd(i33, i7, "", true, true);
  // %35 = getelementptr inbounds i8, i8* %1, i64 %34
  auto *i35 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i1, i34);
  // %36 = load i8, i8* %35, align 1, !tbaa !9, !range !11
  auto *i36 = builder.CreateLoad(builder.getInt8Ty(), i35);
  // %37 = add nuw nsw i8 %31, %36
  auto *i37 = builder.CreateAdd(i31, i36, "", true, true);
  // %38 = add nuw nsw i64 %33, %10
  auto *i38 = builder.CreateAdd(i33, i10, "", true, true);
  // %39 = getelementptr inbounds i8, i8* %1, i64 %38
  auto *i39 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i1, i38);
  // %40 = load i8, i8* %39, align 1, !tbaa !9, !range !11
  auto *i40 = builder.CreateLoad(builder.getInt8Ty(), i39);
  // %41 = add nuw nsw i8 %37, %40
  auto *i41 = builder.CreateAdd(i37, i40, "", true, true);
  // %42 = add nuw nsw i64 %33, %13
  auto *i42 = builder.CreateAdd(i33, i13, "", true, true);
  // %43 = getelementptr inbounds i8, i8* %1, i64 %42
  auto *i43 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i1, i42);
  // %44 = load i8, i8* %43, align 1, !tbaa !9, !range !11
  auto *i44 = builder.CreateLoad(builder.getInt8Ty(), i43);
  // %45 = add nuw nsw i8 %41, %44
  auto *i45 = builder.CreateAdd(i41, i44, "", true, true);
  // %46 = add nuw nsw i64 %27, %13
  auto *i46 = builder.CreateAdd(i27, i13, "", true, true);
  // %47 = getelementptr inbounds i8, i8* %1, i64 %46
  auto *i47 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i1, i46);
  // %48 = load i8, i8* %47, align 1, !tbaa !9, !range !11
  auto *i48 = builder.CreateLoad(builder.getInt8Ty(), i47);
  // %49 = add nuw nsw i8 %45, %48
  auto *i49 = builder.CreateAdd(i45, i48, "", true, true);
  // %50 = add nuw nsw i64 %22, %13
  auto *i50 = builder.CreateAdd(i22, i13, "", true, true);
  // %51 = getelementptr inbounds i8, i8* %1, i64 %50
  auto *i51 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i1, i50);
  // %52 = load i8, i8* %51, align 1, !tbaa !9, !range !11
  auto *i52 = builder.CreateLoad(builder.getInt8Ty(), i51);
  // %53 = add nuw nsw i8 %49, %52
  auto *i53 = builder.CreateAdd(i49, i52, "", true, true);
  // %54 = add nuw nsw i64 %22, %10
  auto *i54 = builder.CreateAdd(i22, i10, "", true, true);
  // %55 = getelementptr inbounds i8, i8* %1, i64 %54
  auto *i55 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i1, i54);
  // %56 = load i8, i8* %55, align 1, !tbaa !9, !range !11
  auto *i56 = builder.CreateLoad(builder.getInt8Ty(), i55);
  // %57 = add nuw nsw i8 %53, %56
  auto *i57 = builder.CreateAdd(i53, i56, "", true, true);
  // %58 = add nuw nsw i64 %27, %10
  auto *i58 = builder.CreateAdd(i27, i10, "", true, true);
  // %59 = getelementptr inbounds i8, i8* %1, i64 %58
  auto *i59 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i1, i58);
  // %60 = load i8, i8* %59, align 1, !tbaa !9, !range !11
  auto *i60 = builder.CreateLoad(builder.getInt8Ty(), i59);
  // %61 = icmp eq i8 %60, 0
  auto *i61 = builder.CreateICmpEQ(i60, builder.getInt8(0));
  // %62 = and i8 %57, -2
  auto *i62 = builder.CreateAnd(i57, builder.getInt8(-2));
  // %63 = icmp eq i8 %62, 2
  auto *i63 = builder.CreateICmpEQ(i62, builder.getInt8(2));
  // %64 = icmp eq i8 %57, 3
  auto *i64 = builder.CreateICmpEQ(i57, builder.getInt8(3));
  // %65 = select i1 %61, i1 %64, i1 %63
  auto *i65 = builder.CreateSelect(i61, i64, i63);
  // %66 = add nuw nsw i64 %20, %14
  auto *i66 = builder.CreateAdd(i20, i14, "", true, true);
  // %67 = getelementptr inbounds i8, i8* %2, i64 %66
  auto *i67 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i2, i66);
  // %68 = zext i1 %65 to i8
  auto *i68 = builder.CreateZExt(i65, builder.getInt8Ty());
  // store i8 %68, i8* %67, align 1, !tbaa !9
  builder.CreateStore(i68, i67);
  // %69 = add nuw nsw i64 %20, 1
  auto *i69 = builder.CreateAdd(i20, builder.getInt64(1), "", true, true);
  i20->addIncoming(i69, b19);
  // %70 = icmp eq i64 %69, 400
  auto *i70 = builder.CreateICmpEQ(i69, builder.getInt64(400));
  // br i1 %70, label %16, label %19, !llvm.loop !16
  builder.CreateCondBr(i70, b16, b19);
}

void Generator::makePutPixel()
{
  auto *func = makeFuncDecl({builder.getInt32Ty(), builder.getInt32Ty(),
                             builder.getInt8Ty(), builder.getInt8Ty(),
                             builder.getInt8Ty()},
                            builder.getVoidTy(), "putPixel");

  func->addParamAttr(2, llvm::Attribute::ZExt);
  func->addParamAttr(3, llvm::Attribute::ZExt);
  func->addParamAttr(4, llvm::Attribute::ZExt);
}

void Generator::makeDrawField()
{
  auto *func = makeVoidFuncDecl(builder.getVoidTy(), "drawField");
  auto *b0 = llvm::BasicBlock::Create(context, "", func);
  auto *b1 = llvm::BasicBlock::Create(context, "", func);
  auto *b8 = llvm::BasicBlock::Create(context, "", func);
  auto *b9 = llvm::BasicBlock::Create(context, "", func);
  auto *b12 = llvm::BasicBlock::Create(context, "", func);
  auto *b23 = llvm::BasicBlock::Create(context, "", func);
  auto *b24 = llvm::BasicBlock::Create(context, "", func);
  auto *b25 = llvm::BasicBlock::Create(context, "", func);

  builder.SetInsertPoint(b0);
  builder.CreateBr(b1);

  builder.SetInsertPoint(b1);

  // %2 = phi i64 [ 0, %0 ], [ %10, %9 ]
  auto *i2 = builder.CreatePHI(builder.getInt64Ty(), 2);
  i2->addIncoming(builder.getInt64(0), b0);
  // %3 = add nuw nsw i64 %2, 400
  auto *i3 = builder.CreateAdd(i2, builder.getInt64(400), "", true, true);
  // %4 = urem i64 %3, 400
  auto *i4 = builder.CreateURem(i3, builder.getInt64(400));
  // %5 = mul nuw nsw i64 %4, 400
  auto *i5 = builder.CreateMul(i4, builder.getInt64(400), "", true, true);
  // %6 = trunc i64 %2 to i32
  auto *i6 = builder.CreateTrunc(i2, builder.getInt32Ty());
  // %7 = trunc i64 %2 to i32
  auto *i7 = builder.CreateTrunc(i2, builder.getInt32Ty());
  // br label %12
  builder.CreateBr(b12);

  builder.SetInsertPoint(b8);
  // ret void
  builder.CreateRetVoid();

  builder.SetInsertPoint(b9);
  // %10 = add nuw nsw i64 %2, 1
  auto *i10 = builder.CreateAdd(i2, builder.getInt64(1), "", true, true);
  i2->addIncoming(i10, b9);
  // %11 = icmp eq i64 %10, 400
  auto *i11 = builder.CreateICmpEQ(i10, builder.getInt64(400));
  // br i1 %11, label %8, label %1, !llvm.loop !17
  builder.CreateCondBr(i11, b8, b1);

  builder.SetInsertPoint(b12);
  // %13 = phi i32 [ 0, %1 ], [ %26, %25 ]
  auto *i13 = builder.CreatePHI(builder.getInt32Ty(), 2);
  i13->addIncoming(builder.getInt32(0), b1);
  // %14 = trunc i32 %13 to i16
  auto *i14 = builder.CreateTrunc(i13, builder.getInt16Ty());
  // %15 = add i16 %14, 400
  auto *i15 = builder.CreateAdd(i14, builder.getInt16(400));
  // %16 = urem i16 %15, 400
  auto *i16 = builder.CreateURem(i15, builder.getInt16(400));
  // %17 = zext i16 %16 to i64
  auto *i17 = builder.CreateZExt(i16, builder.getInt64Ty());
  // %18 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  auto *i18 =
    builder.CreateLoad(builder.getInt8PtrTy(), getGlobVar("pActiveField"));
  setAlignment(i18, 8);
  // %19 = add nuw nsw i64 %5, %17
  auto *i19 = builder.CreateAdd(i5, i17, "", true, true);
  // %20 = getelementptr inbounds i8, i8* %18, i64 %19
  auto *i20 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i18, i19);
  // %21 = load i8, i8* %20, align 1, !tbaa !9, !range !11
  auto *i21 = builder.CreateLoad(builder.getInt8Ty(), i20);
  // %22 = icmp eq i8 %21, 0
  auto *i22 = builder.CreateICmpEQ(i21, builder.getInt8(0));
  // br i1 %22, label %24, label %23
  builder.CreateCondBr(i22, b24, b23);

  builder.SetInsertPoint(b23);
  // tail call void @putPixel(i32 noundef %13, i32 noundef %6, i8 noundef
  // zeroext 0, i8 noundef zeroext -1, i8 noundef zeroext 0) #7
  auto *call = builder.CreateCall(
    getFunc("putPixel"),
    {i13, i6, builder.getInt8(0), builder.getInt8(-1), builder.getInt8(0)});

  call->addParamAttr(2, llvm::Attribute::ZExt);
  call->addParamAttr(3, llvm::Attribute::ZExt);
  call->addParamAttr(4, llvm::Attribute::ZExt);
  call->setTailCall();
  // br label %25
  builder.CreateBr(b25);

  builder.SetInsertPoint(b24);
  // tail call void @putPixel(i32 noundef %13, i32 noundef %7, i8 noundef
  // zeroext 0, i8 noundef zeroext 0, i8 noundef zeroext 0) #7
  auto *call2 = builder.CreateCall(
    getFunc("putPixel"),
    {i13, i7, builder.getInt8(0), builder.getInt8(0), builder.getInt8(0)});

  call2->addParamAttr(2, llvm::Attribute::ZExt);
  call2->addParamAttr(3, llvm::Attribute::ZExt);
  call2->addParamAttr(4, llvm::Attribute::ZExt);
  call2->setTailCall();

  // br label %25
  builder.CreateBr(b25);

  builder.SetInsertPoint(b25);
  // %26 = add nuw nsw i32 %13, 1
  auto *i26 = builder.CreateAdd(i13, builder.getInt32(1), "", true, true);
  i13->addIncoming(i26, b25);
  // %27 = icmp eq i32 %26, 400
  auto *i27 = builder.CreateICmpEQ(i26, builder.getInt32(400));
  // br i1 %27, label %9, label %12
  builder.CreateCondBr(i27, b9, b12);
}

void Generator::makeSwapActiveField()
{
  auto *func = makeVoidFuncDecl(builder.getVoidTy(), "swapActiveField");
  auto *b0 = llvm::BasicBlock::Create(context, "", func);
  builder.SetInsertPoint(b0);
  auto *active = getGlobVar("pActiveField");
  auto *next = getGlobVar("pNextField");

  // %1 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  auto *i1 = builder.CreateLoad(builder.getInt8PtrTy(), active);
  setAlignment(i1, 8);
  // %2 = load i8*, i8** @pNextField, align 8, !tbaa !5
  auto *i2 = builder.CreateLoad(builder.getInt8PtrTy(), next);
  setAlignment(i2, 8);
  // store i8* %2, i8** @pActiveField, align 8, !tbaa !5
  auto *storeActive = builder.CreateStore(i2, active);
  setAlignment(storeActive, 8);
  // store i8* %1, i8** @pNextField, align 8, !tbaa !5
  auto *storeNext = builder.CreateStore(i1, next);
  setAlignment(storeNext, 8);
  // ret void
  builder.CreateRetVoid();
}

void Generator::makeMain()
{
  auto *func = makeVoidFuncDecl(builder.getInt32Ty(), "main");
  auto *b0 = llvm::BasicBlock::Create(context, "", func);
  auto *b5 = llvm::BasicBlock::Create(context, "", func);
  auto *b8 = llvm::BasicBlock::Create(context, "", func);
  auto *b11 = llvm::BasicBlock::Create(context, "", func);
  auto *b13 = llvm::BasicBlock::Create(context, "", func);
  auto *b22 = llvm::BasicBlock::Create(context, "", func);
  auto *b23 = llvm::BasicBlock::Create(context, "", func);
  auto *b29 = llvm::BasicBlock::Create(context, "", func);
  auto *b32 = llvm::BasicBlock::Create(context, "", func);
  auto *b45 = llvm::BasicBlock::Create(context, "", func);
  auto *b49 = llvm::BasicBlock::Create(context, "", func);

  builder.SetInsertPoint(b0);
  auto *arrayType = llvm::ArrayType::get(builder.getInt8Ty(), 160000);
  // %1 = alloca [160000 x i8], align 16
  auto *i1 = builder.CreateAlloca(arrayType);
  setAlignment(i1, 16);
  // %2 = alloca [160000 x i8], align 16
  auto *i2 = builder.CreateAlloca(arrayType);
  setAlignment(i2, 16);
  // %3 = getelementptr inbounds [160000 x i8], [160000 x i8]* %1, i64 0, i64 0
  auto *i3 = builder.CreateInBoundsGEP(
    arrayType, i1, {builder.getInt64(0), builder.getInt64(0)});
  // call void @llvm.lifetime.start.p0i8(i64 160000, i8* nonnull %3) #7
  // call void @llvm.memset.p0i8.i64(i8* noundef nonnull align 16
  // dereferenceable(160000) %3, i8 0, i64 160000, i1 false)

  // %4 = getelementptr inbounds [160000 x i8], [160000 x i8]* %2, i64 0, i64 0
  auto *i4 = builder.CreateInBoundsGEP(
    arrayType, i2, {builder.getInt64(0), builder.getInt64(0)});
  // call void @llvm.lifetime.start.p0i8(i64 160000, i8* nonnull %4) #7
  // call void @llvm.memset.p0i8.i64(i8* noundef nonnull align 16
  // dereferenceable(160000) %4, i8 0, i64 160000, i1 false)

  // store i8* %3, i8** @pActiveField, align 8, !tbaa !5
  setAlignment(builder.CreateStore(i3, getGlobVar("pActiveField")), 8);
  // store i8* %4, i8** @pNextField, align 8, !tbaa !5
  setAlignment(builder.CreateStore(i4, getGlobVar("pNextField")), 8);
  // call void @createWindow(i32 noundef 400, i32 noundef 400) #7
  builder.CreateCall(getFunc("createWindow"),
                     {builder.getInt32(400), builder.getInt32(400)});
  // br label %5
  builder.CreateBr(b5);

  builder.SetInsertPoint(b5);
  // %6 = phi i64 [ 0, %0 ], [ %9, %8 ]
  auto *i6 = builder.CreatePHI(builder.getInt64Ty(), 2);
  i6->addIncoming(builder.getInt64(0), b0);
  // %7 = mul nuw nsw i64 %6, 400
  auto *i7 = builder.CreateMul(i6, builder.getInt64(400), "", true, true);
  // br label %13
  builder.CreateBr(b13);

  builder.SetInsertPoint(b8);
  // %9 = add nuw nsw i64 %6, 1
  auto *i9 = builder.CreateAdd(i6, builder.getInt64(1), "", true, true);
  i6->addIncoming(i9, b8);
  // %10 = icmp eq i64 %9, 400
  auto *i10 = builder.CreateICmpEQ(i9, builder.getInt64(400));
  // br i1 %10, label %11, label %5, !llvm.loop !12
  builder.CreateCondBr(i10, b11, b5);

  builder.SetInsertPoint(b11);
  // %12 = call zeroext i1 @isWindowOpen() #7
  auto *i12 = builder.CreateCall(getFunc("isWindowOpen"));
  i12->addRetAttr(llvm::Attribute::ZExt);
  // br i1 %12, label %22, label %49
  builder.CreateCondBr(i12, b22, b49);

  builder.SetInsertPoint(b13);
  // %14 = phi i64 [ 0, %5 ], [ %20, %13 ]
  auto *i14 = builder.CreatePHI(builder.getInt64Ty(), 2);
  i14->addIncoming(builder.getInt64(0), b5);
  // %15 = call zeroext i1 @genRandomBool() #7
  auto *i15 = builder.CreateCall(getFunc("genRandomBool"));
  i15->addRetAttr(llvm::Attribute::ZExt);
  // %16 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  auto *i16 =
    builder.CreateLoad(builder.getInt8PtrTy(), getGlobVar("pActiveField"));
  setAlignment(i16, 8);
  // %17 = add nuw nsw i64 %14, %7
  auto *i17 = builder.CreateAdd(i14, i7, "", true, true);
  // %18 = getelementptr inbounds i8, i8* %16, i64 %17
  auto *i18 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i16, i17);
  // %19 = zext i1 %15 to i8
  auto *i19 = builder.CreateZExt(i15, builder.getInt8Ty());
  // store i8 %19, i8* %18, align 1, !tbaa !9
  builder.CreateStore(i19, i18);
  // %20 = add nuw nsw i64 %14, 1
  auto *i20 = builder.CreateAdd(i14, builder.getInt64(1), "", true, true);
  i14->addIncoming(i20, b13);
  // %21 = icmp eq i64 %20, 400
  auto *i21 = builder.CreateICmpEQ(i20, builder.getInt64(400));
  // br i1 %21, label %8, label %13, !llvm.loop !14
  builder.CreateCondBr(i21, b8, b13);

  builder.SetInsertPoint(b22);
  // call void @handleEvents() #7
  builder.CreateCall(getFunc("handleEvents"));
  // call void @clearWindow(i8 noundef zeroext 0, i8 noundef zeroext 0, i8
  // noundef zeroext 0) #7
  auto *zeroInt8 = builder.getInt8(0);
  auto *clearCall =
    builder.CreateCall(getFunc("clearWindow"), {zeroInt8, zeroInt8, zeroInt8});
  clearCall->addParamAttr(0, llvm::Attribute::ZExt);
  clearCall->addParamAttr(1, llvm::Attribute::ZExt);
  clearCall->addParamAttr(2, llvm::Attribute::ZExt);
  // call void @newGeneration()
  builder.CreateCall(getFunc("newGeneration"));
  // br label %23
  builder.CreateBr(b23);

  builder.SetInsertPoint(b23);
  // %24 = phi i64 [ 0, %22 ], [ %30, %29 ]
  auto *i24 = builder.CreatePHI(builder.getInt64Ty(), 2);
  i24->addIncoming(builder.getInt64(0), b22);
  // %25 = add nuw nsw i64 %24, 400
  auto *i25 = builder.CreateAdd(i24, builder.getInt64(400), "", true, true);
  // %26 = urem i64 %25, 400
  auto *i26 = builder.CreateURem(i25, builder.getInt64(400));
  // %27 = mul nuw nsw i64 %26, 400
  auto *i27 = builder.CreateMul(i26, builder.getInt64(400), "", true, true);
  // %28 = trunc i64 %24 to i32
  auto *i28 = builder.CreateTrunc(i24, builder.getInt32Ty());
  // br label %32
  builder.CreateBr(b32);

  builder.SetInsertPoint(b29);
  // %30 = add nuw nsw i64 %24, 1
  auto *i30 = builder.CreateAdd(i24, builder.getInt64(1), "", true, true);
  i24->addIncoming(i30, b29);
  // %31 = icmp eq i64 %30, 400
  auto *i31 = builder.CreateICmpEQ(i30, builder.getInt64(400));
  // br i1 %31, label %45, label %23, !llvm.loop !17
  builder.CreateCondBr(i31, b45, b23);

  builder.SetInsertPoint(b32);

  // %33 = phi i32 [ 0, %23 ], [ %43, %32 ]
  auto *i33 = builder.CreatePHI(builder.getInt32Ty(), 2);
  i33->addIncoming(builder.getInt32(0), b23);
  // %34 = trunc i32 %33 to i16
  auto *i34 = builder.CreateTrunc(i33, builder.getInt16Ty());
  // %35 = add i16 %34, 400
  auto *i35 = builder.CreateAdd(i34, builder.getInt16(400));
  // %36 = urem i16 %35, 400
  auto *i36 = builder.CreateURem(i35, builder.getInt16(400));
  // %37 = zext i16 %36 to i64
  auto *i37 = builder.CreateZExt(i36, builder.getInt64Ty());
  // %38 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  auto *i38 =
    builder.CreateLoad(builder.getInt8PtrTy(), getGlobVar("pActiveField"));
  setAlignment(i38, 8);
  // %39 = add nuw nsw i64 %27, %37
  auto *i39 = builder.CreateAdd(i27, i37, "", true, true);
  // %40 = getelementptr inbounds i8, i8* %38, i64 %39
  auto *i40 = builder.CreateInBoundsGEP(builder.getInt8Ty(), i38, i39);
  // %41 = load i8, i8* %40, align 1, !tbaa !9, !range !11
  auto *i41 = builder.CreateLoad(builder.getInt8Ty(), i40);
  // %42 = sub nsw i8 0, %41
  auto *i42 = builder.CreateSub(zeroInt8, i41, "", false, true);
  // call void @putPixel(i32 noundef %33, i32 noundef %28, i8 noundef zeroext 0,
  // i8 noundef zeroext %42, i8 noundef zeroext 0) #7
  auto *pupiCall = builder.CreateCall(getFunc("putPixel"),
                                      {i33, i28, zeroInt8, i42, zeroInt8});
  pupiCall->addParamAttr(2, llvm::Attribute::ZExt);
  pupiCall->addParamAttr(3, llvm::Attribute::ZExt);
  pupiCall->addParamAttr(4, llvm::Attribute::ZExt);
  // %43 = add nuw nsw i32 %33, 1
  auto *i43 = builder.CreateAdd(i33, builder.getInt32(1), "", true, true);
  i33->addIncoming(i43, b32);
  // %44 = icmp eq i32 %43, 400
  auto *i44 = builder.CreateICmpEQ(i43, builder.getInt32(400));
  // br i1 %44, label %29, label %32, !llvm.loop !18
  builder.CreateCondBr(i44, b29, b32);

  builder.SetInsertPoint(b45);
  // %46 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  auto *i46 =
    builder.CreateLoad(builder.getInt8PtrTy(), getGlobVar("pActiveField"));
  setAlignment(i46, 8);
  // %47 = load i8*, i8** @pNextField, align 8, !tbaa !5
  auto *i47 =
    builder.CreateLoad(builder.getInt8PtrTy(), getGlobVar("pNextField"));
  setAlignment(i47, 8);
  // store i8* %47, i8** @pActiveField, align 8, !tbaa !5
  setAlignment(builder.CreateStore(i47, getGlobVar("pActiveField")), 8);
  // store i8* %46, i8** @pNextField, align 8, !tbaa !5
  setAlignment(builder.CreateStore(i46, getGlobVar("pNextField")), 8);
  // call void @flushWindow() #7
  builder.CreateCall(getFunc("flushWindow"));
  // %48 = call zeroext i1 @isWindowOpen() #7
  auto *i48 = builder.CreateCall(getFunc("isWindowOpen"));
  i48->addRetAttr(llvm::Attribute::ZExt);
  // br i1 %48, label %22, label %49, !llvm.loop !19
  builder.CreateCondBr(i48, b22, b49);

  builder.SetInsertPoint(b49);
  // call void @llvm.lifetime.end.p0i8(i64 160000, i8* nonnull %4) #7
  // call void @llvm.lifetime.end.p0i8(i64 160000, i8* nonnull %3) #7

  // ret i32 0
  builder.CreateRet(builder.getInt32(0));
}

void Generator::makeIsWindowOpen()
{
  makeVoidFuncDecl(builder.getInt1Ty(), "isWindowOpen")
    ->addRetAttr(llvm::Attribute::ZExt);
}

void Generator::makeCreateWindow()
{
  makeFuncDecl({builder.getInt32Ty(), builder.getInt32Ty()},
               builder.getVoidTy(), "createWindow");
}

void Generator::makeHandleEvents()
{
  makeVoidFuncDecl(builder.getVoidTy(), "handleEvents");
}
void Generator::makeClearWindow()
{
  auto *func = makeFuncDecl(
    {builder.getInt8Ty(), builder.getInt8Ty(), builder.getInt8Ty()},
    builder.getVoidTy(), "clearWindow");
  func->addParamAttr(0, llvm::Attribute::ZExt);
  func->addParamAttr(1, llvm::Attribute::ZExt);
  func->addParamAttr(2, llvm::Attribute::ZExt);
}
void Generator::makeFlushWindow()
{
  makeVoidFuncDecl(builder.getVoidTy(), "flushWindow");
}

void Generator::generateIR()
{
  makeGlobalPtr("pActiveField");
  makeGlobalPtr("pNextField");

  makeGenRandomBool();

  makeGetCell();
  makeFillField();
  makeGetNeighbours();
  makeNewGeneration();

  makePutPixel();

  makeDrawField();
  makeSwapActiveField();

  makeCreateWindow();
  makeIsWindowOpen();
  makeHandleEvents();
  makeClearWindow();
  makeFlushWindow();

  makeMain();
}

void Generator::dump(std::ostream &ost) const
{
  std::string buffer;
  llvm::raw_string_ostream os(buffer);

  pModule->print(os, nullptr);
  os.flush();

  ost << buffer;
}

llvm::Function *Generator::makeFuncDecl(const std::vector<llvm::Type *> &params,
                                        llvm::Type *retTy,
                                        std::string_view name)
{
  llvm::ArrayRef<llvm::Type *> argsRef(params);
  auto *fType = llvm::FunctionType::get(retTy, argsRef, false);

  return llvm::Function::Create(
    fType, llvm::Function::LinkageTypes::ExternalLinkage, name, pModule);
}

llvm::Function *Generator::makeVoidFuncDecl(llvm::Type *retTy,
                                            std::string_view name)
{
  auto *fType = llvm::FunctionType::get(retTy, false);

  return llvm::Function::Create(
    fType, llvm::Function::LinkageTypes::ExternalLinkage, name, pModule);
}

llvm::GlobalVariable *Generator::getGlobVar(std::string_view name)
{
  auto *var = pModule->getGlobalVariable(name);
  if (var == nullptr)
  {
    std::ostringstream ss;
    ss << "Global variable with name " << name << " was not declared\n";
    throw std::logic_error(ss.str());
  }
  return var;
}

llvm::Function *Generator::getFunc(std::string_view name)
{
  auto *func = pModule->getFunction(name);
  if (func == nullptr)
  {
    std::ostringstream ss;
    ss << "Function with name " << name << " was not declared\n";
    throw std::logic_error(ss.str());
  }
  return func;
}
} // namespace irgen
