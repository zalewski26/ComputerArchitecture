#!/bin/bash
> tmp;
echo 0 0 >> tmp;
i=1;
receive=`awk '{if($1=="wlp3s0:") print $2}' /proc/net/dev`;
transmit=`awk '{if($1=="wlp3s0:") print $10}' /proc/net/dev`;
sumDown=0;
sumUp=0;
while [ 1 ];
do
    sleep 1;
    clear;
    ./plot.sh;

	current=`awk '{if($1=="wlp3s0:") print $2}' /proc/net/dev`;
    speedDown=$((current-receive));
    receive=$current;
    current=`awk '{if($1=="wlp3s0:") print $10}' /proc/net/dev`;
    speedUp=$((current-transmit));
    transmit=$current;

    sumDown=$((sumDown+speedDown));
    sumUp=$((sumUp+speedUp));

    echo Current downloading:;
    ./convert.sh $speedDown;
    echo Current uploading:;
    ./convert.sh $speedUp;
    echo Average downloading: 
    ./convert.sh $((sumDown/i));
    echo Average uploading:
    ./convert.sh $((sumUp/i));

    echo $i $((sumDown/i)) >> tmp;

    sec=`awk '{print $1}' /proc/uptime`;
	min=$(echo "$sec/60" | bc);
	hours=$(echo "$min/60" | bc);
	days=$(echo "$hours/24" | bc);
	sec=$(echo "$sec-($min*60)" | bc);
	min=$(echo "$min-($hours*60)" | bc);
	hours=$(echo "$hours-($days*24)" | bc);

	echo Lifetime: $days days $hours hours $min minutes $sec seconds;

	battery=`awk -F "=" '/POWER_SUPPLY_CAPACITY=/{print $2}' /sys/class/power_supply/BAT0/uevent`;

	echo Battery: $battery%;

	load=`awk '{print $0}' /proc/loadavg`;

	echo Average load: $load;

    ((i++));
done;

