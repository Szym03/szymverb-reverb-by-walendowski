# JUCE Console Project Template

This is a minimal starting template for a JUCE-based console application (an application that runs in the terminal, rather than using a GUI).

## Installing JUCE

To build this project, it will need to be able to find the JUCE library. There are many ways to do this, but one easy way is to put JUCE in the parent directory above this project, so that multiple projects can reference the same JUCE installation.

You will only need to do this once per JUCE installation, which can be shared between multiple projects.

```bash
# navigate to parent directory (assuming you're starting from within
# JUCEProjectTemplate)
$ cd ..
# retrieve JUCE from GitHub
$ git clone https://github.com/juce-framework/JUCE.git
```

## Setting Up the Project Build Directory

CMake needs to set up a build directory that it will use as a workspace to turn your source code into an executable binary. Note that you do _not_ want to include this directory in your git repository. To avoid this, add a line with just "build" to a file called `.gitignore` at the root of your repository.

You will need to do this once per project, not every time you build.

```bash
# navigate back to the project directory
$ cd JUCEProjectTemplate
# this will create a directory called "build"
$ cmake -B build
```

## Building

Every time you make a change to your source code, you will need to rebuild your program before you can run it.

To build this project, run the following commands:

```bash
# build your project, using the "build" directory you created before
# the `-j` option tells it to use all your computers cores in parallel,
# which is not necessary but will speed up builds of larger projects.
$ cmake --build build -j
```
