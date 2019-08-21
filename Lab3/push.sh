

for file in $@
do
	scp $file root@192.168.7.2:/root/incoming 
	ssh root@192.168.7.2 "cd /root/incoming;rmmod $file; insmod $file"
done


