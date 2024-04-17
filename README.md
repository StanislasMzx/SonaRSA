# SonaRSA

___

Made by :

- Stanislas MEZUREUX : stanislas.mezureux@telecomnancy.eu
- Adrien LAROUSSE : adrien.larousse@telecomnancy.eu

___



## Compile the project

On the root folder :

```bash
make all
```

## Order to run the SonaRSA

1. Launch the agent
2. Launch the manager

## Run the agent

On a terminal, run :

```bash
cd src/
sudo ./src/agent/agent
```

## Run the manager

On a new terminal, run :

```bash
cd src/
./manager/client 127.0.0.1 2222
```

## Features of the manager

1. Scan auto of network with `scan -a`
2. Scan of a list IP address on the network with `scan -l ip1 ... ipn`
3. Scan of the the thousand most used ports with `scan -p ip`
4. Scan of a range of ports for an IP address with `scan -p ip stard_port end_port`
5. Scan of a port of an IP adress with `scan -p ip port`



