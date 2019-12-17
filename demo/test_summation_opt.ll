; ModuleID = 'test_summation_opt.bc'
source_filename = "test_summation.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %cursed1 = alloca i64
  %i = alloca i64, align 8
  %cursed = alloca i64
  %0 = load i64, i64* %cursed1
  %1 = load i64, i64* %cursed
  store i32 0, i32* %retval, align 4
  store i64 0, i64* %i, align 8
  %2 = and i64 %1, 4294967295
  %3 = zext i32 0 to i64
  %4 = shl i64 %3, 32
  %5 = or i64 %4, %2
  store i64 %5, i64* %cursed
  %6 = and i64 %0, 0
  %7 = zext i16 3 to i64
  %8 = shl i64 %7, 48
  %9 = or i64 %8, %6
  %10 = zext i16 4 to i64
  %11 = shl i64 %10, 0
  %12 = or i64 %11, %9
  %13 = zext i16 2 to i64
  %14 = shl i64 %13, 32
  %15 = or i64 %14, %12
  %16 = zext i16 1 to i64
  %17 = shl i64 %16, 16
  %18 = or i64 %17, %15
  store i64 %18, i64* %cursed1
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %19 = load i64, i64* %i, align 8
  %cmp = icmp slt i64 %19, 200000000
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %20 = load i64, i64* %cursed
  %21 = ashr i64 %20, 32
  %22 = and i64 %21, 4294967295
  %23 = trunc i64 %22 to i32
  %24 = load i64, i64* %cursed1
  %25 = ashr i64 %24, 32
  %26 = and i64 %25, 65535
  %27 = trunc i64 %26 to i16
  %28 = ashr i64 %24, 16
  %29 = and i64 %28, 65535
  %30 = trunc i64 %29 to i16
  %31 = ashr i64 %24, 48
  %32 = and i64 %31, 65535
  %33 = trunc i64 %32 to i16
  %34 = and i64 %24, 65535
  %35 = trunc i64 %34 to i16
  %inc = add i16 %30, 1
  %conv = sext i16 %30 to i32
  %add = add nsw i32 0, %conv
  %inc1 = add i16 %27, 1
  %conv2 = sext i16 %27 to i32
  %add3 = add nsw i32 %add, %conv2
  %inc4 = add i16 %33, 1
  %conv5 = sext i16 %33 to i32
  %add6 = add nsw i32 %add3, %conv5
  %inc7 = add i16 %35, 1
  %conv8 = sext i16 %35 to i32
  %add9 = add nsw i32 %add6, %conv8
  %add10 = add nsw i32 %23, %add9
  %inc11 = add i16 %inc, 1
  %conv12 = sext i16 %inc to i32
  %mul = mul nsw i32 %add10, %conv12
  %inc13 = add i16 %inc1, 1
  %conv14 = sext i16 %inc1 to i32
  %mul15 = mul nsw i32 %mul, %conv14
  %inc16 = add i16 %inc4, 1
  %conv17 = sext i16 %inc4 to i32
  %mul18 = mul nsw i32 %mul15, %conv17
  %inc19 = add i16 %inc7, 1
  %conv20 = sext i16 %inc7 to i32
  %mul21 = mul nsw i32 %mul18, %conv20
  %36 = and i64 %24, 0
  %37 = zext i16 %inc16 to i64
  %38 = shl i64 %37, 48
  %39 = or i64 %38, %36
  %40 = zext i16 %inc19 to i64
  %41 = shl i64 %40, 0
  %42 = or i64 %41, %39
  %43 = zext i16 %inc13 to i64
  %44 = shl i64 %43, 32
  %45 = or i64 %44, %42
  %46 = zext i16 %inc11 to i64
  %47 = shl i64 %46, 16
  %48 = or i64 %47, %45
  store i64 %48, i64* %cursed1
  %49 = and i64 %20, 0
  %50 = zext i32 %mul21 to i64
  %51 = shl i64 %50, 32
  %52 = or i64 %51, %49
  %53 = zext i32 %add9 to i64
  %54 = shl i64 %53, 0
  %55 = or i64 %54, %52
  store i64 %55, i64* %cursed
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %56 = load i64, i64* %i, align 8
  %inc22 = add nsw i64 %56, 1
  store i64 %inc22, i64* %i, align 8
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %57 = load i64, i64* %cursed
  %58 = ashr i64 %57, 32
  %59 = and i64 %58, 4294967295
  %60 = trunc i64 %59 to i32
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %60)
  %61 = load i32, i32* %retval, align 4
  ret i32 %61
}

declare dso_local i32 @printf(i8*, ...) #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.1 (tags/RELEASE_801/final)"}
