select * from avrdevices
where lower(name)  not in (
select lower(devicename) from AllDevices where devicetype=2
)

select devicename from AllDevices 
where lower(devicename) not in (select lower(name) from avrdevices)
and devicetype=2  