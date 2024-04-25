
#!/usr/bin/python3
import urllib3
import xmltodict
url="https://water.weather.gov/ahps2/hydrograph_to_xml.php?gage=glun8&output=xml"
http = urllib3.PoolManager()
response = http.request('GET',url)
try:
        data = xmltodict.parse(response.data)
except:
        print("failed to parse xml from response (%s)" % traceback.format_exc())

f = open('/home/weewx/archive/waterlevel.txt','w')
f.write(data['site']['observed']['datum'][0]['primary']['#text'])
f.close()
print(data['site']['observed']['datum'][0]['primary']['#text'])
print('reading data')

f= open("/home/weewx/archive/waterlevel.txt","r")
total_power=f.read()
f.close()
print(total_power)


