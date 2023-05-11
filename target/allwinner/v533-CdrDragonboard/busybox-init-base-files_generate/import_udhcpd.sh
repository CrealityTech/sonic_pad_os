#!/bin/bash

pwd=$(dirname $0)

cp -vf $pwd/udhcpd.conf $1/etc

exit 0
