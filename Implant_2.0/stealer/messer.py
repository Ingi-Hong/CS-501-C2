import json

def main():
    #test = '{"KEY":"1babf62120fc3ac18da6b95fb3b6271013b8e957535eda157c6a1581cbf12bc9","RECORDS":[{"password":"763130755d3e730a7236ed1b2d9957996ee22ad8df0d73becedb4562c479e52c2402673c5e7c860072654a","url":"https://www.walmart.com/","username":"wyattmorris24@gmail.com"},{"password":"7631300c0af71d93a79cfdb5343e49c1d3a2f56245769f1d4bd182b1444736f61d0ec1e24b7cdb99a6135fcaf8a394f1ee23","url":"https://ndscindv.com/","username":"dfvndfvn"},{"password":"763130aa082079b9e560b3c6b3886fbccdcf79cd14f0e2115307fc69189eabc8f1ced908f8e16ccf","url":"https://a.com/","username":"bcool"}]}'
    f = open("dump.txt", 'r')
    g = f.read()
    json_res = json.loads(g)
    key = json_res['KEY']
    cookies = json_res['COOKIES']
    passwords = json_res['PASSWORDS']
    print("KEY:   ", key)
    for ele in cookies:
        print(cookies[ele][0], "\n\n")
    for e in passwords:
        passwords[e]['password']

        print(passwords[e][0], "\n\n")

def a():
    f = open('../newbad.txt')
    g = f.read()
    c = json.loads(g)
    

#50-58
a()