#!/bin/bash
cd /proc;
echo -e Pid "\t" PPid "\t" State "\t" Opened files "\t" Name
for file in *;
do
   if [[ $file =~ ^[0-9]+$ ]] && [ -e $file ]; then
      pid=`awk '$1=="Pid:"{print $2}' /proc/"$file"/status`;
      ppid=`awk '$1=="PPid:"{print $2}' /proc/"$file"/status`;
      state=`awk '$1=="State:"{print $2}' /proc/"$file"/status`;
      name=`awk '$1=="Name:"{print $2}' /proc/"$file"/status`;

   counter=`lsof -p $pid | wc -l`;
   counter=$((counter-1));

   echo -e $pid "\t" $ppid "\t" $state "\t" $counter "\t\t" $name
   fi
done;
