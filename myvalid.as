Welcoming: .string "Good Morning!"
.extern L1
.extern L4
L2: .data -2,4,700,82
L3: .data -100,22,-85,930,52

prn Welcoming
lea L2,r3
add #300,r6
cmp r1,r2
sub r5,r3
mov r0,L2
cmp L3,L1
prn r3
prn #98
clr r1
prn r1
not r5
inc r1
jmp &Welcoming
red r2
add L4,L2

stop

.entry Welcoming
.entry L2