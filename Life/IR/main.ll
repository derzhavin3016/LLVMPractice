; ModuleID = 'main.cc'
source_filename = "main.cc"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"struct.life::Field" = type { i32, i32, i32* }

$_ZN4life5FieldC2Ev = comdat any

; Function Attrs: mustprogress noinline norecurse optnone uwtable
define dso_local noundef i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca %"struct.life::Field", align 8
  %5 = alloca %"struct.life::Field", align 8
  store i32 0, i32* %1, align 4
  store i32 180, i32* %2, align 4
  store i32 180, i32* %3, align 4
  %6 = load i32, i32* %3, align 4
  %7 = load i32, i32* %2, align 4
  call void @_ZN5graph12createWindowEjj(i32 noundef %6, i32 noundef %7)
  call void @_ZN4life5FieldC2Ev(%"struct.life::Field"* noundef nonnull align 8 dereferenceable(16) %4) #3
  call void @_ZN4life5FieldC2Ev(%"struct.life::Field"* noundef nonnull align 8 dereferenceable(16) %5) #3
  %8 = load i32, i32* %3, align 4
  %9 = load i32, i32* %2, align 4
  %10 = call noundef zeroext i1 @_ZN4life9initFieldEPNS_5FieldEjj(%"struct.life::Field"* noundef %4, i32 noundef %8, i32 noundef %9)
  br i1 %10, label %12, label %11

11:                                               ; preds = %0
  store i32 1, i32* %1, align 4
  br label %22

12:                                               ; preds = %0
  %13 = load i32, i32* %3, align 4
  %14 = load i32, i32* %2, align 4
  %15 = call noundef zeroext i1 @_ZN4life9initFieldEPNS_5FieldEjj(%"struct.life::Field"* noundef %5, i32 noundef %13, i32 noundef %14)
  br i1 %15, label %17, label %16

16:                                               ; preds = %12
  call void @_ZN4life12destroyFieldEPNS_5FieldE(%"struct.life::Field"* noundef %4)
  store i32 1, i32* %1, align 4
  br label %22

17:                                               ; preds = %12
  call void @_ZN4life9fillFieldEPNS_5FieldE(%"struct.life::Field"* noundef %4)
  br label %18

18:                                               ; preds = %20, %17
  %19 = call noundef zeroext i1 @_ZN5graph12isWindowOpenEv()
  br i1 %19, label %20, label %21

20:                                               ; preds = %18
  call void @_ZN5graph12handleEventsEv()
  call void @_ZN5graph11clearWindowEhhh(i8 noundef zeroext 0, i8 noundef zeroext 0, i8 noundef zeroext 0)
  call void @_ZN4life13newGenerationEPNS_5FieldES1_(%"struct.life::Field"* noundef %4, %"struct.life::Field"* noundef %5)
  call void @_ZN4life9drawFieldEPNS_5FieldE(%"struct.life::Field"* noundef %5)
  call void @_ZN4life10swapFieldsEPNS_5FieldES1_(%"struct.life::Field"* noundef %4, %"struct.life::Field"* noundef %5)
  call void @_ZN5graph11flushWindowEv()
  br label %18, !llvm.loop !6

21:                                               ; preds = %18
  call void @_ZN4life12destroyFieldEPNS_5FieldE(%"struct.life::Field"* noundef %4)
  call void @_ZN4life12destroyFieldEPNS_5FieldE(%"struct.life::Field"* noundef %5)
  br label %22

22:                                               ; preds = %21, %16, %11
  %23 = load i32, i32* %1, align 4
  ret i32 %23
}

declare void @_ZN5graph12createWindowEjj(i32 noundef, i32 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN4life5FieldC2Ev(%"struct.life::Field"* noundef nonnull align 8 dereferenceable(16) %0) unnamed_addr #2 comdat align 2 {
  %2 = alloca %"struct.life::Field"*, align 8
  store %"struct.life::Field"* %0, %"struct.life::Field"** %2, align 8
  %3 = load %"struct.life::Field"*, %"struct.life::Field"** %2, align 8
  %4 = getelementptr inbounds %"struct.life::Field", %"struct.life::Field"* %3, i32 0, i32 2
  store i32* null, i32** %4, align 8
  ret void
}

declare noundef zeroext i1 @_ZN4life9initFieldEPNS_5FieldEjj(%"struct.life::Field"* noundef, i32 noundef, i32 noundef) #1

declare void @_ZN4life12destroyFieldEPNS_5FieldE(%"struct.life::Field"* noundef) #1

declare void @_ZN4life9fillFieldEPNS_5FieldE(%"struct.life::Field"* noundef) #1

declare noundef zeroext i1 @_ZN5graph12isWindowOpenEv() #1

declare void @_ZN5graph12handleEventsEv() #1

declare void @_ZN5graph11clearWindowEhhh(i8 noundef zeroext, i8 noundef zeroext, i8 noundef zeroext) #1

declare void @_ZN4life13newGenerationEPNS_5FieldES1_(%"struct.life::Field"* noundef, %"struct.life::Field"* noundef) #1

declare void @_ZN4life9drawFieldEPNS_5FieldE(%"struct.life::Field"* noundef) #1

declare void @_ZN4life10swapFieldsEPNS_5FieldES1_(%"struct.life::Field"* noundef, %"struct.life::Field"* noundef) #1

declare void @_ZN5graph11flushWindowEv() #1

attributes #0 = { mustprogress noinline norecurse optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind }

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
