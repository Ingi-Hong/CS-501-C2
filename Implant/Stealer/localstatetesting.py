f = open('C:\Users\Wyatt2\AppData\Local\Google\Chrome\User Data\Local State', 'r', encoding='UTF-8')
g = f.read()
loc = g.find("encrypted_key") + 16
k = ''
s = loc

while g[loc] != "\"":
    k += g[loc]
    loc += 1
print(loc - s)
print(k)