#! /bin/bash
echo "Creating pipe into /tmp/vpipe1 and /tmp/vpipe2"
mkfifo /tmp/vpipe1 /tmp/vpipe2

cd "$(dirname "$0")"
echo "Starting the first process that send through vpipe1 and receive from vpipe2"
xterm -e ./serial-talker /tmp/vpipe1 /tmp/vpipe2

echo "Starting the second process that send through vpipe2 and receive from vpipe1"
xterm -e ./serial-talker /tmp/vpipe2 /tmp/vpipe1
