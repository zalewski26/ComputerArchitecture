#!/bin/bash
>prev;
>current;
>>history;

site=$1;
if [ -z $1 ]; then
   site="http://ki.pwr.edu.pl";
fi

sleeptime=$2;
if [ -z $2 ]; then
   sleeptime=7;
fi

lynx -dump $site > prev;
dateprev=$(date);
exec 3>&1;
exec 1>/dev/null;
git add prev;
git commit -m "base";
exec 1>&3;

while [ true ]
do
   sleep $sleeptime;

   lynx -dump $site > current;
   datecurr=$(date);

   changes=`diff current prev`;
   if [ -n "$changes" ]; then
      echo "_______________________________";
      echo $dateprev;
      echo Changes:;
      diff current prev;
      echo $datecurr;
      echo "_______________________________";

      echo "_______________________________">>history;
      echo $dateprev>>history;
      echo Changes:>>history;
      diff current prev >>history;
      echo $datecurr>>history;
      echo "_______________________________">>history;

      cat current > prev;
      dateprev=$datecurr;
      exec 3>&1;
      exec 1>/dev/null;
      git add prev;
      git commit -m "changes";
      exec 1>&3;
   fi
done;
