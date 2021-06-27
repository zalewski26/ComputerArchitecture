#!/bin/bash
exec 3>&2;
exec 2>/dev/null;
joke=`curl https://api.icndb.com/jokes/random | jq .value.joke`;
url=`curl https://api.thecatapi.com/v1/images/search | jq .[].url`;
url=`echo "${url//\"}"`;
curl $url > cat.jpg;
exec 2>&3;
catimg -w 200 cat.jpg;
echo $joke;
