#!/bin/sh

echo "## START SCRIPT ##"

echo "## MAKE ##"
make

exitcode=1

while [ $exitcode -ne 0 ]
do

    if [ ! -f riks-server ]
    then
        echo "## EXECUTABLE NOT FOUND ##"
        echo "## END SCRIPT ##"
        exit 1
    fi

    echo "## WAITING BEFORE RELAUNCH... ##"

    sleep 5

    echo "## LAUNCHING SERVER ##"

    ./riks-server

    exitcode=$?
    echo "## SERVER EXITED WITH CODE $exitcode ##"

done

echo "## END SCRIPT ##"
