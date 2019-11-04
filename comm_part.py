from bluetooth import *
import lightblue
target_name="Galaxy S10+"
file_to_send = "/home/pi/uday1/anu.txt" 
target_address=None
obex_port = None                 
nearby_devices=discover_devices()
for address in nearby_devices:
    if target_name==lookup_name(address):
        target_address=address
        break
if target_address is not None:
    print("found target bt device with address",target_address)
else:
    print("could not find the bluetooth device")
    
print "searching for the object push service..."  
services = lightblue.findservices(target_address)  
for service in services:  
    if service[2] == "OBEX Object Push":  
        obex_port = service[1]       
        print "OK, service '", service[2], "' is in port", service[1], "!"  
        break  
print "sending a file..."  
try:  
    lightblue.obex.sendfile( target_address, service[1], file_to_send )  
    print "completed!\n"  
except:  
    print "an error occurred while sending file\n"  
