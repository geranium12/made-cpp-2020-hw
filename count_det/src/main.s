.text

.global count_det

count_det:
ldr r1, [r0, #16]
ldr r2, [r0, #32]
mul r1, r2
ldr r4, [r0]
mul r3, r1, r4

ldr r1, [r0, #20]
ldr r2, [r0, #28]
mul r1, r2
mul r1, r4
sub r3, r1

ldr r1, [r0, #20]
ldr r2, [r0, #24]
mul r1, r2
ldr r4, [r0, #4]
mul r1, r4
add r3, r1

ldr r1, [r0, #12]
ldr r2, [r0, #32]
mul r1, r2
mul r1, r4
sub r3, r1

ldr r1, [r0, #12]
ldr r2, [r0, #28]
mul r1, r2
ldr r4, [r0, #8]
mul r1, r4
add r3, r1

ldr r1, [r0, #16]
ldr r2, [r0, #24]
mul r1, r2
mul r1, r4
sub r3, r1
mov r0, r3

bx lr

