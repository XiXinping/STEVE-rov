# Camera For ROV

It is 2 python files, one being for the server (RPI Abord ROV) and one for the client (Control Staion). It uses OpenCSV windows for each camera, the control pannel where the data from different sensors will be in a different UI. 

# How It Runs and Jankyness Level

It uses websockets, and most of the code was found on the internet. From what I understand it pickles each frame, then sends it in a packet preceded by "Q" and the length of the tuple. It is quite fragile and extremely janky.

# Networking Required for it to Functon. 

It uses the localhost of the server machine, to use it on multiple computers you first must set up either a reverse proxy or ssh tunnel. 

You can use `ssh {username}@{ip address} -L 9999:{localhost on server machine}:9999` to set up the tunnel required. 

In the client python file there is a variable for the ip address of the server, simply put the localhost ip address of the client machine. This can be found by running `ifconfig` in a terminal, the `lo` ip address is localhost for your machine. 
