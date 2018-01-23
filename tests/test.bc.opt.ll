; ModuleID = 'test.bc.opt'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct._cap_struct = type opaque
%struct.__va_list_tag = type { i32, i32, i8*, i8* }

@.str = private unnamed_addr constant [8 x i8] c"a = %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [17 x i8] c"hello from test\0A\00", align 1
@.str.2 = private unnamed_addr constant [32 x i8] c"there are at least 3 arguments\0A\00", align 1
@.str.3 = private unnamed_addr constant [36 x i8] c"there are no more than 2 arguments\0A\00", align 1
@.str.7 = private unnamed_addr constant [37 x i8] c"actually doing nothing but printing\0A\00", align 1
@.str.1.8 = private unnamed_addr constant [17 x i8] c"hello from f1()\0A\00", align 1
@.str.2.9 = private unnamed_addr constant [17 x i8] c"hello from f2()\0A\00", align 1
@.str.3.10 = private unnamed_addr constant [17 x i8] c"hello from f3()\0A\00", align 1
@.str.4 = private unnamed_addr constant [17 x i8] c"hello from f4()\0A\00", align 1
@.str.17 = private unnamed_addr constant [7 x i8] c"\0APERM\09\00", align 1
@.str.1.18 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.2.19 = private unnamed_addr constant [10 x i8] c"\0AINHERIT\09\00", align 1
@.str.3.20 = private unnamed_addr constant [9 x i8] c"\0AEFFECT\09\00", align 1
@.str.4.21 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

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
  br label %0

; <label>:0                                       ; preds = %entry
  %call = call i32 (i32, ...) @priv_raise(i32 1, i32 0)
  call void (...) bitcast (void ()* @f5 to void (...)*)()
  %call1 = call i32 (i32, ...) @priv_lower(i32 1, i32 0)
  br label %1

; <label>:1                                       ; preds = %0
  %2 = load i32, i32* %a.addr, align 4
  br label %3

; <label>:3                                       ; preds = %1
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str, i32 0, i32 0), i32 %2)
  br label %4

; <label>:4                                       ; preds = %3
  call void (...) bitcast (void ()* @print_cap to void (...)*)()
  ret void
}

declare i32 @printf(i8*, ...) #1

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
  br label %2

; <label>:2                                       ; preds = %entry
  call void @foo(i32 %1)
  br label %3

; <label>:3                                       ; preds = %2
  ret i8 99
}

; Function Attrs: nounwind uwtable
define void @bar(i32 %b) #0 {
entry:
  %b.addr = alloca i32, align 4
  store i32 %b, i32* %b.addr, align 4
  br label %0

; <label>:0                                       ; preds = %entry
  %call = call i32 (i32, ...) @priv_raise(i32 1, i32 0)
  %1 = load i32, i32* %b.addr, align 4
  br label %2

; <label>:2                                       ; preds = %0
  call void @foo(i32 %1)
  br label %3

; <label>:3                                       ; preds = %2
  %call1 = call i32 (i32, ...) @priv_lower(i32 1, i32 0)
  br label %4

; <label>:4                                       ; preds = %3
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
  br label %0

; <label>:0                                       ; preds = %entry
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.1, i32 0, i32 0))
  br label %1

; <label>:1                                       ; preds = %0
  br label %2

; <label>:2                                       ; preds = %1
  call void @foo(i32 10)
  br label %3

; <label>:3                                       ; preds = %2
  call void (...) bitcast (void ()* @f1 to void (...)*)()
  %4 = load i32, i32* %argc.addr, align 4
  %cmp = icmp sgt i32 %4, 2
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %3
  %call1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.2, i32 0, i32 0))
  br label %5

; <label>:5                                       ; preds = %if.then
  br label %if.end

if.else:                                          ; preds = %3
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.3, i32 0, i32 0))
  br label %6

; <label>:6                                       ; preds = %if.else
  br label %if.end

if.end:                                           ; preds = %6, %5
  call void (...) bitcast (void ()* @f2 to void (...)*)()
  br label %7

; <label>:7                                       ; preds = %if.end
  call void @indirect_foo(i32 10, i8 (i32)* @baz, i8 (i32)* @baz)
  br label %8

; <label>:8                                       ; preds = %7
  ret i32 0
}

; Function Attrs: nounwind uwtable
define void @uncalled_fun0() #0 {
entry:
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.7, i32 0, i32 0))
  br label %0

; <label>:0                                       ; preds = %entry
  ret void
}

; Function Attrs: nounwind uwtable
define void @uncalled_fun1() #0 {
entry:
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.7, i32 0, i32 0))
  br label %0

; <label>:0                                       ; preds = %entry
  ret void
}

; Function Attrs: nounwind uwtable
define void @f1() #0 {
entry:
  %call = call i32 (i32, ...) @priv_raise(i32 1, i32 0)
  br label %0

; <label>:0                                       ; preds = %entry
  call void @f2()
  br label %1

; <label>:1                                       ; preds = %0
  %call1 = call i32 (i32, ...) @priv_lower(i32 1, i32 0)
  br label %2

; <label>:2                                       ; preds = %1
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.1.8, i32 0, i32 0))
  br label %3

; <label>:3                                       ; preds = %2
  call void @f3()
  br label %4

; <label>:4                                       ; preds = %3
  ret void
}

; Function Attrs: nounwind uwtable
define void @f2() #0 {
entry:
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.2.9, i32 0, i32 0))
  br label %0

; <label>:0                                       ; preds = %entry
  call void @f5()
  br label %1

; <label>:1                                       ; preds = %0
  br label %2

; <label>:2                                       ; preds = %1
  call void @f5()
  br label %3

; <label>:3                                       ; preds = %2
  ret void
}

; Function Attrs: nounwind uwtable
define void @f3() #0 {
entry:
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.3.10, i32 0, i32 0))
  br label %0

; <label>:0                                       ; preds = %entry
  call void @f4()
  br label %1

; <label>:1                                       ; preds = %0
  ret void
}

; Function Attrs: nounwind uwtable
define void @f4() #0 {
entry:
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.4, i32 0, i32 0))
  br label %0

; <label>:0                                       ; preds = %entry
  call void @f1()
  br label %1

; <label>:1                                       ; preds = %0
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
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @priv_raise(i32 %count, ...) #0 {
entry:
  %retval = alloca i32, align 4
  %count.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %cap = alloca %struct._cap_struct*, align 8
  %cap_v = alloca i32, align 4
  %cap_v_list = alloca i32*, align 8
  %arglist = alloca [1 x %struct.__va_list_tag], align 16
  store i32 %count, i32* %count.addr, align 4
  %0 = load i32, i32* %count.addr, align 4
  %cmp = icmp sgt i32 %0, 37
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 -1, i32* %retval
  br label %return

if.end:                                           ; preds = %entry
  %1 = load i32, i32* %count.addr, align 4
  %conv = sext i32 %1 to i64
  %mul = mul i64 4, %conv
  %2 = alloca i8, i64 %mul
  %3 = bitcast i8* %2 to i32*
  store i32* %3, i32** %cap_v_list, align 8
  br label %4

; <label>:4                                       ; preds = %if.end
  %call = call %struct._cap_struct* @cap_get_proc()
  br label %5

; <label>:5                                       ; preds = %4
  store %struct._cap_struct* %call, %struct._cap_struct** %cap, align 8
  %cmp1 = icmp eq %struct._cap_struct* %call, null
  br i1 %cmp1, label %if.then.3, label %if.end.4

if.then.3:                                        ; preds = %5
  store i32 -1, i32* %retval
  br label %return

if.end.4:                                         ; preds = %5
  %arraydecay = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %arglist, i32 0, i32 0
  %arraydecay5 = bitcast %struct.__va_list_tag* %arraydecay to i8*
  br label %6

; <label>:6                                       ; preds = %if.end.4
  call void @llvm.va_start(i8* %arraydecay5)
  br label %7

; <label>:7                                       ; preds = %6
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %7
  %8 = load i32, i32* %i, align 4
  %9 = load i32, i32* %count.addr, align 4
  %cmp6 = icmp slt i32 %8, %9
  br i1 %cmp6, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %arraydecay8 = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %arglist, i32 0, i32 0
  %gp_offset_p = getelementptr inbounds %struct.__va_list_tag, %struct.__va_list_tag* %arraydecay8, i32 0, i32 0
  %gp_offset = load i32, i32* %gp_offset_p
  %fits_in_gp = icmp ule i32 %gp_offset, 40
  br i1 %fits_in_gp, label %vaarg.in_reg, label %vaarg.in_mem

vaarg.in_reg:                                     ; preds = %for.body
  %10 = getelementptr inbounds %struct.__va_list_tag, %struct.__va_list_tag* %arraydecay8, i32 0, i32 3
  %reg_save_area = load i8*, i8** %10
  %11 = getelementptr i8, i8* %reg_save_area, i32 %gp_offset
  %12 = bitcast i8* %11 to i32*
  %13 = add i32 %gp_offset, 8
  store i32 %13, i32* %gp_offset_p
  br label %vaarg.end

vaarg.in_mem:                                     ; preds = %for.body
  %overflow_arg_area_p = getelementptr inbounds %struct.__va_list_tag, %struct.__va_list_tag* %arraydecay8, i32 0, i32 2
  %overflow_arg_area = load i8*, i8** %overflow_arg_area_p
  %14 = bitcast i8* %overflow_arg_area to i32*
  %overflow_arg_area.next = getelementptr i8, i8* %overflow_arg_area, i32 8
  store i8* %overflow_arg_area.next, i8** %overflow_arg_area_p
  br label %vaarg.end

vaarg.end:                                        ; preds = %vaarg.in_mem, %vaarg.in_reg
  %vaarg.addr = phi i32* [ %12, %vaarg.in_reg ], [ %14, %vaarg.in_mem ]
  %15 = load i32, i32* %vaarg.addr
  store i32 %15, i32* %cap_v, align 4
  %16 = load i32, i32* %cap_v, align 4
  %17 = load i32, i32* %i, align 4
  %idxprom = sext i32 %17 to i64
  %18 = load i32*, i32** %cap_v_list, align 8
  %arrayidx = getelementptr inbounds i32, i32* %18, i64 %idxprom
  store i32 %16, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %vaarg.end
  %19 = load i32, i32* %i, align 4
  %inc = add nsw i32 %19, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %arraydecay9 = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %arglist, i32 0, i32 0
  %arraydecay910 = bitcast %struct.__va_list_tag* %arraydecay9 to i8*
  br label %20

; <label>:20                                      ; preds = %for.end
  call void @llvm.va_end(i8* %arraydecay910)
  br label %21

; <label>:21                                      ; preds = %20
  %22 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %23 = load i32, i32* %count.addr, align 4
  %24 = load i32*, i32** %cap_v_list, align 8
  br label %25

; <label>:25                                      ; preds = %21
  %call11 = call i32 @cap_set_flag(%struct._cap_struct* %22, i32 0, i32 %23, i32* %24, i32 1)
  br label %26

; <label>:26                                      ; preds = %25
  %cmp12 = icmp ne i32 %call11, 0
  br i1 %cmp12, label %if.then.14, label %if.end.16

if.then.14:                                       ; preds = %26
  %27 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %28 = bitcast %struct._cap_struct* %27 to i8*
  br label %29

; <label>:29                                      ; preds = %if.then.14
  %call15 = call i32 @cap_free(i8* %28)
  br label %30

; <label>:30                                      ; preds = %29
  store i32 -1, i32* %retval
  br label %return

if.end.16:                                        ; preds = %26
  %31 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  br label %32

; <label>:32                                      ; preds = %if.end.16
  %call17 = call i32 @cap_set_proc(%struct._cap_struct* %31)
  br label %33

; <label>:33                                      ; preds = %32
  %cmp18 = icmp slt i32 %call17, 0
  br i1 %cmp18, label %if.then.20, label %if.end.22

if.then.20:                                       ; preds = %33
  %34 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %35 = bitcast %struct._cap_struct* %34 to i8*
  br label %36

; <label>:36                                      ; preds = %if.then.20
  %call21 = call i32 @cap_free(i8* %35)
  br label %37

; <label>:37                                      ; preds = %36
  store i32 -1, i32* %retval
  br label %return

if.end.22:                                        ; preds = %33
  %38 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %39 = bitcast %struct._cap_struct* %38 to i8*
  br label %40

; <label>:40                                      ; preds = %if.end.22
  %call23 = call i32 @cap_free(i8* %39)
  br label %41

; <label>:41                                      ; preds = %40
  store i32 0, i32* %retval
  br label %return

return:                                           ; preds = %41, %37, %30, %if.then.3, %if.then
  %42 = load i32, i32* %retval
  ret i32 %42
}

declare %struct._cap_struct* @cap_get_proc() #1

; Function Attrs: nounwind
declare void @llvm.va_start(i8*) #2

; Function Attrs: nounwind
declare void @llvm.va_end(i8*) #2

declare i32 @cap_set_flag(%struct._cap_struct*, i32, i32, i32*, i32) #1

declare i32 @cap_free(i8*) #1

declare i32 @cap_set_proc(%struct._cap_struct*) #1

; Function Attrs: nounwind uwtable
define i32 @priv_lower(i32 %count, ...) #0 {
entry:
  %retval = alloca i32, align 4
  %count.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %cap = alloca %struct._cap_struct*, align 8
  %cap_v = alloca i32, align 4
  %cap_v_list = alloca i32*, align 8
  %arglist = alloca [1 x %struct.__va_list_tag], align 16
  store i32 %count, i32* %count.addr, align 4
  %0 = load i32, i32* %count.addr, align 4
  %cmp = icmp sgt i32 %0, 37
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 -1, i32* %retval
  br label %return

if.end:                                           ; preds = %entry
  %1 = load i32, i32* %count.addr, align 4
  %conv = sext i32 %1 to i64
  %mul = mul i64 4, %conv
  %2 = alloca i8, i64 %mul
  %3 = bitcast i8* %2 to i32*
  store i32* %3, i32** %cap_v_list, align 8
  br label %4

; <label>:4                                       ; preds = %if.end
  %call = call %struct._cap_struct* @cap_get_proc()
  br label %5

; <label>:5                                       ; preds = %4
  store %struct._cap_struct* %call, %struct._cap_struct** %cap, align 8
  %cmp1 = icmp eq %struct._cap_struct* %call, null
  br i1 %cmp1, label %if.then.3, label %if.end.4

if.then.3:                                        ; preds = %5
  store i32 -1, i32* %retval
  br label %return

if.end.4:                                         ; preds = %5
  %arraydecay = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %arglist, i32 0, i32 0
  %arraydecay5 = bitcast %struct.__va_list_tag* %arraydecay to i8*
  br label %6

; <label>:6                                       ; preds = %if.end.4
  call void @llvm.va_start(i8* %arraydecay5)
  br label %7

; <label>:7                                       ; preds = %6
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %7
  %8 = load i32, i32* %i, align 4
  %9 = load i32, i32* %count.addr, align 4
  %cmp6 = icmp slt i32 %8, %9
  br i1 %cmp6, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %arraydecay8 = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %arglist, i32 0, i32 0
  %gp_offset_p = getelementptr inbounds %struct.__va_list_tag, %struct.__va_list_tag* %arraydecay8, i32 0, i32 0
  %gp_offset = load i32, i32* %gp_offset_p
  %fits_in_gp = icmp ule i32 %gp_offset, 40
  br i1 %fits_in_gp, label %vaarg.in_reg, label %vaarg.in_mem

vaarg.in_reg:                                     ; preds = %for.body
  %10 = getelementptr inbounds %struct.__va_list_tag, %struct.__va_list_tag* %arraydecay8, i32 0, i32 3
  %reg_save_area = load i8*, i8** %10
  %11 = getelementptr i8, i8* %reg_save_area, i32 %gp_offset
  %12 = bitcast i8* %11 to i32*
  %13 = add i32 %gp_offset, 8
  store i32 %13, i32* %gp_offset_p
  br label %vaarg.end

vaarg.in_mem:                                     ; preds = %for.body
  %overflow_arg_area_p = getelementptr inbounds %struct.__va_list_tag, %struct.__va_list_tag* %arraydecay8, i32 0, i32 2
  %overflow_arg_area = load i8*, i8** %overflow_arg_area_p
  %14 = bitcast i8* %overflow_arg_area to i32*
  %overflow_arg_area.next = getelementptr i8, i8* %overflow_arg_area, i32 8
  store i8* %overflow_arg_area.next, i8** %overflow_arg_area_p
  br label %vaarg.end

vaarg.end:                                        ; preds = %vaarg.in_mem, %vaarg.in_reg
  %vaarg.addr = phi i32* [ %12, %vaarg.in_reg ], [ %14, %vaarg.in_mem ]
  %15 = load i32, i32* %vaarg.addr
  store i32 %15, i32* %cap_v, align 4
  %16 = load i32, i32* %cap_v, align 4
  %17 = load i32, i32* %i, align 4
  %idxprom = sext i32 %17 to i64
  %18 = load i32*, i32** %cap_v_list, align 8
  %arrayidx = getelementptr inbounds i32, i32* %18, i64 %idxprom
  store i32 %16, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %vaarg.end
  %19 = load i32, i32* %i, align 4
  %inc = add nsw i32 %19, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %arraydecay9 = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %arglist, i32 0, i32 0
  %arraydecay910 = bitcast %struct.__va_list_tag* %arraydecay9 to i8*
  br label %20

; <label>:20                                      ; preds = %for.end
  call void @llvm.va_end(i8* %arraydecay910)
  br label %21

; <label>:21                                      ; preds = %20
  %22 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %23 = load i32, i32* %count.addr, align 4
  %24 = load i32*, i32** %cap_v_list, align 8
  br label %25

; <label>:25                                      ; preds = %21
  %call11 = call i32 @cap_set_flag(%struct._cap_struct* %22, i32 0, i32 %23, i32* %24, i32 0)
  br label %26

; <label>:26                                      ; preds = %25
  %cmp12 = icmp ne i32 %call11, 0
  br i1 %cmp12, label %if.then.14, label %if.end.16

if.then.14:                                       ; preds = %26
  %27 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %28 = bitcast %struct._cap_struct* %27 to i8*
  br label %29

; <label>:29                                      ; preds = %if.then.14
  %call15 = call i32 @cap_free(i8* %28)
  br label %30

; <label>:30                                      ; preds = %29
  store i32 -1, i32* %retval
  br label %return

if.end.16:                                        ; preds = %26
  %31 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  br label %32

; <label>:32                                      ; preds = %if.end.16
  %call17 = call i32 @cap_set_proc(%struct._cap_struct* %31)
  br label %33

; <label>:33                                      ; preds = %32
  %cmp18 = icmp slt i32 %call17, 0
  br i1 %cmp18, label %if.then.20, label %if.end.22

if.then.20:                                       ; preds = %33
  %34 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %35 = bitcast %struct._cap_struct* %34 to i8*
  br label %36

; <label>:36                                      ; preds = %if.then.20
  %call21 = call i32 @cap_free(i8* %35)
  br label %37

; <label>:37                                      ; preds = %36
  store i32 -1, i32* %retval
  br label %return

if.end.22:                                        ; preds = %33
  %38 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %39 = bitcast %struct._cap_struct* %38 to i8*
  br label %40

; <label>:40                                      ; preds = %if.end.22
  %call23 = call i32 @cap_free(i8* %39)
  br label %41

; <label>:41                                      ; preds = %40
  store i32 0, i32* %retval
  br label %return

return:                                           ; preds = %41, %37, %30, %if.then.3, %if.then
  %42 = load i32, i32* %retval
  ret i32 %42
}

; Function Attrs: nounwind uwtable
define i32 @priv_remove(i32 %count, ...) #0 {
entry:
  %retval = alloca i32, align 4
  %count.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %cap = alloca %struct._cap_struct*, align 8
  %cap_v = alloca i32, align 4
  %cap_v_list = alloca i32*, align 8
  %arglist = alloca [1 x %struct.__va_list_tag], align 16
  store i32 %count, i32* %count.addr, align 4
  %0 = load i32, i32* %count.addr, align 4
  %cmp = icmp sgt i32 %0, 37
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 -1, i32* %retval
  br label %return

if.end:                                           ; preds = %entry
  %1 = load i32, i32* %count.addr, align 4
  %conv = sext i32 %1 to i64
  %mul = mul i64 4, %conv
  %2 = alloca i8, i64 %mul
  %3 = bitcast i8* %2 to i32*
  store i32* %3, i32** %cap_v_list, align 8
  br label %4

; <label>:4                                       ; preds = %if.end
  %call = call %struct._cap_struct* @cap_get_proc()
  br label %5

; <label>:5                                       ; preds = %4
  store %struct._cap_struct* %call, %struct._cap_struct** %cap, align 8
  %cmp1 = icmp eq %struct._cap_struct* %call, null
  br i1 %cmp1, label %if.then.3, label %if.end.4

if.then.3:                                        ; preds = %5
  store i32 -1, i32* %retval
  br label %return

if.end.4:                                         ; preds = %5
  %arraydecay = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %arglist, i32 0, i32 0
  %arraydecay5 = bitcast %struct.__va_list_tag* %arraydecay to i8*
  br label %6

; <label>:6                                       ; preds = %if.end.4
  call void @llvm.va_start(i8* %arraydecay5)
  br label %7

; <label>:7                                       ; preds = %6
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %7
  %8 = load i32, i32* %i, align 4
  %9 = load i32, i32* %count.addr, align 4
  %cmp6 = icmp slt i32 %8, %9
  br i1 %cmp6, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %arraydecay8 = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %arglist, i32 0, i32 0
  %gp_offset_p = getelementptr inbounds %struct.__va_list_tag, %struct.__va_list_tag* %arraydecay8, i32 0, i32 0
  %gp_offset = load i32, i32* %gp_offset_p
  %fits_in_gp = icmp ule i32 %gp_offset, 40
  br i1 %fits_in_gp, label %vaarg.in_reg, label %vaarg.in_mem

vaarg.in_reg:                                     ; preds = %for.body
  %10 = getelementptr inbounds %struct.__va_list_tag, %struct.__va_list_tag* %arraydecay8, i32 0, i32 3
  %reg_save_area = load i8*, i8** %10
  %11 = getelementptr i8, i8* %reg_save_area, i32 %gp_offset
  %12 = bitcast i8* %11 to i32*
  %13 = add i32 %gp_offset, 8
  store i32 %13, i32* %gp_offset_p
  br label %vaarg.end

vaarg.in_mem:                                     ; preds = %for.body
  %overflow_arg_area_p = getelementptr inbounds %struct.__va_list_tag, %struct.__va_list_tag* %arraydecay8, i32 0, i32 2
  %overflow_arg_area = load i8*, i8** %overflow_arg_area_p
  %14 = bitcast i8* %overflow_arg_area to i32*
  %overflow_arg_area.next = getelementptr i8, i8* %overflow_arg_area, i32 8
  store i8* %overflow_arg_area.next, i8** %overflow_arg_area_p
  br label %vaarg.end

vaarg.end:                                        ; preds = %vaarg.in_mem, %vaarg.in_reg
  %vaarg.addr = phi i32* [ %12, %vaarg.in_reg ], [ %14, %vaarg.in_mem ]
  %15 = load i32, i32* %vaarg.addr
  store i32 %15, i32* %cap_v, align 4
  %16 = load i32, i32* %cap_v, align 4
  %17 = load i32, i32* %i, align 4
  %idxprom = sext i32 %17 to i64
  %18 = load i32*, i32** %cap_v_list, align 8
  %arrayidx = getelementptr inbounds i32, i32* %18, i64 %idxprom
  store i32 %16, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %vaarg.end
  %19 = load i32, i32* %i, align 4
  %inc = add nsw i32 %19, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %arraydecay9 = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %arglist, i32 0, i32 0
  %arraydecay910 = bitcast %struct.__va_list_tag* %arraydecay9 to i8*
  br label %20

; <label>:20                                      ; preds = %for.end
  call void @llvm.va_end(i8* %arraydecay910)
  br label %21

; <label>:21                                      ; preds = %20
  %22 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %23 = load i32, i32* %count.addr, align 4
  %24 = load i32*, i32** %cap_v_list, align 8
  br label %25

; <label>:25                                      ; preds = %21
  %call11 = call i32 @cap_set_flag(%struct._cap_struct* %22, i32 0, i32 %23, i32* %24, i32 0)
  br label %26

; <label>:26                                      ; preds = %25
  %cmp12 = icmp ne i32 %call11, 0
  br i1 %cmp12, label %if.then.14, label %if.end.16

if.then.14:                                       ; preds = %26
  %27 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %28 = bitcast %struct._cap_struct* %27 to i8*
  br label %29

; <label>:29                                      ; preds = %if.then.14
  %call15 = call i32 @cap_free(i8* %28)
  br label %30

; <label>:30                                      ; preds = %29
  store i32 -1, i32* %retval
  br label %return

if.end.16:                                        ; preds = %26
  %31 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %32 = load i32, i32* %count.addr, align 4
  %33 = load i32*, i32** %cap_v_list, align 8
  br label %34

; <label>:34                                      ; preds = %if.end.16
  %call17 = call i32 @cap_set_flag(%struct._cap_struct* %31, i32 1, i32 %32, i32* %33, i32 0)
  br label %35

; <label>:35                                      ; preds = %34
  %cmp18 = icmp ne i32 %call17, 0
  br i1 %cmp18, label %if.then.20, label %if.end.22

if.then.20:                                       ; preds = %35
  %36 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %37 = bitcast %struct._cap_struct* %36 to i8*
  br label %38

; <label>:38                                      ; preds = %if.then.20
  %call21 = call i32 @cap_free(i8* %37)
  br label %39

; <label>:39                                      ; preds = %38
  store i32 -1, i32* %retval
  br label %return

if.end.22:                                        ; preds = %35
  %40 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  br label %41

; <label>:41                                      ; preds = %if.end.22
  %call23 = call i32 @cap_set_proc(%struct._cap_struct* %40)
  br label %42

; <label>:42                                      ; preds = %41
  %cmp24 = icmp slt i32 %call23, 0
  br i1 %cmp24, label %if.then.26, label %if.end.28

if.then.26:                                       ; preds = %42
  %43 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %44 = bitcast %struct._cap_struct* %43 to i8*
  br label %45

; <label>:45                                      ; preds = %if.then.26
  %call27 = call i32 @cap_free(i8* %44)
  br label %46

; <label>:46                                      ; preds = %45
  store i32 -1, i32* %retval
  br label %return

if.end.28:                                        ; preds = %42
  %47 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %48 = bitcast %struct._cap_struct* %47 to i8*
  br label %49

; <label>:49                                      ; preds = %if.end.28
  %call29 = call i32 @cap_free(i8* %48)
  br label %50

; <label>:50                                      ; preds = %49
  store i32 0, i32* %retval
  br label %return

return:                                           ; preds = %50, %46, %39, %30, %if.then.3, %if.then
  %51 = load i32, i32* %retval
  ret i32 %51
}

; Function Attrs: nounwind uwtable
define i32 @priv_lowerall() #0 {
entry:
  %retval = alloca i32, align 4
  %i = alloca i32, align 4
  %cap = alloca %struct._cap_struct*, align 8
  %cap_v = alloca i32, align 4
  %cap_v_list = alloca i32*, align 8
  %arglist = alloca [1 x %struct.__va_list_tag], align 16
  br label %0

; <label>:0                                       ; preds = %entry
  %call = call %struct._cap_struct* @cap_get_proc()
  br label %1

; <label>:1                                       ; preds = %0
  store %struct._cap_struct* %call, %struct._cap_struct** %cap, align 8
  %cmp = icmp eq %struct._cap_struct* %call, null
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %1
  store i32 -1, i32* %retval
  br label %return

if.end:                                           ; preds = %1
  %2 = alloca i8, i64 152
  %3 = bitcast i8* %2 to i32*
  store i32* %3, i32** %cap_v_list, align 8
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end
  %4 = load i32, i32* %i, align 4
  %cmp1 = icmp slt i32 %4, 38
  br i1 %cmp1, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %5 = load i32, i32* %i, align 4
  %6 = load i32, i32* %i, align 4
  %idxprom = sext i32 %6 to i64
  %7 = load i32*, i32** %cap_v_list, align 8
  %arrayidx = getelementptr inbounds i32, i32* %7, i64 %idxprom
  store i32 %5, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %8 = load i32, i32* %i, align 4
  %inc = add nsw i32 %8, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %9 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %10 = load i32*, i32** %cap_v_list, align 8
  br label %11

; <label>:11                                      ; preds = %for.end
  %call2 = call i32 @cap_set_flag(%struct._cap_struct* %9, i32 0, i32 38, i32* %10, i32 0)
  br label %12

; <label>:12                                      ; preds = %11
  %cmp3 = icmp ne i32 %call2, 0
  br i1 %cmp3, label %if.then.4, label %if.end.6

if.then.4:                                        ; preds = %12
  %13 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %14 = bitcast %struct._cap_struct* %13 to i8*
  br label %15

; <label>:15                                      ; preds = %if.then.4
  %call5 = call i32 @cap_free(i8* %14)
  br label %16

; <label>:16                                      ; preds = %15
  store i32 -1, i32* %retval
  br label %return

if.end.6:                                         ; preds = %12
  %17 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  br label %18

; <label>:18                                      ; preds = %if.end.6
  %call7 = call i32 @cap_set_proc(%struct._cap_struct* %17)
  br label %19

; <label>:19                                      ; preds = %18
  %cmp8 = icmp slt i32 %call7, 0
  br i1 %cmp8, label %if.then.9, label %if.end.11

if.then.9:                                        ; preds = %19
  %20 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %21 = bitcast %struct._cap_struct* %20 to i8*
  br label %22

; <label>:22                                      ; preds = %if.then.9
  %call10 = call i32 @cap_free(i8* %21)
  br label %23

; <label>:23                                      ; preds = %22
  store i32 -1, i32* %retval
  br label %return

if.end.11:                                        ; preds = %19
  %24 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %25 = bitcast %struct._cap_struct* %24 to i8*
  br label %26

; <label>:26                                      ; preds = %if.end.11
  %call12 = call i32 @cap_free(i8* %25)
  br label %27

; <label>:27                                      ; preds = %26
  store i32 0, i32* %retval
  br label %return

return:                                           ; preds = %27, %23, %16, %if.then
  %28 = load i32, i32* %retval
  ret i32 %28
}

; Function Attrs: nounwind uwtable
define void @print_cap() #0 {
entry:
  %i = alloca i32, align 4
  %cap = alloca %struct._cap_struct*, align 8
  %value = alloca i32, align 4
  br label %0

; <label>:0                                       ; preds = %entry
  %call = call %struct._cap_struct* @cap_get_proc()
  br label %1

; <label>:1                                       ; preds = %0
  store %struct._cap_struct* %call, %struct._cap_struct** %cap, align 8
  br label %2

; <label>:2                                       ; preds = %1
  %call1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.17, i32 0, i32 0))
  br label %3

; <label>:3                                       ; preds = %2
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %3
  %4 = load i32, i32* %i, align 4
  %cmp = icmp sle i32 %4, 37
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %5 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %6 = load i32, i32* %i, align 4
  br label %7

; <label>:7                                       ; preds = %for.body
  %call2 = call i32 @cap_get_flag(%struct._cap_struct* %5, i32 %6, i32 1, i32* %value)
  br label %8

; <label>:8                                       ; preds = %7
  %9 = load i32, i32* %value, align 4
  %cmp3 = icmp eq i32 %9, 1
  %conv = zext i1 %cmp3 to i32
  br label %10

; <label>:10                                      ; preds = %8
  %call4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1.18, i32 0, i32 0), i32 %conv)
  br label %11

; <label>:11                                      ; preds = %10
  br label %for.inc

for.inc:                                          ; preds = %11
  %12 = load i32, i32* %i, align 4
  %inc = add nsw i32 %12, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %call5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2.19, i32 0, i32 0))
  br label %13

; <label>:13                                      ; preds = %for.end
  store i32 0, i32* %i, align 4
  br label %for.cond.6

for.cond.6:                                       ; preds = %for.inc.14, %13
  %14 = load i32, i32* %i, align 4
  %cmp7 = icmp sle i32 %14, 37
  br i1 %cmp7, label %for.body.9, label %for.end.16

for.body.9:                                       ; preds = %for.cond.6
  %15 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %16 = load i32, i32* %i, align 4
  br label %17

; <label>:17                                      ; preds = %for.body.9
  %call10 = call i32 @cap_get_flag(%struct._cap_struct* %15, i32 %16, i32 2, i32* %value)
  br label %18

; <label>:18                                      ; preds = %17
  %19 = load i32, i32* %value, align 4
  %cmp11 = icmp eq i32 %19, 1
  %conv12 = zext i1 %cmp11 to i32
  br label %20

; <label>:20                                      ; preds = %18
  %call13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1.18, i32 0, i32 0), i32 %conv12)
  br label %21

; <label>:21                                      ; preds = %20
  br label %for.inc.14

for.inc.14:                                       ; preds = %21
  %22 = load i32, i32* %i, align 4
  %inc15 = add nsw i32 %22, 1
  store i32 %inc15, i32* %i, align 4
  br label %for.cond.6

for.end.16:                                       ; preds = %for.cond.6
  %call17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.3.20, i32 0, i32 0))
  br label %23

; <label>:23                                      ; preds = %for.end.16
  store i32 0, i32* %i, align 4
  br label %for.cond.18

for.cond.18:                                      ; preds = %for.inc.26, %23
  %24 = load i32, i32* %i, align 4
  %cmp19 = icmp sle i32 %24, 37
  br i1 %cmp19, label %for.body.21, label %for.end.28

for.body.21:                                      ; preds = %for.cond.18
  %25 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %26 = load i32, i32* %i, align 4
  br label %27

; <label>:27                                      ; preds = %for.body.21
  %call22 = call i32 @cap_get_flag(%struct._cap_struct* %25, i32 %26, i32 0, i32* %value)
  br label %28

; <label>:28                                      ; preds = %27
  %29 = load i32, i32* %value, align 4
  %cmp23 = icmp eq i32 %29, 1
  %conv24 = zext i1 %cmp23 to i32
  br label %30

; <label>:30                                      ; preds = %28
  %call25 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1.18, i32 0, i32 0), i32 %conv24)
  br label %31

; <label>:31                                      ; preds = %30
  br label %for.inc.26

for.inc.26:                                       ; preds = %31
  %32 = load i32, i32* %i, align 4
  %inc27 = add nsw i32 %32, 1
  store i32 %inc27, i32* %i, align 4
  br label %for.cond.18

for.end.28:                                       ; preds = %for.cond.18
  %call29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.4.21, i32 0, i32 0))
  br label %33

; <label>:33                                      ; preds = %for.end.28
  %34 = load %struct._cap_struct*, %struct._cap_struct** %cap, align 8
  %35 = bitcast %struct._cap_struct* %34 to i8*
  br label %36

; <label>:36                                      ; preds = %33
  %call30 = call i32 @cap_free(i8* %35)
  br label %37

; <label>:37                                      ; preds = %36
  ret void
}

declare i32 @cap_get_flag(%struct._cap_struct*, i32, i32, i32*) #1

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0, !0, !0}

!0 = !{!"clang version 3.7.0 (tags/RELEASE_370/final)"}
