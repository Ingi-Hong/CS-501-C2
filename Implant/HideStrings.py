

def xor(s):
    f = ''
    for i in range(len(s)):
        f += chr(ord(s[i]) ^ ord('9'))
    return f


a = ["CreateProcessA", "LoadLibraryA", "Sleep", "GetProcAddress", "FreeLibrary","GetStartupInfoA"]
for f in a:
    res = xor(f)
    #print(res, xor(res))
    #print("---------")

print("dekernel",xor("Kernel32.dll") )
print("COOL BEANS")