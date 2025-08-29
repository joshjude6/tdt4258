.global _start

// selve strengen er i r0
// lengden på strengen er i r1
// counter 0 er i r3 = i
// len(s)-1 er i r4 = j
// s[i] er i r5
// s[j] er i r6
// addresse for JTAG er i r7
// addresse for LED er i r8

_start:
    ldr r0, =input // laster inn streng
    ldr r7, =0xFF20100 // JTAG addresse
    ldr r8, =0xFF20000 // LED addresse
    ldr r9, =not_palin // streng for ikke-palindrom
    ldr r10, =is_palin // streng for palindrom
    bl check_length
    bl check_palindrome
    b _exit

check_length:
    mov r1, #0
    ldrb r2, [r0, r1] // sjekker siste byte for null -> gå videre til slutt condition
    cmp r2, #0
    beq end_cnt_length // brancher til neste dersom nullbyte 
    add r1, r1, #1
    b check_length

end_cnt_length: 
    mov r3, #0
    sub r4, r1, #1
    mov r12, #1
    bx lr

check_palindrome:
    // sjekk om i >= j
    cmp r3, r4
    bge is_palindrome
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

comp:
    cmp r5, r6
    bne not_palindrome

advance:
    add r3, r3, #1 // i++
    sub r4, r4, #1 // j--
    b check_palindrome

inc:
    add r3, r3, #1
    b check_palindrome

dec:
    sub r4, r4, #1
    b check_palindrome

wild_left:
    cmp r6, #32
    beq not_palindrome
    add r3, r3, #1
    b check_palindrome

wild_right:
    cmp r5, #32
    beq not_palindrome
    sub r4, r4, #1
    b check_palindrome


is_palindrome:
    // lys opp de 5 LEDene lengst til høyre
    // skrive til JTAG UART box = Is a palindrome
    ldr r11, =0b0000011111
    str r11, [r8]
    b _exit


not_palindrome:
    // lys opp de 5 LEDene lengst til venstre
    // skrive til JTAG UART box = Not a palindrome
    ldr r11, =0b1111100000
    str r11, [r8]
    b _exit
    

_exit:
    b .


.data
    input: .asciz "Grav ned den varg" // nullbit på slutten, enklere å telle 
    not_palin: .asciz "Not a palindrome"
    is_palin: .asciz "Is a palindrome"



