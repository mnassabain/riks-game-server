# Riks Serveur Jeu

Ce depôt est un fork du depôt originel du Groupe MS: https://git.unistra.fr/ms/riks-game-server

Partie serveur jeu

## Docker

You can run the server by using Docker containerization. 
The image is based on Debian, it copies the source files and builds the program inside the container.

### Usage
#### Build
```bash 
$ make docker-build
```
#### Run
```bash 
$ make docker-run
```
#### Stop
```bash 
$ make docker-stop
```