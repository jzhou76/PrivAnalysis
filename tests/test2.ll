; ModuleID = 'test2.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@g = global i32 10, align 4
@.str = private unnamed_addr constant [6 x i8] c"g > 0\00", align 1
@.str.1 = private unnamed_addr constant [7 x i8] c"g <= 0\00", align 1
@.str.3 = private unnamed_addr constant [37 x i8] c"actually doing nothing but printing\0A\00", align 1
@.str.1.4 = private unnamed_addr constant [17 x i8] c"hello from f1()\0A\00", align 1
@.str.2 = private unnamed_addr constant [17 x i8] c"hello from f2()\0A\00", align 1
@.str.3.5 = private unnamed_addr constant [17 x i8] c"hello from f3()\0A\00", align 1
@.str.4 = private unnamed_addr constant [17 x i8] c"hello from f4()\0A\00", align 1

; Function Attrs: nounwind uwtable
define i32 @same_source_func(i32 %a, i32 %b) #0 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  store i32 %b, i32* %b.addr, align 4
  %0 = load i32, i32* %a.addr, align 4
  %1 = load i32, i32* %b.addr, align 4
  %mul = mul nsw i32 %0, %1
  %2 = load i32, i32* %a.addr, align 4
  %add = add nsw i32 %mul, %2
  ret i32 %add
}

; Function Attrs: nounwind uwtable
define i32 @bar(i32 %a) #0 {
entry:
  %a.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  %0 = load i32, i32* %a.addr, align 4
  ret i32 %0
}

; Function Attrs: nounwind uwtable
define void @foo(i32 (i32)* %f) #0 {
entry:
  %f.addr = alloca i32 (i32)*, align 8
  store i32 (i32)* %f, i32 (i32)** %f.addr, align 8
  %0 = load i32, i32* @g, align 4
  %cmp = icmp sgt i32 %0, 0
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str, i32 0, i32 0))
  br label %if.end

if.else:                                          ; preds = %entry
  %call1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0))
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  ret void
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  store i32 0, i32* %retval
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  call void (...) bitcast (void ()* @f5 to void (...)*)()
  %call = call i32 @same_source_func(i32 1, i32 2)
  call void @foo(i32 (i32)* @bar)
  ret i32 0
}

; Function Attrs: nounwind uwtable
define void @uncalled_fun0() #0 {
entry:
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.3, i32 0, i32 0))
  ret void
}

; Function Attrs: nounwind uwtable
define void @uncalled_fun1() #0 {
entry:
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.3, i32 0, i32 0))
  ret void
}

; Function Attrs: nounwind uwtable
define void @f1() #0 {
entry:
  %call = call i32 (i32, ...) @priv_raise(i32 1, i32 0)
  call void @f2()
  %call1 = call i32 (i32, ...) @priv_lower(i32 1, i32 0)
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.1.4, i32 0, i32 0))
  call void @f3()
  ret void
}

declare i32 @priv_raise(i32, ...) #1

declare i32 @priv_lower(i32, ...) #1

; Function Attrs: nounwind uwtable
define void @f2() #0 {
entry:
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.2, i32 0, i32 0))
  call void @f5()
  call void @f5()
  ret void
}

; Function Attrs: nounwind uwtable
define void @f3() #0 {
entry:
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.3.5, i32 0, i32 0))
  call void @f4()
  ret void
}

; Function Attrs: nounwind uwtable
define void @f4() #0 {
entry:
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.4, i32 0, i32 0))
  call void @f1()
  ret void
}

; Function Attrs: nounwind uwtable
define void @f5() #0 {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 10
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %1 = load i32, i32* %i, align 4
  %inc = add nsw i32 %1, 1
  store i32 %inc, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %call = call i32 (i32, ...) @priv_raise(i32 1, i32 0)
  %call1 = call i32 (i32, ...) @priv_lower(i32 1, i32 0)
  ret void
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0, !0}

!0 = !{!"clang version 3.7.0 (tags/RELEASE_370/final)"}
