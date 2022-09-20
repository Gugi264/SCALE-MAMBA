# one_mult-0--0
ldint r0, 0 # 0
pushint r0 # 1
# one_mult-0-begin-loop-1
popint r1 # 2
triple 3, s0, s1, s2 # 3
triple 3, s4, s5, s6 # 4
subs s7, s0, s4 # 5
subs s8, s2, s5 # 6
startopen 2, s7, s8 # 7
stopopen 2, c0, c1 # 8
mulm s9, s5, c0 # 9
mulm s10, s4, c1 # 10
mulc c2, c0, c1 # 11
adds s11, s6, s9 # 12
adds s12, s11, s10 # 13
addm s3, s12, c2 # 14
triple 3, s14, s15, s16 # 15
subs s17, s1, s14 # 16
subs s18, s2, s15 # 17
startopen 2, s17, s18 # 18
stopopen 2, c3, c4 # 19
mulm s19, s15, c3 # 20
mulm s20, s14, c4 # 21
mulc c5, c3, c4 # 22
adds s21, s16, s19 # 23
adds s22, s21, s20 # 24
addm s13, s22, c5 # 25
triple 3, s24, s25, s26 # 26
subs s27, s0, s24 # 27
subs s28, s1, s25 # 28
startopen 2, s27, s28 # 29
stopopen 2, c6, c7 # 30
mulm s29, s25, c6 # 31
mulm s30, s24, c7 # 32
mulc c8, c6, c7 # 33
adds s31, s26, s29 # 34
adds s32, s31, s30 # 35
addm s23, s32, c8 # 36
startopen 1, s23 # 37
stopopen 1, c9 # 38
startopen 1, s2 # 39
stopopen 1, c10 # 40
print_char4 1936287860 # 41
print_char4 1869116192 # 42
print_char4 543452277 # 43
print_char4 1696621922 # 44
print_char4 1818326385 # 45
print_char 32 # 46
print_reg c9 # 47
print_char 44 # 48
print_char 32 # 49
print_reg c10 # 50
print_char 10 # 51
startopen 1, s0 # 52
stopopen 1, c11 # 53
print_char4 1936287860 # 54
print_char4 544434464 # 55
print_char4 1970037110 # 56
print_char4 996221029 # 57
print_char 32 # 58
print_reg c11 # 59
print_char 10 # 60
ldint r2, 1 # 61
addint r3, r1, r2 # 62
pushint r3 # 63
ldint r4, 10 # 64
ltint r5, r3, r4 # 65
jmpne r5, 0, -65 # 66
# one_mult-0-end-loop-2
popint r6 # 67
