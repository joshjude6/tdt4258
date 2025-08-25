def palindrome_checker(s):
    i = 0
    j = len(s) - 1
    is_palindrome = True
    while i < j:
        if s[i] != s[j]:
            is_palindrome = False
            break
        else:
            i += 1
            j -= 1

    if is_palindrome:
        print("String is a palindrome")
    else:
        print("String is not a palindrome")


palindrome_checker("malayalam")
