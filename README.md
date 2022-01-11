#  Underwater ROV

Code and Documentation for the MVTHS MATE Robotics Team 2021/2022

## Basic Client Server 

The Client Server will use the 

## Basic Web Server

Robot will use Flask web server. Server directory is under RPIwebserver. Flask code will be in the helloworld.py file. HTML templates will be in the templates directory. There will only be HTML. Data from the RPI will go into the HTML file, any item inside {{}} refers to data from the RPI. 

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

## C/C++/Arduino

We will be using the [Google C++ style guide](https://google.github.io/styleguide/cppguide.html)
You can use cpplint to check if you are following the conventions.

cpplint can be installed with
`pip3 install cpplint`

The gist of it is to keep your lines under 80 characters, use snake_case for variable names, and to use the following syntax for functions, if statements, and switch statements.


### Functions

Functions should be very small, so that it can be portable to other files.

```
int some_function() {
    // stuff here
}
```

### If Statements
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

### Switch Statements

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

## Python

We will be using the [PEP8 standard styleguide for python](https://www.python.org/dev/peps/pep-0008/)
PEP8 is likely how you learned how to write python, so it should come naturally.

You can use the PEP8 linter that can be installed with
`pip3 install pep8`

Use snake case for variable naming, and try to keep your code clean and portable.
Use spaces instead of tabs, most IDEs will convert tabs to spaces automatically. 

### Functions 

Functions should be small and portable. 

```
def function:
    # Do stuff
```

### If Statements

```
if something == true:
    # do stuff
```

## Other_Languages

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

