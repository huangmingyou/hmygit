#!/bin/bash
destip=$1
proto=$2
timelong=$3
hm=`date +%H:%M`
sec=`date +%S`
startsec=$(($sec+5))
endsec=$(($startsec+$timelong))
echo "iptables -A FORWARD -d $destip -p $proto   -i eth0 -m time --timestart $hm:$startsec --timestop $hm:$endsec  -j DROP"

