# Create the image by typing: `docker build -t ms/riks-server .`
# Run it by entering the command: `docker run --rm -ti -p 9002:9002 ms/riks-server`

FROM debian
LABEL maintainer="yossef.rostaqi@etu.unistra.fr"

# Updating and installing dependencies
RUN apt-get update
RUN apt-get install -y gcc g++ sqlite3 git make

# Changing the WORKDIR
WORKDIR /riks-game-server

# Adding the src files to WORKDIR in the container
ADD . .

# Build
RUN make

# Command to run when the container is run
CMD ["./riks-server"]

# Exposing the port needed by the server
EXPOSE 9002
