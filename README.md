#  Underwater ROV

Code and Documentation for the MVTHS MATE Robotics Team 2021/2022

## Basic Client Server 

Websockets are the primary method of communication between different processes. More information about websockets can be found [here](https://websockets.readthedocs.io/en/stable/).

The Raspberry Pi will host a main websocket server that receives joystick data from a PC, receives camera data, sends commands to an arduino, and sends data to a web client.

## Basic Web Server

The main server hosted on the pi can be used as a web server. A web client will connect to the server and display sensor data and a live camera feed.

# Commiting Guide

What programming conventions should be used for different programming languages.

* [General](##General)
* [C/C++/Arduino](##C/C++/Arduino)
* [Python](##Python)
* [Other Languages](##Other_Languages)

## General

**COMMENT, COMMENT, COMMENT!**

We get points for documentation, therefore, the more comments you make the better (within reason, of course)

With that said, make sure your comments are actually correct. Wrong or misleading comments are worse than no comments

Update READMEs in the respective directories of a commit if your commit changes the scope, or how to run that section of code

The conventions that we will be using for different languages, are just that conventions. Only follow them when it makes sense, if breaking the convention will make your code more readable, do it.

## Styling Guidelines

### C/C++/Arduino

We will be using the [Google C++ style guide](https://google.github.io/styleguide/cppguide.html)
You can use cpplint to check if you are following the conventions.

cpplint can be installed with
`pip3 install cpplint`

The gist of it is to keep your lines under 80 characters, use snake_case for variable names, and to use the following syntax for functions, if statements, and switch statements.


#### Functions

Functions should be very small, so that it can be portable to other files.

```
int some_function() {
    // stuff here
}
```

#### If Statements
```
if (condition) {                   // no spaces inside parentheses, space before brace
  DoOneThing();                    // two space indent
  DoAnotherThing();
} else if (int a = f(); a != 3) {  // closing brace on new line, else on same line
  DoAThirdThing(a);
} else {
  DoNothing();
}   
```

#### Switch Statements

```
switch (var) {
  case 0: {  // 2 space indent
    ...      // 4 space indent
    break;
  }
  case 1: {
    ...
    break;
  }
  default: {
    assert(false);
  }
}
```

### Python

We will be using the [PEP8 standard styleguide for python](https://www.python.org/dev/peps/pep-0008/)
PEP8 is likely how you learned how to write python, so it should come naturally.

You can use the PEP8 linter that can be installed with
`pip3 install pep8`

Use snake case for variable naming, and try to keep your code clean and portable.
Use spaces instead of tabs, most IDEs will convert tabs to spaces automatically. 

#### Functions 

Functions should be small and portable. 

```
def function:
    # Do stuff
```

#### If Statements

```
if something == true:
    # do stuff
```

### Other Languages

Try to avoid using superfluous languages, for instance don't write something in Java when it would make just as much sense to write it in Python.

Try to keep your code readable, if there are standard conventions for you language use those, and comment what is happening in your code even more, as contributers may not know that language.

If a language is used in the project extensively, please add conventions to the README

### Languages That Will Likely Be Used

Please add any new language that you will be using.

* HTML
* JavaScript
* CSS
* C/Arduino/C++
* Python
* Perl
* Makefile
* Bash

## Raspberry Pi Server

### RPI

The underwater-rov directory is contains all relevant code. Anything else on the home directory should not be considered and is irrelevant.

Additionally, ignore the pop-up that appears upon booting up the RPI.

### main_server

main_server contains the main python code that will handle communication between clients and the arduino. 

As of January 27, 2022, whether or not camera data will pass through it is undecided.

Communication will be handled by the python websockets library.

Port 8765 will be used to send main server data to the client. 

### Camera 

All files pertaining to the camera are in the camera folder.



