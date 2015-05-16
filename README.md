# libproject
Portable project management library
## Motivation
At the current moment, there are so much IDE's and advanced editors.
Most of them has GUI. 
Graphical interface is a good protocol, but most of implementations are too incompatible.
So our project is a step to separate protocol and it's rendering.
One of the best C++ IDEs - QtCreator, already have pretty abstract and extensible protocol, for project management.
But, it's hard depended on Qt. So we decided, to implement our own implementation, without huge runtime depencies, but with keep in mind qtc's interfaces.

## Purposes
Initially it's just an educational project for @drewpts and others.

Main target which we strave for - it's obtain portable, extensible and easy to use library, for managing source code projects.

## Installation
Download latest version of [gmock](https://code.google.com/p/googlemock/downloads/detail?name=gmock-1.7.0.zip&can=2&q=).
Extract it at one level with downloaded project source folder, to *gmock* directory.
Then use code below, to build project:
```
mkdir libproject-build && cd libproject-build
cmake ../libproject
make
sudo make install
```
Also available targets:
* documentation
* coverage (linux only, gcovr and gcc required)
* test

To run tests, follow steps below:
```
cd <build-directory>/tests
./libproject_test
```

## What is ready
* Build system skeleton
* Some mockup implementation for API(json based)
* qtc(qt-creator) [plugin](https://github.com/drewpts/LPROJ-main/tree/develop/samples/frontend/qtc-plugin), which integrates API usage in Qt creator
* Draft of API(it's fully unstable, and will mutate)
* Some documentation's [draft](https://github.com/drewpts/LPROJ-main/wiki/Index)

## Roadmap
* [Feature fullness](http://fake.org)
* Plugin's system
* C, and Python bindings by swig or may be manual
* Document whatever we can
