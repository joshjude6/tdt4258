.global _start

// r0 = input, r1 = len, r3 = i, r4 = j, r5/r6 = chars
// r7/r8 = JTAG/LED base (unused/used)

_start:
    ldr r0, =input
    ldr r7, =0xFF20100      // leave as-is if you like; not relevant to AAPCS
    ldr r8, =0xFF20000
    ldr r9, =not_palin
    ldr r10, =is_palin
	mov r1, #0
    bl  check_length        // r1 = len
    mov r3, #0              // i = 0           <-- moved out of check_length
    sub r4, r1, #1          // j = len-1       <-- moved out of check_length

    bl  check_palindrome
    b   _exit
	
check_length:
    ldrb r2, [r0, r1]
    cmp  r2, #0
    beq  end_cnt_length
    add  r1, r1, #1
    b    check_length
	
end_cnt_length:
    bx   lr                  // <-- no writes to r3/r4 here

check_palindrome:
    push {r4}                // preserve callee-saved r4

    // sjekk om i >= j
    cmp r3, r4
    bge pal_ok               // was: bge is_palindrome

    // laster inn venstre og høyre karakter
    ldrb r5, [r0, r3]
    ldrb r6, [r0, r4]

    // sjekk om det er en ?
    cmp r5, #63
    beq wild_left
    cmp r6, #63
    beq wild_right

    // sjekk om det er en #
    cmp r5, #35
    beq wild_left
    cmp r6, #35
    beq wild_right

    // sjekk om det er et mellomrom
    cmp r5, #32
    beq inc
    cmp r6, #32
    beq dec

    // store/små bokstav-check
    cmp r5, #'A'
    blt convert_case_r6
    cmp r5, #'Z'
    bgt convert_case_r6
    add r5, r5, #32

convert_case_r6:
    cmp r6, #'A'
    blt comp
    cmp r6, #'Z'
    bgt comp
    add r6, r6, #32
    b   comp

comp:
    cmp r5, r6
    bne pal_fail            // was: bne not_palindrome
    b   advance

advance:
    add r3, r3, #1          @ i++
    sub r4, r4, #1          @ j--
    b   check_palindrome

inc:
    add r3, r3, #1
    b   check_palindrome

dec:
    sub r4, r4, #1
    b   check_palindrome

wild_left:
    cmp r6, #32
    beq pal_fail            // was: beq not_palindrome
    add r3, r3, #1
	sub r4, r4, #1
    b   check_palindrome

wild_right:
    cmp r5, #32
    beq pal_fail            // was: beq not_palindrome
	add r3, r3, #1
    sub r4, r4, #1
    b   check_palindrome

pal_ok:
    pop {r4}                // restore callee-saved before leaving
    b   is_palindrome

pal_fail:
    pop {r4}
    b   not_palindrome

is_palindrome:
    ldr r1, =0xFF200000
	mov r0, #0x1F
	str r0, [r1]
    b _exit

not_palindrome:
    ldr r1, =0xFF200000
	mov r0, #0x3E0
	str r0, [r1]
    b _exit

_exit:
    b .

.data
    input: .asciz "Grav ned den varg"
    not_palin: .asciz "Not a palindrome"
    is_palin: .asciz "Is a palindrome"
