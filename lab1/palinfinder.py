def palindrome_checker(s):
    s = ''.join(c.lower() for c in s if c.isalnum() or c in ['?', '#'])
    i, j = 0, len(s) - 1
    if len(s) < 2:
        return False
    while i < j:
        l, r = s[i], s[j]
        if l == r or l in ['?', '#'] or r in ['?', '#']:
            i += 1
            j -= 1
        else:
            return False
    return True



for t in ["malayalam", "abc?dc#a", "renal"]:
    print(t, "->", "Palindrome" if palindrome_checker(t) else "Not palindrome")



