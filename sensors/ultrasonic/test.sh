#!/bin/bash

#################################
### Author: Pontus Laestadius ###
### Last modified: 2018-03-22 ###
#################################

# Write command 0x51 to the I2C register 0x71.
i2cset -y 1 0x71 0x51

# Wait an approximate 65mS.
sleep 0.1

# Read the whole chip register from the register 0x71 .
i2cget -y 1 0x71


