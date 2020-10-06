/// Your code goes here...

.text
.global count_det //global def of func

count_det:
  // r0 for reading
  // r1-r6 for results of 6 productions
  // r1 = + a[0] * a[4] * a[8]
  // r2 = - a[0] * a[5] * a[7]
  // r3 = - a[1] * a[3] * a[8]
  // r4 = + a[1] * a[5] * a[6]
  // r5 = + a[2] * a[3] * a[7]
  // r6 = - a[2] * a[4] * a[6]

  push {r4-r7}

  // load a[0], a[1], a[2] to r1, r
  ldr r1, [r0], #4  // read a[0]
  mov r2, r1 // copy r1 to r2
  ldr r3, [r0], #4  // read a[1]
  mov r4, r3
  ldr r5, [r0], #4  // read a[2]
  mov r6, r5

  // use r7 as for reading

  ldr r7, [r0], #4 // read a[3]
  mul r3, r3, r7 // a[1] * a[3]
  mul r5, r5, r7 // a[2] * a[3]

  ldr r7, [r0], #4 // read a[4]
  mul r1, r1, r7 // a[0] * a[4]
  mul r6, r6, r7 // a[2] * a[4]

  ldr r7, [r0], #4 // read a[5]
  mul r2, r2, r7 // a[0] * a[5]
  mul r4, r4, r7 // a[2] * a[4]

  ldr r7, [r0], #4 // read a[6]
  mul r4, r4, r7 // a[1] * a[5] * a[6]
  mul r6, r6, r7 // a[2] * a[4] * a[6]

  ldr r7, [r0], #4 // read a[7]
  mul r2, r2, r7 // a[0] * a[5] * a[7]
  mul r5, r5, r7 // a[2] * a[3] * a[7]

  ldr r7, [r0], #4 // read a[8]
  mul r1, r1, r7 // a[0] * a[4] * a[8]
  mul r3, r3, r7 // a[1] * a[3] * a[8]

  // sum all to r1
  sub r1, r1, r2
  sub r1, r1, r3
  add r1, r1, r4
  add r1, r1, r5
  sub r1, r1, r6

  mov r0, r1 //return

  pop {r4-r7}

  bx lr
