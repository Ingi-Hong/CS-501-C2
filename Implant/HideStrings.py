

def xor(s):
    f = ''
    for i in range(len(s)):
        f += chr(ord(s[i]) ^ ord('9'))
    return f


a = ["CreateProcessA"]
for f in a:

    res = xor(f)
    print(res)
    print(xor(res))
