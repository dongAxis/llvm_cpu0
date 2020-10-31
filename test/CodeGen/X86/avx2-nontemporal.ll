; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=i686-unknown-linux-gnu -mattr=+avx2 | FileCheck %s --check-prefix=X32
; RUN: llc < %s -mtriple=x86_64-unknown-linux-gnu -mattr=+avx2 | FileCheck %s --check-prefix=X64

define void @f(<8 x float> %A, i8* %B, <4 x double> %C, <4 x i64> %E, <8 x i32> %F, <16 x i16> %G, <32 x i8> %H) nounwind {
; X32-LABEL: f:
; X32:       # BB#0:
; X32-NEXT:    pushl %ebp
; X32-NEXT:    movl %esp, %ebp
; X32-NEXT:    andl $-32, %esp
; X32-NEXT:    subl $32, %esp
; X32-NEXT:    vmovdqa 104(%ebp), %ymm3
; X32-NEXT:    vmovdqa 72(%ebp), %ymm4
; X32-NEXT:    vmovdqa 40(%ebp), %ymm5
; X32-NEXT:    movl 8(%ebp), %eax
; X32-NEXT:    vaddps .LCPI0_0, %ymm0, %ymm0
; X32-NEXT:    vmovntps %ymm0, (%eax)
; X32-NEXT:    vpaddq .LCPI0_1, %ymm2, %ymm0
; X32-NEXT:    vmovntdq %ymm0, (%eax)
; X32-NEXT:    vaddpd .LCPI0_2, %ymm1, %ymm0
; X32-NEXT:    vmovntpd %ymm0, (%eax)
; X32-NEXT:    vpaddd .LCPI0_3, %ymm5, %ymm0
; X32-NEXT:    vmovntdq %ymm0, (%eax)
; X32-NEXT:    vpaddw .LCPI0_4, %ymm4, %ymm0
; X32-NEXT:    vmovntdq %ymm0, (%eax)
; X32-NEXT:    vpaddb .LCPI0_5, %ymm3, %ymm0
; X32-NEXT:    vmovntdq %ymm0, (%eax)
; X32-NEXT:    movl %ebp, %esp
; X32-NEXT:    popl %ebp
; X32-NEXT:    vzeroupper
; X32-NEXT:    retl
;
; X64-LABEL: f:
; X64:       # BB#0:
; X64-NEXT:    vaddps {{.*}}(%rip), %ymm0, %ymm0
; X64-NEXT:    vmovntps %ymm0, (%rdi)
; X64-NEXT:    vpaddq {{.*}}(%rip), %ymm2, %ymm0
; X64-NEXT:    vmovntdq %ymm0, (%rdi)
; X64-NEXT:    vaddpd {{.*}}(%rip), %ymm1, %ymm0
; X64-NEXT:    vmovntpd %ymm0, (%rdi)
; X64-NEXT:    vpaddd {{.*}}(%rip), %ymm3, %ymm0
; X64-NEXT:    vmovntdq %ymm0, (%rdi)
; X64-NEXT:    vpaddw {{.*}}(%rip), %ymm4, %ymm0
; X64-NEXT:    vmovntdq %ymm0, (%rdi)
; X64-NEXT:    vpaddb {{.*}}(%rip), %ymm5, %ymm0
; X64-NEXT:    vmovntdq %ymm0, (%rdi)
; X64-NEXT:    vzeroupper
; X64-NEXT:    retq
  %cast = bitcast i8* %B to <8 x float>*
  %A2 = fadd <8 x float> %A, <float 1.0, float 2.0, float 3.0, float 4.0, float 5.0, float 6.0, float 7.0, float 8.0>
  store <8 x float> %A2, <8 x float>* %cast, align 32, !nontemporal !0
  %cast1 = bitcast i8* %B to <4 x i64>*
  %E2 = add <4 x i64> %E, <i64 1, i64 2, i64 3, i64 4>
  store <4 x i64> %E2, <4 x i64>* %cast1, align 32, !nontemporal !0
  %cast2 = bitcast i8* %B to <4 x double>*
  %C2 = fadd <4 x double> %C, <double 1.0, double 2.0, double 3.0, double 4.0>
  store <4 x double> %C2, <4 x double>* %cast2, align 32, !nontemporal !0
  %cast3 = bitcast i8* %B to <8 x i32>*
  %F2 = add <8 x i32> %F, <i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8>
  store <8 x i32> %F2, <8 x i32>* %cast3, align 32, !nontemporal !0
  %cast4 = bitcast i8* %B to <16 x i16>*
  %G2 = add <16 x i16> %G, <i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7, i16 8, i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7, i16 8>
  store <16 x i16> %G2, <16 x i16>* %cast4, align 32, !nontemporal !0
  %cast5 = bitcast i8* %B to <32 x i8>*
  %H2 = add <32 x i8> %H, <i8 1, i8 2, i8 3, i8 4, i8 5, i8 6, i8 7, i8 8, i8 1, i8 2, i8 3, i8 4, i8 5, i8 6, i8 7, i8 8, i8 1, i8 2, i8 3, i8 4, i8 5, i8 6, i8 7, i8 8, i8 1, i8 2, i8 3, i8 4, i8 5, i8 6, i8 7, i8 8>
  store <32 x i8> %H2, <32 x i8>* %cast5, align 32, !nontemporal !0
  ret void
}

!0 = !{i32 1}
