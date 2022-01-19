# Sockets Alternative For Main Server

A basic main server written using sockets instead of websockets. It may or may not be used depending on a yet-to-be-made decision.

The server opens a TCP socket, binds to it and starts listening. The client then will connect to it and send commands such as `refresh` to get sensor data, and the like. 

The server will decode that data, to make it a sting that can be interpreted by if statements more easily. The if statements will execute funcions located in the arudino_functions file. 

# Reasons to use Sockets over Websockets

* Uses more traditional, sequential programming, as opposed to event based programming.
* Will use less bandwidth, due to requests not needing HTTP boilerplate. 
* Gives more granular control
* We can use all libraries, not just ones that are compatible with asyncio

# Reasons to use Websockets over Sockets

* Makes threading easier
* A layer of abstraction that could make programming it easier
* Uses HTTP, so less code will be needed in the web client

