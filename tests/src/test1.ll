; ModuleID = 'test1.c'
source_filename = "test1.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i8, align 1
  %b = alloca i8, align 1
  %c = alloca i8, align 1
  %d = alloca i8, align 1
  %e = alloca i8, align 1
  %f = alloca i8, align 1
  %g = alloca i8, align 1
  %h = alloca i8, align 1
  %sum = alloca i32, align 4
  %k = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i8 97, i8* %a, align 1
  store i8 98, i8* %b, align 1
  store i8 99, i8* %c, align 1
  store i8 100, i8* %d, align 1
  store i8 101, i8* %e, align 1
  store i8 102, i8* %f, align 1
  store i8 103, i8* %g, align 1
  store i8 104, i8* %h, align 1
  store i32 0, i32* %sum, align 4
  store i32 0, i32* %k, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, i32* %k, align 4
  %cmp = icmp slt i32 %0, 10
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %1 = load i32, i32* %k, align 4
  %cmp1 = icmp slt i32 %1, 5
  br i1 %cmp1, label %if.then, label %if.else

if.then:                                          ; preds = %for.body
  %2 = load i8, i8* %a, align 1
  %conv = sext i8 %2 to i32
  %3 = load i8, i8* %b, align 1
  %conv2 = sext i8 %3 to i32
  %add = add nsw i32 %conv, %conv2
  %4 = load i8, i8* %c, align 1
  %conv3 = sext i8 %4 to i32
  %add4 = add nsw i32 %add, %conv3
  %5 = load i8, i8* %d, align 1
  %conv5 = sext i8 %5 to i32
  %add6 = add nsw i32 %add4, %conv5
  %6 = load i32, i32* %sum, align 4
  %add7 = add nsw i32 %6, %add6
  store i32 %add7, i32* %sum, align 4
  br label %if.end

if.else:                                          ; preds = %for.body
  %7 = load i8, i8* %e, align 1
  %conv8 = sext i8 %7 to i32
  %8 = load i8, i8* %f, align 1
  %conv9 = sext i8 %8 to i32
  %add10 = add nsw i32 %conv8, %conv9
  %9 = load i8, i8* %g, align 1
  %conv11 = sext i8 %9 to i32
  %add12 = add nsw i32 %add10, %conv11
  %10 = load i8, i8* %h, align 1
  %conv13 = sext i8 %10 to i32
  %add14 = add nsw i32 %add12, %conv13
  %11 = load i32, i32* %sum, align 4
  %add15 = add nsw i32 %11, %add14
  store i32 %add15, i32* %sum, align 4
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %12 = load i32, i32* %sum, align 4
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %12)
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %13 = load i32, i32* %k, align 4
  %inc = add nsw i32 %13, 1
  store i32 %inc, i32* %k, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %14 = load i32, i32* %retval, align 4
  ret i32 %14
}

declare dso_local i32 @printf(i8*, ...) #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.1 (tags/RELEASE_801/final)"}
