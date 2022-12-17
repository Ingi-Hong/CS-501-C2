from rsa import decrypt, key
q=10608097729759645865370348156228792171509504695437771230722945218294221283813126204278002934451748552882331175717239375829829050284842102610059982447404739
p=13275552883534770395208193491738709084985449227659684111768157102680399037616040210916489999633832931342912488838796886573275612410673053593839534017306351
n=140828362405129318180427061309685227655906966463984880667730211170307388233674487689335840879732073147346024672417998685248299007658583633553801413340975475503547903339313984964758797906071034777882942760516163348737996240254324742241395253273562247739849723735713460845704358491715187760890904440251452197389
e=65537
d=139680883186911531954198395811731828635990670019354093077871536944069627193090051776399555437466224435005167184661903482570077672075787905029185853356171739347974175576979214463238680401251984340528786260554229491873167818057084049617970440528513243279067282580506507153640774311069672909188591776391906733173

def rsadecrypt(bytes):
    rsakey=key.PrivateKey(n,e,d,p,q)
    decrypted=decrypt(bytes,rsakey)
    return decrypted
