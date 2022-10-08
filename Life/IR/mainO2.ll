; ModuleID = 'main.c'
source_filename = "main.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@pActiveField = dso_local local_unnamed_addr global i8* null, align 8
@pNextField = dso_local local_unnamed_addr global i8* null, align 8

; Function Attrs: nounwind uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = alloca [160000 x i8], align 16
  %2 = alloca [160000 x i8], align 16
  %3 = getelementptr inbounds [160000 x i8], [160000 x i8]* %1, i64 0, i64 0
  call void @llvm.lifetime.start.p0i8(i64 160000, i8* nonnull %3) #4
  call void @llvm.memset.p0i8.i64(i8* noundef nonnull align 16 dereferenceable(160000) %3, i8 0, i64 160000, i1 false)
  %4 = getelementptr inbounds [160000 x i8], [160000 x i8]* %2, i64 0, i64 0
  call void @llvm.lifetime.start.p0i8(i64 160000, i8* nonnull %4) #4
  call void @llvm.memset.p0i8.i64(i8* noundef nonnull align 16 dereferenceable(160000) %4, i8 0, i64 160000, i1 false)
  store i8* %3, i8** @pActiveField, align 8, !tbaa !5
  store i8* %4, i8** @pNextField, align 8, !tbaa !5
  call void @createWindow(i32 noundef 400, i32 noundef 400) #4
  br label %5

5:                                                ; preds = %8, %0
  %6 = phi i64 [ 0, %0 ], [ %9, %8 ]
  %7 = mul nuw nsw i64 %6, 400
  br label %13

8:                                                ; preds = %13
  %9 = add nuw nsw i64 %6, 1
  %10 = icmp eq i64 %9, 400
  br i1 %10, label %11, label %5, !llvm.loop !9

11:                                               ; preds = %8
  %12 = call zeroext i1 @isWindowOpen() #4
  br i1 %12, label %22, label %118

13:                                               ; preds = %13, %5
  %14 = phi i64 [ 0, %5 ], [ %20, %13 ]
  %15 = call zeroext i1 @genRandomBool() #4
  %16 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  %17 = add nuw nsw i64 %14, %7
  %18 = getelementptr inbounds i8, i8* %16, i64 %17
  %19 = zext i1 %15 to i8
  store i8 %19, i8* %18, align 1, !tbaa !11
  %20 = add nuw nsw i64 %14, 1
  %21 = icmp eq i64 %20, 400
  br i1 %21, label %8, label %13, !llvm.loop !13

22:                                               ; preds = %11, %114
  call void @handleEvents() #4
  call void @clearWindow(i8 noundef zeroext 0, i8 noundef zeroext 0, i8 noundef zeroext 0) #4
  %23 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  %24 = load i8*, i8** @pNextField, align 8, !tbaa !5
  br label %25

25:                                               ; preds = %37, %22
  %26 = phi i64 [ 0, %22 ], [ %38, %37 ]
  %27 = add nuw nsw i64 %26, 399
  %28 = urem i64 %27, 400
  %29 = mul nuw nsw i64 %28, 400
  %30 = add nuw nsw i64 %26, 400
  %31 = urem i64 %30, 400
  %32 = mul nuw nsw i64 %31, 400
  %33 = add nuw nsw i64 %26, 401
  %34 = urem i64 %33, 400
  %35 = mul nuw nsw i64 %34, 400
  %36 = mul nuw nsw i64 %26, 400
  br label %40

37:                                               ; preds = %40
  %38 = add nuw nsw i64 %26, 1
  %39 = icmp eq i64 %38, 400
  br i1 %39, label %92, label %25, !llvm.loop !14

40:                                               ; preds = %40, %25
  %41 = phi i64 [ 0, %25 ], [ %90, %40 ]
  %42 = add nuw nsw i64 %41, 399
  %43 = urem i64 %42, 400
  %44 = add nuw nsw i64 %43, %29
  %45 = getelementptr inbounds i8, i8* %23, i64 %44
  %46 = load i8, i8* %45, align 1, !tbaa !11, !range !15
  %47 = add nuw nsw i64 %41, 400
  %48 = urem i64 %47, 400
  %49 = add nuw nsw i64 %48, %29
  %50 = getelementptr inbounds i8, i8* %23, i64 %49
  %51 = load i8, i8* %50, align 1, !tbaa !11, !range !15
  %52 = add nuw nsw i8 %51, %46
  %53 = add nuw nsw i64 %41, 401
  %54 = urem i64 %53, 400
  %55 = add nuw nsw i64 %54, %29
  %56 = getelementptr inbounds i8, i8* %23, i64 %55
  %57 = load i8, i8* %56, align 1, !tbaa !11, !range !15
  %58 = add nuw nsw i8 %52, %57
  %59 = add nuw nsw i64 %54, %32
  %60 = getelementptr inbounds i8, i8* %23, i64 %59
  %61 = load i8, i8* %60, align 1, !tbaa !11, !range !15
  %62 = add nuw nsw i8 %58, %61
  %63 = add nuw nsw i64 %54, %35
  %64 = getelementptr inbounds i8, i8* %23, i64 %63
  %65 = load i8, i8* %64, align 1, !tbaa !11, !range !15
  %66 = add nuw nsw i8 %62, %65
  %67 = add nuw nsw i64 %48, %35
  %68 = getelementptr inbounds i8, i8* %23, i64 %67
  %69 = load i8, i8* %68, align 1, !tbaa !11, !range !15
  %70 = add nuw nsw i8 %66, %69
  %71 = add nuw nsw i64 %43, %35
  %72 = getelementptr inbounds i8, i8* %23, i64 %71
  %73 = load i8, i8* %72, align 1, !tbaa !11, !range !15
  %74 = add nuw nsw i8 %70, %73
  %75 = add nuw nsw i64 %43, %32
  %76 = getelementptr inbounds i8, i8* %23, i64 %75
  %77 = load i8, i8* %76, align 1, !tbaa !11, !range !15
  %78 = add nuw nsw i8 %74, %77
  %79 = add nuw nsw i64 %48, %32
  %80 = getelementptr inbounds i8, i8* %23, i64 %79
  %81 = load i8, i8* %80, align 1, !tbaa !11, !range !15
  %82 = icmp eq i8 %81, 0
  %83 = and i8 %78, -2
  %84 = icmp eq i8 %83, 2
  %85 = icmp eq i8 %78, 3
  %86 = select i1 %82, i1 %85, i1 %84
  %87 = add nuw nsw i64 %41, %36
  %88 = getelementptr inbounds i8, i8* %24, i64 %87
  %89 = zext i1 %86 to i8
  store i8 %89, i8* %88, align 1, !tbaa !11
  %90 = add nuw nsw i64 %41, 1
  %91 = icmp eq i64 %90, 400
  br i1 %91, label %37, label %40, !llvm.loop !16

92:                                               ; preds = %37, %98
  %93 = phi i64 [ %99, %98 ], [ 0, %37 ]
  %94 = add nuw nsw i64 %93, 400
  %95 = urem i64 %94, 400
  %96 = mul nuw nsw i64 %95, 400
  %97 = trunc i64 %93 to i32
  br label %101

98:                                               ; preds = %101
  %99 = add nuw nsw i64 %93, 1
  %100 = icmp eq i64 %99, 400
  br i1 %100, label %114, label %92, !llvm.loop !17

101:                                              ; preds = %101, %92
  %102 = phi i32 [ 0, %92 ], [ %112, %101 ]
  %103 = trunc i32 %102 to i16
  %104 = add i16 %103, 400
  %105 = urem i16 %104, 400
  %106 = zext i16 %105 to i64
  %107 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  %108 = add nuw nsw i64 %96, %106
  %109 = getelementptr inbounds i8, i8* %107, i64 %108
  %110 = load i8, i8* %109, align 1, !tbaa !11, !range !15
  %111 = sub nsw i8 0, %110
  call void @putPixel(i32 noundef %102, i32 noundef %97, i8 noundef zeroext 0, i8 noundef zeroext %111, i8 noundef zeroext 0) #4
  %112 = add nuw nsw i32 %102, 1
  %113 = icmp eq i32 %112, 400
  br i1 %113, label %98, label %101, !llvm.loop !18

114:                                              ; preds = %98
  %115 = load i8*, i8** @pActiveField, align 8, !tbaa !5
  %116 = load i8*, i8** @pNextField, align 8, !tbaa !5
  store i8* %116, i8** @pActiveField, align 8, !tbaa !5
  store i8* %115, i8** @pNextField, align 8, !tbaa !5
  call void @flushWindow() #4
  %117 = call zeroext i1 @isWindowOpen() #4
  br i1 %117, label %22, label %118, !llvm.loop !19

118:                                              ; preds = %114, %11
  call void @llvm.lifetime.end.p0i8(i64 160000, i8* nonnull %4) #4
  call void @llvm.lifetime.end.p0i8(i64 160000, i8* nonnull %3) #4
  ret i32 0
}

; Function Attrs: argmemonly mustprogress nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: argmemonly mustprogress nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #2

declare void @createWindow(i32 noundef, i32 noundef) local_unnamed_addr #3

declare zeroext i1 @isWindowOpen() local_unnamed_addr #3

declare void @handleEvents() local_unnamed_addr #3

declare void @clearWindow(i8 noundef zeroext, i8 noundef zeroext, i8 noundef zeroext) local_unnamed_addr #3

declare void @flushWindow() local_unnamed_addr #3

; Function Attrs: argmemonly mustprogress nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

declare zeroext i1 @genRandomBool() local_unnamed_addr #3

declare void @putPixel(i32 noundef, i32 noundef, i8 noundef zeroext, i8 noundef zeroext, i8 noundef zeroext) local_unnamed_addr #3

attributes #0 = { nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { argmemonly mustprogress nofree nosync nounwind willreturn }
attributes #2 = { argmemonly mustprogress nofree nounwind willreturn writeonly }
attributes #3 = { "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind }

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
!9 = distinct !{!9, !10}
!10 = !{!"llvm.loop.mustprogress"}
!11 = !{!12, !12, i64 0}
!12 = !{!"_Bool", !7, i64 0}
!13 = distinct !{!13, !10}
!14 = distinct !{!14, !10}
!15 = !{i8 0, i8 2}
!16 = distinct !{!16, !10}
!17 = distinct !{!17, !10}
!18 = distinct !{!18, !10}
!19 = distinct !{!19, !10}
