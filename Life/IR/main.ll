; ModuleID = 'main.c'
source_filename = "main.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@pActiveField = dso_local global i8* null, align 8
@pNextField = dso_local global i8* null, align 8

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca [160000 x i8], align 16
  %3 = alloca [160000 x i8], align 16
  store i32 0, i32* %1, align 4
  %4 = bitcast [160000 x i8]* %2 to i8*
  call void @llvm.memset.p0i8.i64(i8* align 16 %4, i8 0, i64 160000, i1 false)
  %5 = bitcast [160000 x i8]* %3 to i8*
  call void @llvm.memset.p0i8.i64(i8* align 16 %5, i8 0, i64 160000, i1 false)
  %6 = getelementptr inbounds [160000 x i8], [160000 x i8]* %2, i64 0, i64 0
  store i8* %6, i8** @pActiveField, align 8
  %7 = getelementptr inbounds [160000 x i8], [160000 x i8]* %3, i64 0, i64 0
  store i8* %7, i8** @pNextField, align 8
  call void @createWindow(i32 noundef 400, i32 noundef 400)
  call void @fillField()
  br label %8

8:                                                ; preds = %10, %0
  %9 = call zeroext i1 @isWindowOpen()
  br i1 %9, label %10, label %11

10:                                               ; preds = %8
  call void @handleEvents()
  call void @clearWindow(i8 noundef zeroext 0, i8 noundef zeroext 0, i8 noundef zeroext 0)
  call void @newGeneration()
  call void @drawField()
  call void @swapActiveField()
  call void @flushWindow()
  br label %8, !llvm.loop !6

11:                                               ; preds = %8
  %12 = load i32, i32* %1, align 4
  ret i32 %12
}

; Function Attrs: argmemonly nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #1

declare void @createWindow(i32 noundef, i32 noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define internal void @fillField() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  br label %3

3:                                                ; preds = %27, %0
  %4 = load i32, i32* %1, align 4
  %5 = zext i32 %4 to i64
  %6 = icmp ult i64 %5, 400
  br i1 %6, label %7, label %30

7:                                                ; preds = %3
  store i32 0, i32* %2, align 4
  br label %8

8:                                                ; preds = %23, %7
  %9 = load i32, i32* %2, align 4
  %10 = zext i32 %9 to i64
  %11 = icmp ult i64 %10, 400
  br i1 %11, label %12, label %26

12:                                               ; preds = %8
  %13 = call zeroext i1 @genRandomBool()
  %14 = load i8*, i8** @pActiveField, align 8
  %15 = load i32, i32* %1, align 4
  %16 = zext i32 %15 to i64
  %17 = mul i64 %16, 400
  %18 = load i32, i32* %2, align 4
  %19 = zext i32 %18 to i64
  %20 = add i64 %17, %19
  %21 = getelementptr inbounds i8, i8* %14, i64 %20
  %22 = zext i1 %13 to i8
  store i8 %22, i8* %21, align 1
  br label %23

23:                                               ; preds = %12
  %24 = load i32, i32* %2, align 4
  %25 = add i32 %24, 1
  store i32 %25, i32* %2, align 4
  br label %8, !llvm.loop !8

26:                                               ; preds = %8
  br label %27

27:                                               ; preds = %26
  %28 = load i32, i32* %1, align 4
  %29 = add i32 %28, 1
  store i32 %29, i32* %1, align 4
  br label %3, !llvm.loop !9

30:                                               ; preds = %3
  ret void
}

declare zeroext i1 @isWindowOpen() #2

declare void @handleEvents() #2

declare void @clearWindow(i8 noundef zeroext, i8 noundef zeroext, i8 noundef zeroext) #2

; Function Attrs: noinline nounwind optnone uwtable
define internal void @newGeneration() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i32 0, i32* %1, align 4
  br label %5

5:                                                ; preds = %55, %0
  %6 = load i32, i32* %1, align 4
  %7 = zext i32 %6 to i64
  %8 = icmp ult i64 %7, 400
  br i1 %8, label %9, label %58

9:                                                ; preds = %5
  store i32 0, i32* %2, align 4
  br label %10

10:                                               ; preds = %51, %9
  %11 = load i32, i32* %2, align 4
  %12 = zext i32 %11 to i64
  %13 = icmp ult i64 %12, 400
  br i1 %13, label %14, label %54

14:                                               ; preds = %10
  %15 = load i32, i32* %2, align 4
  %16 = load i32, i32* %1, align 4
  %17 = call zeroext i8 @getNeighbours(i32 noundef %15, i32 noundef %16)
  store i8 %17, i8* %3, align 1
  store i8 0, i8* %4, align 1
  %18 = load i32, i32* %2, align 4
  %19 = zext i32 %18 to i64
  %20 = load i32, i32* %1, align 4
  %21 = zext i32 %20 to i64
  %22 = call zeroext i1 @getCell(i64 noundef %19, i64 noundef %21)
  br i1 %22, label %23, label %34

23:                                               ; preds = %14
  %24 = load i8, i8* %3, align 1
  %25 = zext i8 %24 to i32
  %26 = icmp eq i32 %25, 2
  br i1 %26, label %31, label %27

27:                                               ; preds = %23
  %28 = load i8, i8* %3, align 1
  %29 = zext i8 %28 to i32
  %30 = icmp eq i32 %29, 3
  br label %31

31:                                               ; preds = %27, %23
  %32 = phi i1 [ true, %23 ], [ %30, %27 ]
  %33 = zext i1 %32 to i8
  store i8 %33, i8* %4, align 1
  br label %39

34:                                               ; preds = %14
  %35 = load i8, i8* %3, align 1
  %36 = zext i8 %35 to i32
  %37 = icmp eq i32 %36, 3
  %38 = zext i1 %37 to i8
  store i8 %38, i8* %4, align 1
  br label %39

39:                                               ; preds = %34, %31
  %40 = load i8, i8* %4, align 1
  %41 = trunc i8 %40 to i1
  %42 = load i8*, i8** @pNextField, align 8
  %43 = load i32, i32* %1, align 4
  %44 = zext i32 %43 to i64
  %45 = mul i64 %44, 400
  %46 = load i32, i32* %2, align 4
  %47 = zext i32 %46 to i64
  %48 = add i64 %45, %47
  %49 = getelementptr inbounds i8, i8* %42, i64 %48
  %50 = zext i1 %41 to i8
  store i8 %50, i8* %49, align 1
  br label %51

51:                                               ; preds = %39
  %52 = load i32, i32* %2, align 4
  %53 = add i32 %52, 1
  store i32 %53, i32* %2, align 4
  br label %10, !llvm.loop !10

54:                                               ; preds = %10
  br label %55

55:                                               ; preds = %54
  %56 = load i32, i32* %1, align 4
  %57 = add i32 %56, 1
  store i32 %57, i32* %1, align 4
  br label %5, !llvm.loop !11

58:                                               ; preds = %5
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @drawField() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  br label %3

3:                                                ; preds = %29, %0
  %4 = load i32, i32* %1, align 4
  %5 = zext i32 %4 to i64
  %6 = icmp ult i64 %5, 400
  br i1 %6, label %7, label %32

7:                                                ; preds = %3
  store i32 0, i32* %2, align 4
  br label %8

8:                                                ; preds = %25, %7
  %9 = load i32, i32* %2, align 4
  %10 = zext i32 %9 to i64
  %11 = icmp ult i64 %10, 400
  br i1 %11, label %12, label %28

12:                                               ; preds = %8
  %13 = load i32, i32* %2, align 4
  %14 = zext i32 %13 to i64
  %15 = load i32, i32* %1, align 4
  %16 = zext i32 %15 to i64
  %17 = call zeroext i1 @getCell(i64 noundef %14, i64 noundef %16)
  br i1 %17, label %18, label %21

18:                                               ; preds = %12
  %19 = load i32, i32* %2, align 4
  %20 = load i32, i32* %1, align 4
  call void @putPixel(i32 noundef %19, i32 noundef %20, i8 noundef zeroext 0, i8 noundef zeroext -1, i8 noundef zeroext 0)
  br label %24

21:                                               ; preds = %12
  %22 = load i32, i32* %2, align 4
  %23 = load i32, i32* %1, align 4
  call void @putPixel(i32 noundef %22, i32 noundef %23, i8 noundef zeroext 0, i8 noundef zeroext 0, i8 noundef zeroext 0)
  br label %24

24:                                               ; preds = %21, %18
  br label %25

25:                                               ; preds = %24
  %26 = load i32, i32* %2, align 4
  %27 = add i32 %26, 1
  store i32 %27, i32* %2, align 4
  br label %8, !llvm.loop !12

28:                                               ; preds = %8
  br label %29

29:                                               ; preds = %28
  %30 = load i32, i32* %1, align 4
  %31 = add i32 %30, 1
  store i32 %31, i32* %1, align 4
  br label %3, !llvm.loop !13

32:                                               ; preds = %3
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @swapActiveField() #0 {
  %1 = alloca i8*, align 8
  %2 = load i8*, i8** @pActiveField, align 8
  store i8* %2, i8** %1, align 8
  %3 = load i8*, i8** @pNextField, align 8
  store i8* %3, i8** @pActiveField, align 8
  %4 = load i8*, i8** %1, align 8
  store i8* %4, i8** @pNextField, align 8
  ret void
}

declare void @flushWindow() #2

declare zeroext i1 @genRandomBool() #2

; Function Attrs: noinline nounwind optnone uwtable
define internal zeroext i8 @getNeighbours(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i64, align 8
  %6 = alloca i64, align 8
  %7 = alloca i8, align 1
  store i32 %0, i32* %3, align 4
  store i32 %1, i32* %4, align 4
  %8 = load i32, i32* %3, align 4
  %9 = zext i32 %8 to i64
  store i64 %9, i64* %5, align 8
  %10 = load i32, i32* %4, align 4
  %11 = zext i32 %10 to i64
  store i64 %11, i64* %6, align 8
  store i8 0, i8* %7, align 1
  %12 = load i64, i64* %5, align 8
  %13 = sub nsw i64 %12, 1
  %14 = load i64, i64* %6, align 8
  %15 = sub nsw i64 %14, 1
  %16 = call zeroext i1 @getCell(i64 noundef %13, i64 noundef %15)
  %17 = zext i1 %16 to i32
  %18 = load i8, i8* %7, align 1
  %19 = zext i8 %18 to i32
  %20 = add nsw i32 %19, %17
  %21 = trunc i32 %20 to i8
  store i8 %21, i8* %7, align 1
  %22 = load i64, i64* %5, align 8
  %23 = load i64, i64* %6, align 8
  %24 = sub nsw i64 %23, 1
  %25 = call zeroext i1 @getCell(i64 noundef %22, i64 noundef %24)
  %26 = zext i1 %25 to i32
  %27 = load i8, i8* %7, align 1
  %28 = zext i8 %27 to i32
  %29 = add nsw i32 %28, %26
  %30 = trunc i32 %29 to i8
  store i8 %30, i8* %7, align 1
  %31 = load i64, i64* %5, align 8
  %32 = add nsw i64 %31, 1
  %33 = load i64, i64* %6, align 8
  %34 = sub nsw i64 %33, 1
  %35 = call zeroext i1 @getCell(i64 noundef %32, i64 noundef %34)
  %36 = zext i1 %35 to i32
  %37 = load i8, i8* %7, align 1
  %38 = zext i8 %37 to i32
  %39 = add nsw i32 %38, %36
  %40 = trunc i32 %39 to i8
  store i8 %40, i8* %7, align 1
  %41 = load i64, i64* %5, align 8
  %42 = add nsw i64 %41, 1
  %43 = load i64, i64* %6, align 8
  %44 = call zeroext i1 @getCell(i64 noundef %42, i64 noundef %43)
  %45 = zext i1 %44 to i32
  %46 = load i8, i8* %7, align 1
  %47 = zext i8 %46 to i32
  %48 = add nsw i32 %47, %45
  %49 = trunc i32 %48 to i8
  store i8 %49, i8* %7, align 1
  %50 = load i64, i64* %5, align 8
  %51 = add nsw i64 %50, 1
  %52 = load i64, i64* %6, align 8
  %53 = add nsw i64 %52, 1
  %54 = call zeroext i1 @getCell(i64 noundef %51, i64 noundef %53)
  %55 = zext i1 %54 to i32
  %56 = load i8, i8* %7, align 1
  %57 = zext i8 %56 to i32
  %58 = add nsw i32 %57, %55
  %59 = trunc i32 %58 to i8
  store i8 %59, i8* %7, align 1
  %60 = load i64, i64* %5, align 8
  %61 = load i64, i64* %6, align 8
  %62 = add nsw i64 %61, 1
  %63 = call zeroext i1 @getCell(i64 noundef %60, i64 noundef %62)
  %64 = zext i1 %63 to i32
  %65 = load i8, i8* %7, align 1
  %66 = zext i8 %65 to i32
  %67 = add nsw i32 %66, %64
  %68 = trunc i32 %67 to i8
  store i8 %68, i8* %7, align 1
  %69 = load i64, i64* %5, align 8
  %70 = sub nsw i64 %69, 1
  %71 = load i64, i64* %6, align 8
  %72 = add nsw i64 %71, 1
  %73 = call zeroext i1 @getCell(i64 noundef %70, i64 noundef %72)
  %74 = zext i1 %73 to i32
  %75 = load i8, i8* %7, align 1
  %76 = zext i8 %75 to i32
  %77 = add nsw i32 %76, %74
  %78 = trunc i32 %77 to i8
  store i8 %78, i8* %7, align 1
  %79 = load i64, i64* %5, align 8
  %80 = sub nsw i64 %79, 1
  %81 = load i64, i64* %6, align 8
  %82 = call zeroext i1 @getCell(i64 noundef %80, i64 noundef %81)
  %83 = zext i1 %82 to i32
  %84 = load i8, i8* %7, align 1
  %85 = zext i8 %84 to i32
  %86 = add nsw i32 %85, %83
  %87 = trunc i32 %86 to i8
  store i8 %87, i8* %7, align 1
  %88 = load i8, i8* %7, align 1
  ret i8 %88
}

; Function Attrs: noinline nounwind optnone uwtable
define internal zeroext i1 @getCell(i64 noundef %0, i64 noundef %1) #0 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %0, i64* %3, align 8
  store i64 %1, i64* %4, align 8
  %5 = load i64, i64* %3, align 8
  %6 = add i64 %5, 400
  %7 = urem i64 %6, 400
  store i64 %7, i64* %3, align 8
  %8 = load i64, i64* %4, align 8
  %9 = add i64 %8, 400
  %10 = urem i64 %9, 400
  store i64 %10, i64* %4, align 8
  %11 = load i8*, i8** @pActiveField, align 8
  %12 = load i64, i64* %4, align 8
  %13 = mul i64 %12, 400
  %14 = load i64, i64* %3, align 8
  %15 = add i64 %13, %14
  %16 = getelementptr inbounds i8, i8* %11, i64 %15
  %17 = load i8, i8* %16, align 1
  %18 = trunc i8 %17 to i1
  ret i1 %18
}

declare void @putPixel(i32 noundef, i32 noundef, i8 noundef zeroext, i8 noundef zeroext, i8 noundef zeroext) #2

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { argmemonly nofree nounwind willreturn writeonly }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 14.0.0-1ubuntu1"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
!13 = distinct !{!13, !7}
