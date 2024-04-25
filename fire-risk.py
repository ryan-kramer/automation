
#!/usr/bin/python3
import urllib3
import re

url="https://www.wfas.net/data/nplains/FDBISBC.txt"
http = urllib3.PoolManager()
response = http.request('GET',url)

riskscore=""
data=str(response.data)
#print(data)
pattern='Grant County...(\w+)'
m = re.search(pattern,data)
risk=m.group(1)
#print(risk)

riskscore=0

if re.search('LOW',risk):
        print('low risk')
        riskscore=0
if re.search('MODERATE',risk):
        print('moderate risk')
        riskscore=1
if re.search('HIGH',risk):
        print('high risk')
        riskscore=2
if re.search('VERY',risk):
        print('very high risk')
        riskscore=3
if re.search('EXTREME',risk):
        print('extreme risk')
        riskscore=4



f = open('/home/weewx/archive/fire.txt','w')
f.write(str(riskscore))
f.close()



