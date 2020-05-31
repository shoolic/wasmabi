; ModuleID = 'sourceStream'
source_filename = "sourceStream"

@tekst = private constant [6 x i8] c"tekst\00"
@"%s\0A" = private constant [4 x i8] c"%s\0A\00"
@"%d\0A" = private constant [4 x i8] c"%d\0A\00"

define i32 @calc(i32 %0, i32 %1) {
entry:
  %base = alloca i32
  store i32 %0, i32* %base
  %exponent = alloca i32
  store i32 %1, i32* %exponent
  br label %selectcase

selectcase:                                       ; preds = %entry
  %exponent1 = load i32, i32* %exponent
  %base2 = load i32, i32* %base
  %2 = trunc i32 %base2 to i1
  %3 = xor i1 %2, true
  %4 = zext i1 %3 to i32
  %5 = trunc i32 %4 to i1
  %6 = and i1 false, %5
  %7 = zext i1 %6 to i32
  %8 = icmp ugt i32 %exponent1, %7
  %9 = zext i1 %8 to i32
  %10 = icmp ult i32 %9, 0
  %11 = zext i1 %10 to i32
  %12 = trunc i32 %11 to i1
  br i1 %12, label %selectcaseblock, label %selectotherwiseblock

selectcaseblock:                                  ; preds = %selectcase
  %base3 = load i32, i32* %base
  %exponent4 = load i32, i32* %exponent
  %13 = sitofp i32 %base3 to float
  %14 = sitofp i32 %exponent4 to float
  %15 = call float @powf(float %13, float %14)
  %16 = fptosi float %15 to i32
  %17 = add i32 %16, 1
  br label %selectcont

selectotherwiseblock:                             ; preds = %selectcase
  br label %selectcont

selectcont:                                       ; preds = %selectotherwiseblock, %selectcaseblock
  %selecttmp = phi i32 [ %17, %selectcaseblock ], [ 0, %selectotherwiseblock ]
  ret i32 %selecttmp
  ret i32 0
}

define void @printer() {
entry:
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @"%s\0A", i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @tekst, i32 0, i32 0))
  ret void
}

define i32 @main() {
entry:
  %0 = call i32 @calc(i32 2, i32 3)
  %x = alloca i32
  store i32 %0, i32* %x
  %y = alloca i32
  store i32 2, i32* %y
  %x1 = load i32, i32* %x
  %y2 = load i32, i32* %y
  %1 = call i32 @calc(i32 %x1, i32 %y2)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @"%d\0A", i32 0, i32 0), i32 %1)
  call void @printer()
  ret i32 0
  ret i32 0
}

declare float @powf(float, float)

declare i32 @printf(i8*, ...)
