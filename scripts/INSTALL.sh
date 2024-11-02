#!/bin/bash

echo "Blackjack will be installed in /usr/local/bin"
sudo g++ src/main.cpp -Ofast -o /usr/local/bin/blackjack
echo "Done, use 'blackjack' to play"
