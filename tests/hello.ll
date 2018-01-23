; ModuleID = 'hello.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [8 x i8] c"a = %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [17 x i8] c"hello from test\0A\00", align 1
@.str.2 = private unnamed_addr constant [32 x i8] c"there are at least 3 arguments\0A\00", align 1
@.str.3 = private unnamed_addr constant [36 x i8] c"there are no more than 2 arguments\0A\00", align 1

; Function Attrs: nounwind uwtable
define void @indirect_foo(i32 %a, i8 (i32)* %fn, i8 (i32)* %f2) #0 {
entry:
  %a.addr = alloca i32, align 4
  %fn.addr = alloca i8 (i32)*, align 8
  %f2.addr = alloca i8 (i32)*, align 8
  store i32 %a, i32* %a.addr, align 4
  store i8 (i32)* %fn, i8 (i32)** %fn.addr, align 8
  store i8 (i32)* %f2, i8 (i32)** %f2.addr, align 8
  %0 = load i8 (i32)*, i8 (i32)** %fn.addr, align 8
  %call = call signext i8 %0(i32 10)
  %1 = load i8 (i32)*, i8 (i32)** %f2.addr, align 8
  %call1 = call signext i8 %1(i32 20)
  ret void
}

; Function Attrs: nounwind uwtable
define void @foo(i32 %a) #0 {
entry:
  %a.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  %call = call i32 (i32, ...) @priv_raise(i32 1, i32 0)
  call void (...) @f5()
  %call1 = call i32 (i32, ...) @priv_lower(i32 1, i32 0)
  %0 = load i32, i32* %a.addr, align 4
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str, i32 0, i32 0), i32 %0)
  call void (...) @print_cap()
  ret void
}

declare i32 @priv_raise(i32, ...) #1

declare void @f5(...) #1

declare i32 @priv_lower(i32, ...) #1

declare i32 @printf(i8*, ...) #1

declare void @print_cap(...) #1

; Function Attrs: nounwind uwtable
define signext i8 @baz(i32 %a) #0 {
entry:
  %a.addr = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  %0 = load i32, i32* %a.addr, align 4
  %add = add nsw i32 %0, 1
  store i32 %add, i32* %b, align 4
  %1 = load i32, i32* %b, align 4
  call void @foo(i32 %1)
  ret i8 99
}

; Function Attrs: nounwind uwtable
define void @bar(i32 %b) #0 {
entry:
  %b.addr = alloca i32, align 4
  store i32 %b, i32* %b.addr, align 4
  %call = call i32 (i32, ...) @priv_raise(i32 1, i32 0)
  %0 = load i32, i32* %b.addr, align 4
  call void @foo(i32 %0)
  %call1 = call i32 (i32, ...) @priv_lower(i32 1, i32 0)
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  store i32 0, i32* %retval
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.1, i32 0, i32 0))
  call void @foo(i32 10)
  call void (...) @f1()
  %0 = load i32, i32* %argc.addr, align 4
  %cmp = icmp sgt i32 %0, 2
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %call1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.2, i32 0, i32 0))
  br label %if.end

if.else:                                          ; preds = %entry
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.3, i32 0, i32 0))
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  call void (...) @f2()
  call void @indirect_foo(i32 10, i8 (i32)* @baz, i8 (i32)* @baz)
  ret i32 0
}

declare void @f1(...) #1

declare void @f2(...) #1

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (tags/RELEASE_370/final)"}
