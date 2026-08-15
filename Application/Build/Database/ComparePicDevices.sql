select * from picdevices
where lower(name) not in (select lower(devicename) from AllDevices where devicetype=1)

select devicename from AllDevices 
where lower(devicename) not in (select lower(name) from picdevices)
and devicetype=1  