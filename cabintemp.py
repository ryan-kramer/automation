#!/usr/bin/python3

import asyncio
import aiohttp
import pysmartthings

token = '3c140e3a-3e8f-4ad4-8c39-1c349deb2034'
sensor= ['5ecc96a2-102f-4a19-81e1-41eecc20df7f','d6160bfc-a81e-43f5-bdf6-fd28b1033466']


async def main():
        async with aiohttp.ClientSession() as session:
                api = pysmartthings.SmartThings(session, token)
                devices=await api.devices(location_ids=None, capabilities=None, device_ids=sensor)
                for device in devices:
                        await device.status.refresh()
                        myvals=device.status.values
                        if device.device_id == '5ecc96a2-102f-4a19-81e1-41eecc20df7f':
                                f=open('/home/weewx/archive/cabintemp.txt','w')
                                f.write(str(myvals['temperature']))
                                #print(str(myvals['temperature']))
                                f.close()
                        if device.device_id == 'd6160bfc-a81e-43f5-bdf6-fd28b1033466':
                                f=open('/home/weewx/archive/welltemp.txt','w')
                                f.write(str(myvals['temperature']))
                                #print(str(myvals['temperature']))
                                f.close()

loop = asyncio.get_event_loop()
loop.run_until_complete(main())
loop.close()

