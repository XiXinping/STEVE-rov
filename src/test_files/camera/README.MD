# Camera Software For Underwater ROV

The webclient is from https://github.com/Bronkoknorb/PyImageStream
it essentially makes a stream of jpeg images that stream to a javascript client in a browser. 

The old camera is for a desktop client

# How To Run

To run this make sure that you have all of the dependancies, do that install `sudo apt-get install git curl libsdl2-mixer-2.0-0 libsdl2-image-2.0-0 libsdl2-2.0-0`

To expose the server (running on localhost), you have to run `ssh pi@<ip of pi> -L 8888:localhost:8888` on the machine that you would like to acsess the server from
