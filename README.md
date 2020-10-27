# kauri_iot_task
Server - client (de/en)coding length

## Dependencies
* cmake
* ace

## Instructions
```bash
chmod +x ./run.sh
./run.sh -p <number port>
```
or start manualy

```bash
mkdir build
cd build 
cmake ..
make -j 2

# in on terminal start server
./Server/Server_MT 5000
# in second terminals can start multiple clients 
./Client/Client 5000 0.0.0.0
# then input tlv string like in exaple right client input and press enter
# translation will be shown in Server terminal
# my tlvlib not check incorrect string! if you enter it, programm can terminate
# or indefind befavior
```

## Exaple right client input

6F1A840E315041592E5359532E4444463031A5088801025F2D02656E \
6f2f840e325041592e5359532e4444463031a51dbf0c1a61184f07a0000000031010500a564953412044454249548701019000
