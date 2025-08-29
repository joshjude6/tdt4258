.global _start
_start:
    ldr r0, =input
	mov r1, #0
    bl  check_length        // r1 = length of string

    // initializing indexes here
    mov r3, #0              // i = 0
    sub r4, r1, #1          // j = len-1

    bl check_palindrome
    b _exit
	
check_length:
    ldrb r2, [r0, r1] // loading byte at r0+r1 into r2
    cmp  r2, #0 // comparing r2 to nullbyte
    beq  end_cnt_length // if null, end the loop
    add  r1, r1, #1 // increment r1
    b    check_length // start over
	
end_cnt_length:
    bx lr // return from the function

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
    bne not_palindrome // if r5 and r6 are not equal, the string is not a palindrome
    b   advance

advance: // increment one side and decrement the other
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
    cmp r6, #32 // wildcard can't match space
    beq not_palindrome // moving on if r6 is not a space
    b advance 

wild_right:
    cmp r5, #32 // wildcard can't match space
    beq not_palindrome // moving on if r5 is not a space
	b advance

is_palindrome:
    pop {r4} // done to restore r4 and avoid clobbering
    ldr r1, =0xFF200000 // LED address
	mov r0, #0x1F // value corresponds to LED 6-10
	str r0, [r1] // activate LED
	ldr r0, =is_palin // load correct string for is_palindrome
    b write_jtag // move on to writing to JTAG

not_palindrome:
    pop {r4} // done to restore r4 and avoid clobbering
    ldr r1, =0xFF200000
	mov r0, #0x3E0 // value corresponds to LED 1-5
	str r0, [r1] // activate LED
	ldr r0, =not_palin // load correct string for not_palindrome
    b write_jtag // move on to writing to JTAG
	
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
.align
    input: .asciz "Grav ned den varg"
    not_palin: .asciz "Not a palindrome"
    is_palin: .asciz "Is a palindrome"
.end
