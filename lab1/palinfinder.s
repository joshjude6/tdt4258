.global _start
_start:
    ldr r0, =input
	mov r1, #0
    bl  check_length        // r1 = length of string

    // initializing indexes here
    mov r3, #0              // i = 0
    sub r4, r1, #1          // j = len-1

    bl  check_palindrome
    b   _exit
	
check_length:
    ldrb r2, [r0, r1]
    cmp  r2, #0
    beq  end_cnt_length
    add  r1, r1, #1
    b    check_length
	
end_cnt_length:
    bx   lr 

check_palindrome:
    // saving the r4 value since it is callee-saved
    push {r4}

    // checking if i is greater or equal to j
    cmp r3, r4
    bge is_palindrome      

    // loading characters input[i] and input[j]
    ldrb r5, [r0, r3]
    ldrb r6, [r0, r4]

    // checking if one of them is a ?
    cmp r5, #63
    beq wild_left
    cmp r6, #63
    beq wild_right

    // checking if one of them is a #
    cmp r5, #35
    beq wild_left
    cmp r6, #35
    beq wild_right

    // checking if one of them is a space
    cmp r5, #32
    beq inc
    cmp r6, #32
    beq dec

    // converting the cases
    cmp r5, #'A' // this checks if r5 is below 'A' (meaning it is not uppercase)
    blt converting_r6
    cmp r5, #'Z' // this checks if r5 is above 'Z' (meaning it is not uppercase)
    bgt converting_r6
    add r5, r5, #32 // if it is uppercase, converts r5 from uppercase to lowercase by adding 32

converting_r6: // same as right before with r5
    cmp r6, #'A'
    blt comp
    cmp r6, #'Z'
    bgt comp
    add r6, r6, #32
    b   comp

comp:
    cmp r5, r6
    bne not_palindrome
    b   advance

advance:
    add r3, r3, #1          // i++
    sub r4, r4, #1          // j--
    b   check_palindrome

inc:
    add r3, r3, #1
    b   check_palindrome

dec:
    sub r4, r4, #1
    b   check_palindrome

wild_left:
    cmp r6, #32 // only checking if the corresponding character on the other side is a space, since it is a wildcard
    beq not_palindrome // moving on if not space
    add r3, r3, #1
	sub r4, r4, #1
    b   check_palindrome

wild_right:
    cmp r5, #32 
    beq not_palindrome 
	add r3, r3, #1
    sub r4, r4, #1
    b   check_palindrome

is_palindrome:
    pop {r4} // done to restore r4 and avoid clobbering
    ldr r1, =0xFF200000
	mov r0, #0x1F
	str r0, [r1]
	ldr r0, =is_palin
    b write_jtag

not_palindrome:
    pop {r4} // done to restore r4 and avoid clobbering
    ldr r1, =0xFF200000
	mov r0, #0x3E0
	str r0, [r1]
	ldr r0, =not_palin
    b write_jtag
	
write_jtag:
	ldr r1, =0xFF201000
	ldrb r2, [r0], #1 // loading the character from r0 and incrementing index
	cmp r2, #0
	beq _exit
	str r2, [r1] // writing character to jtag address
	b write_jtag

_exit:
    b .

.data
    input: .asciz "Grav ned den varg"
    not_palin: .asciz "Not a palindrome"
    is_palin: .asciz "Is a palindrome"
