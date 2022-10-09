; ModuleID = 'main.c'
source_filename = "main.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@pActiveField = dso_local local_unnamed_addr global i8* null, align 8
@pNextField = dso_local local_unnamed_addr global i8* null, align 8

; Function Attrs: mustprogress nofree norecurse nosync nounwind readonly uwtable willreturn
define dso_local zeroext i1 @getCell(i64 noundef %0, i64 noundef %1) local_unnamed_addr #0 {
  %3 = add i64 %0, 400
  %4 = urem i64 %3, 400
  %5 = add i64 %1, 400
  %6 = urem i64 %5, 400
  %7 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  %8 = mul nuw nsw i64 %6, 400
  %9 = add nuw nsw i64 %8, %4
  %10 = getelementptr inbounds i8, i8* %7, i64 %9
  %11 = load i8, i8* %10, align 1, !tbaa !9, !range !11
  %12 = icmp ne i8 %11, 0
  ret i1 %12
}

; Function Attrs: nounwind uwtable
define dso_local void @fillField() local_unnamed_addr #1 {
  br label %1

1:                                                ; preds = %0, %5
  %2 = phi i64 [ 0, %0 ], [ %6, %5 ]
  %3 = mul nuw nsw i64 %2, 400
  br label %8

4:                                                ; preds = %5
  ret void

5:                                                ; preds = %8
  %6 = add nuw nsw i64 %2, 1
  %7 = icmp eq i64 %6, 400
  br i1 %7, label %4, label %1, !llvm.loop !12

8:                                                ; preds = %1, %8
  %9 = phi i64 [ 0, %1 ], [ %15, %8 ]
  %10 = tail call zeroext i1 @genRandomBool() #7
  %11 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  %12 = add nuw nsw i64 %3, %9
  %13 = getelementptr inbounds i8, i8* %11, i64 %12
  %14 = zext i1 %10 to i8
  store i8 %14, i8* %13, align 1, !tbaa !9
  %15 = add nuw nsw i64 %9, 1
  %16 = icmp eq i64 %15, 400
  br i1 %16, label %5, label %8, !llvm.loop !14
}

; Function Attrs: argmemonly mustprogress nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #2

declare zeroext i1 @genRandomBool() local_unnamed_addr #3

; Function Attrs: argmemonly mustprogress nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #2

; Function Attrs: mustprogress nofree norecurse nosync nounwind readonly uwtable willreturn
define dso_local zeroext i8 @getNeighbours(i32 noundef %0, i32 noundef %1) local_unnamed_addr #0 {
  %3 = zext i32 %0 to i64
  %4 = zext i32 %1 to i64
  %5 = add nuw nsw i64 %3, 399
  %6 = urem i64 %5, 400
  %7 = add nuw nsw i64 %4, 399
  %8 = urem i64 %7, 400
  %9 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  %10 = mul nuw nsw i64 %8, 400
  %11 = add nuw nsw i64 %10, %6
  %12 = getelementptr inbounds i8, i8* %9, i64 %11
  %13 = load i8, i8* %12, align 1, !tbaa !9, !range !11
  %14 = add nuw nsw i64 %3, 400
  %15 = urem i64 %14, 400
  %16 = add nuw nsw i64 %10, %15
  %17 = getelementptr inbounds i8, i8* %9, i64 %16
  %18 = load i8, i8* %17, align 1, !tbaa !9, !range !11
  %19 = add nuw nsw i8 %18, %13
  %20 = add nuw nsw i64 %3, 401
  %21 = urem i64 %20, 400
  %22 = add nuw nsw i64 %10, %21
  %23 = getelementptr inbounds i8, i8* %9, i64 %22
  %24 = load i8, i8* %23, align 1, !tbaa !9, !range !11
  %25 = add nuw nsw i8 %19, %24
  %26 = add nuw nsw i64 %4, 400
  %27 = urem i64 %26, 400
  %28 = mul nuw nsw i64 %27, 400
  %29 = add nuw nsw i64 %28, %21
  %30 = getelementptr inbounds i8, i8* %9, i64 %29
  %31 = load i8, i8* %30, align 1, !tbaa !9, !range !11
  %32 = add nuw nsw i8 %25, %31
  %33 = add nuw nsw i64 %4, 401
  %34 = urem i64 %33, 400
  %35 = mul nuw nsw i64 %34, 400
  %36 = add nuw nsw i64 %35, %21
  %37 = getelementptr inbounds i8, i8* %9, i64 %36
  %38 = load i8, i8* %37, align 1, !tbaa !9, !range !11
  %39 = add nuw nsw i8 %32, %38
  %40 = add nuw nsw i64 %35, %15
  %41 = getelementptr inbounds i8, i8* %9, i64 %40
  %42 = load i8, i8* %41, align 1, !tbaa !9, !range !11
  %43 = add nuw nsw i8 %39, %42
  %44 = add nuw nsw i64 %35, %6
  %45 = getelementptr inbounds i8, i8* %9, i64 %44
  %46 = load i8, i8* %45, align 1, !tbaa !9, !range !11
  %47 = add nuw nsw i8 %43, %46
  %48 = add nuw nsw i64 %28, %6
  %49 = getelementptr inbounds i8, i8* %9, i64 %48
  %50 = load i8, i8* %49, align 1, !tbaa !9, !range !11
  %51 = add nuw nsw i8 %47, %50
  ret i8 %51
}

; Function Attrs: nofree norecurse nosync nounwind uwtable
define dso_local void @newGeneration() local_unnamed_addr #4 {
  %1 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  %2 = load i8*, i8** @pNextField, align 8, !tbaa !5
  br label %3

3:                                                ; preds = %0, %16
  %4 = phi i64 [ 0, %0 ], [ %17, %16 ]
  %5 = add nuw nsw i64 %4, 399
  %6 = urem i64 %5, 400
  %7 = mul nuw nsw i64 %6, 400
  %8 = add nuw nsw i64 %4, 400
  %9 = urem i64 %8, 400
  %10 = mul nuw nsw i64 %9, 400
  %11 = add nuw nsw i64 %4, 401
  %12 = urem i64 %11, 400
  %13 = mul nuw nsw i64 %12, 400
  %14 = mul nuw nsw i64 %4, 400
  br label %19

15:                                               ; preds = %16
  ret void

16:                                               ; preds = %19
  %17 = add nuw nsw i64 %4, 1
  %18 = icmp eq i64 %17, 400
  br i1 %18, label %15, label %3, !llvm.loop !15

19:                                               ; preds = %3, %19
  %20 = phi i64 [ 0, %3 ], [ %69, %19 ]
  %21 = add nuw nsw i64 %20, 399
  %22 = urem i64 %21, 400
  %23 = add nuw nsw i64 %22, %7
  %24 = getelementptr inbounds i8, i8* %1, i64 %23
  %25 = load i8, i8* %24, align 1, !tbaa !9, !range !11
  %26 = add nuw nsw i64 %20, 400
  %27 = urem i64 %26, 400
  %28 = add nuw nsw i64 %27, %7
  %29 = getelementptr inbounds i8, i8* %1, i64 %28
  %30 = load i8, i8* %29, align 1, !tbaa !9, !range !11
  %31 = add nuw nsw i8 %30, %25
  %32 = add nuw nsw i64 %20, 401
  %33 = urem i64 %32, 400
  %34 = add nuw nsw i64 %33, %7
  %35 = getelementptr inbounds i8, i8* %1, i64 %34
  %36 = load i8, i8* %35, align 1, !tbaa !9, !range !11
  %37 = add nuw nsw i8 %31, %36
  %38 = add nuw nsw i64 %33, %10
  %39 = getelementptr inbounds i8, i8* %1, i64 %38
  %40 = load i8, i8* %39, align 1, !tbaa !9, !range !11
  %41 = add nuw nsw i8 %37, %40
  %42 = add nuw nsw i64 %33, %13
  %43 = getelementptr inbounds i8, i8* %1, i64 %42
  %44 = load i8, i8* %43, align 1, !tbaa !9, !range !11
  %45 = add nuw nsw i8 %41, %44
  %46 = add nuw nsw i64 %27, %13
  %47 = getelementptr inbounds i8, i8* %1, i64 %46
  %48 = load i8, i8* %47, align 1, !tbaa !9, !range !11
  %49 = add nuw nsw i8 %45, %48
  %50 = add nuw nsw i64 %22, %13
  %51 = getelementptr inbounds i8, i8* %1, i64 %50
  %52 = load i8, i8* %51, align 1, !tbaa !9, !range !11
  %53 = add nuw nsw i8 %49, %52
  %54 = add nuw nsw i64 %22, %10
  %55 = getelementptr inbounds i8, i8* %1, i64 %54
  %56 = load i8, i8* %55, align 1, !tbaa !9, !range !11
  %57 = add nuw nsw i8 %53, %56
  %58 = add nuw nsw i64 %27, %10
  %59 = getelementptr inbounds i8, i8* %1, i64 %58
  %60 = load i8, i8* %59, align 1, !tbaa !9, !range !11
  %61 = icmp eq i8 %60, 0
  %62 = and i8 %57, -2
  %63 = icmp eq i8 %62, 2
  %64 = icmp eq i8 %57, 3
  %65 = select i1 %61, i1 %64, i1 %63
  %66 = add nuw nsw i64 %20, %14
  %67 = getelementptr inbounds i8, i8* %2, i64 %66
  %68 = zext i1 %65 to i8
  store i8 %68, i8* %67, align 1, !tbaa !9
  %69 = add nuw nsw i64 %20, 1
  %70 = icmp eq i64 %69, 400
  br i1 %70, label %16, label %19, !llvm.loop !16
}

; Function Attrs: nounwind uwtable
define dso_local void @drawField() local_unnamed_addr #1 {
  br label %1

1:                                                ; preds = %0, %9
  %2 = phi i64 [ 0, %0 ], [ %10, %9 ]
  %3 = add nuw nsw i64 %2, 400
  %4 = urem i64 %3, 400
  %5 = mul nuw nsw i64 %4, 400
  %6 = trunc i64 %2 to i32
  %7 = trunc i64 %2 to i32
  br label %12

8:                                                ; preds = %9
  ret void

9:                                                ; preds = %25
  %10 = add nuw nsw i64 %2, 1
  %11 = icmp eq i64 %10, 400
  br i1 %11, label %8, label %1, !llvm.loop !17

12:                                               ; preds = %1, %25
  %13 = phi i32 [ 0, %1 ], [ %26, %25 ]
  %14 = trunc i32 %13 to i16
  %15 = add i16 %14, 400
  %16 = urem i16 %15, 400
  %17 = zext i16 %16 to i64
  %18 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  %19 = add nuw nsw i64 %5, %17
  %20 = getelementptr inbounds i8, i8* %18, i64 %19
  %21 = load i8, i8* %20, align 1, !tbaa !9, !range !11
  %22 = icmp eq i8 %21, 0
  br i1 %22, label %24, label %23

23:                                               ; preds = %12
  tail call void @putPixel(i32 noundef %13, i32 noundef %6, i8 noundef zeroext 0, i8 noundef zeroext -1, i8 noundef zeroext 0) #7
  br label %25

24:                                               ; preds = %12
  tail call void @putPixel(i32 noundef %13, i32 noundef %7, i8 noundef zeroext 0, i8 noundef zeroext 0, i8 noundef zeroext 0) #7
  br label %25

25:                                               ; preds = %23, %24
  %26 = add nuw nsw i32 %13, 1
  %27 = icmp eq i32 %26, 400
  br i1 %27, label %9, label %12, !llvm.loop !18
}

declare void @putPixel(i32 noundef, i32 noundef, i8 noundef zeroext, i8 noundef zeroext, i8 noundef zeroext) local_unnamed_addr #3

; Function Attrs: mustprogress nofree norecurse nosync nounwind uwtable willreturn
define dso_local void @swapActiveField() local_unnamed_addr #5 {
  %1 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  %2 = load i8*, i8** @pNextField, align 8, !tbaa !5
  store i8* %2, i8** @pActiveField, align 8, !tbaa !5
  store i8* %1, i8** @pNextField, align 8, !tbaa !5
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
  %1 = alloca [160000 x i8], align 16
  %2 = alloca [160000 x i8], align 16
  %3 = getelementptr inbounds [160000 x i8], [160000 x i8]* %1, i64 0, i64 0
  call void @llvm.lifetime.start.p0i8(i64 160000, i8* nonnull %3) #7
  call void @llvm.memset.p0i8.i64(i8* noundef nonnull align 16 dereferenceable(160000) %3, i8 0, i64 160000, i1 false)
  %4 = getelementptr inbounds [160000 x i8], [160000 x i8]* %2, i64 0, i64 0
  call void @llvm.lifetime.start.p0i8(i64 160000, i8* nonnull %4) #7
  call void @llvm.memset.p0i8.i64(i8* noundef nonnull align 16 dereferenceable(160000) %4, i8 0, i64 160000, i1 false)
  store i8* %3, i8** @pActiveField, align 8, !tbaa !5
  store i8* %4, i8** @pNextField, align 8, !tbaa !5
  call void @createWindow(i32 noundef 400, i32 noundef 400) #7
  br label %5

5:                                                ; preds = %8, %0
  %6 = phi i64 [ 0, %0 ], [ %9, %8 ]
  %7 = mul nuw nsw i64 %6, 400
  br label %13

8:                                                ; preds = %13
  %9 = add nuw nsw i64 %6, 1
  %10 = icmp eq i64 %9, 400
  br i1 %10, label %11, label %5, !llvm.loop !12

11:                                               ; preds = %8
  %12 = call zeroext i1 @isWindowOpen() #7
  br i1 %12, label %22, label %49

13:                                               ; preds = %13, %5
  %14 = phi i64 [ 0, %5 ], [ %20, %13 ]
  %15 = call zeroext i1 @genRandomBool() #7
  %16 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  %17 = add nuw nsw i64 %14, %7
  %18 = getelementptr inbounds i8, i8* %16, i64 %17
  %19 = zext i1 %15 to i8
  store i8 %19, i8* %18, align 1, !tbaa !9
  %20 = add nuw nsw i64 %14, 1
  %21 = icmp eq i64 %20, 400
  br i1 %21, label %8, label %13, !llvm.loop !14

22:                                               ; preds = %11, %45
  call void @handleEvents() #7
  call void @clearWindow(i8 noundef zeroext 0, i8 noundef zeroext 0, i8 noundef zeroext 0) #7
  call void @newGeneration()
  br label %23

23:                                               ; preds = %29, %22
  %24 = phi i64 [ 0, %22 ], [ %30, %29 ]
  %25 = add nuw nsw i64 %24, 400
  %26 = urem i64 %25, 400
  %27 = mul nuw nsw i64 %26, 400
  %28 = trunc i64 %24 to i32
  br label %32

29:                                               ; preds = %32
  %30 = add nuw nsw i64 %24, 1
  %31 = icmp eq i64 %30, 400
  br i1 %31, label %45, label %23, !llvm.loop !17

32:                                               ; preds = %32, %23
  %33 = phi i32 [ 0, %23 ], [ %43, %32 ]
  %34 = trunc i32 %33 to i16
  %35 = add i16 %34, 400
  %36 = urem i16 %35, 400
  %37 = zext i16 %36 to i64
  %38 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  %39 = add nuw nsw i64 %27, %37
  %40 = getelementptr inbounds i8, i8* %38, i64 %39
  %41 = load i8, i8* %40, align 1, !tbaa !9, !range !11
  %42 = sub nsw i8 0, %41
  call void @putPixel(i32 noundef %33, i32 noundef %28, i8 noundef zeroext 0, i8 noundef zeroext %42, i8 noundef zeroext 0) #7
  %43 = add nuw nsw i32 %33, 1
  %44 = icmp eq i32 %43, 400
  br i1 %44, label %29, label %32, !llvm.loop !18

45:                                               ; preds = %29
  %46 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  %47 = load i8*, i8** @pNextField, align 8, !tbaa !5
  store i8* %47, i8** @pActiveField, align 8, !tbaa !5
  store i8* %46, i8** @pNextField, align 8, !tbaa !5
  call void @flushWindow() #7
  %48 = call zeroext i1 @isWindowOpen() #7
  br i1 %48, label %22, label %49, !llvm.loop !19

49:                                               ; preds = %45, %11
  call void @llvm.lifetime.end.p0i8(i64 160000, i8* nonnull %4) #7
  call void @llvm.lifetime.end.p0i8(i64 160000, i8* nonnull %3) #7
  ret i32 0
}

; Function Attrs: argmemonly mustprogress nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #6

declare void @createWindow(i32 noundef, i32 noundef) local_unnamed_addr #3

declare zeroext i1 @isWindowOpen() local_unnamed_addr #3

declare void @handleEvents() local_unnamed_addr #3

declare void @clearWindow(i8 noundef zeroext, i8 noundef zeroext, i8 noundef zeroext) local_unnamed_addr #3

declare void @flushWindow() local_unnamed_addr #3

attributes #0 = { mustprogress nofree norecurse nosync nounwind readonly uwtable willreturn "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { argmemonly mustprogress nofree nosync nounwind willreturn }
attributes #3 = { "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nofree norecurse nosync nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { mustprogress nofree norecurse nosync nounwind uwtable willreturn "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { argmemonly mustprogress nofree nounwind willreturn writeonly }
attributes #7 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{!"Ubuntu clang version 14.0.0-1ubuntu1"}
!5 = !{!6, !6, i64 0}
!6 = !{!"any pointer", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = !{!10, !10, i64 0}
!10 = !{!"_Bool", !7, i64 0}
!11 = !{i8 0, i8 2}
!12 = distinct !{!12, !13}
!13 = !{!"llvm.loop.mustprogress"}
!14 = distinct !{!14, !13}
!15 = distinct !{!15, !13}
!16 = distinct !{!16, !13}
!17 = distinct !{!17, !13}
!18 = distinct !{!18, !13}
!19 = distinct !{!19, !13}
