.global start

// selve strengen er i r0
// lengden på strengen er i r1
// counter 0 er i r3
// len(s)-1 er i r4
// s[i] er i r5
// s[j] er i r6
// resultat 0/1 om palindrom eller ikke er i r12, antar 1 inntil videre

_start:
    ldr r0, =input // laster inn streng
    bl check_length
    b _exit

check_length:
    mov r1, #0
    ldrb r2, [r0, r1]
    cmp r2, #0
    beq end_cnt_length
    add r1, r1, #1
    b check_length

end_cnt_length: 
    mov r3, #0
    sub r4, r1, #1
    mov r12, #1
    bx lr

check_palindrome:
    // laster inn venstre og høyre karakter
    ldrb r5, [r0, r3]
    ldrb r6, [r0, r4]
    // sjekk om det er en ?
    
    // sjekk om det er en #
    // sjekk om det er et mellomrom
    // store/små bokstav-check

advance:
    add r5, r5, #1
    sub r6, r6, #1

_exit:
    b .


.data:
    input: .asciz "Grav ned den varg" // nullbit på slutten, enklere å telle 

