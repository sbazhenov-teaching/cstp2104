# CSTP 2104 individual project
## Primary focus
The primary focus of the project must be Windows interactive application programming.
#### Windows platform
Must be for Windows platform, using [Win32 native API](https://docs.microsoft.com/en-us/windows/win32/api/).
#### Interactive
Must interact with the user taking input in some form and provide output to the screen (have graphical user interface), and do so responsively, i.e. without noticeable lag. Any time-consuming operations must be running in background not ruining responsiveness.\
Some ideas for GUI: a combination of standard and creative custom controls, or Direct2D graphics. Some keywords: windows messages, mouse/keyboard events, device contexts, windows, brushes, child windows, custom controls etc.
#### Primary language - C++
C++ remains primary language used for native application development and allows easy interaction with various Windows APIs.
#### Multi-project solution
It'll be useful to add some static or dynamic libraries. Do it in a meaningful way. Think how your library could be used by somebody else.
#### Multithreading
Think how your app could use [separate threads](https://docs.microsoft.com/en-us/windows/win32/procthread/creating-threads) in the project. It's important to practice this topic as this will be required for the final exam.
## Complexity
As there are no assignments in this course, projects have to be reasonably complex. Complexity doesn't necessarily have to be around just one area, it could be a combination of several smaller challenges.
Some additional areas of interest that could contribute to complexity:
* [Low level networking](https://docs.microsoft.com/en-us/windows/win32/winsock/windows-sockets-start-page-2);
* Using [Raw Input](https://docs.microsoft.com/en-us/windows/win32/inputdev/using-raw-input), relevant if making something like a game;
* Using resources packaged with the app;
* Using registry and filesystem;
* C++/C# interoperability;
* DLLs.
## Coding style
Coding style must be consistent. Choose one and follow it. Pay close attention to:
* Tabs/spaces. Decide on some indentation rules, for example use the same number of spaces (e.g. 2 or 4) or a tab everywhere, and follow that consistently. Don't leave dangling whitespace;
* Alignment;
* Vertical spacing - avoid excessive use of empty lines. Use them between functions, for example;
* Limit variables' scope;
* Avoid code duplication - use functions etc.;
* Naming convention. Decide on some special naming rules. For example:
  * How classes' names will visibly differ from variables and functions?
  * How private class members will visibly differ from public ones?
  * How constants will visibly differ from variables?
* Brackets - same placement style everywhere;
* Line length - limit at 140 symbols.
## Other requirements
* Bug-free - working as intended, including edge cases;
* Reasonable algorithmic complexity;
* Use object oriented principles where applicable. In particular, pay attention to encapsulation.
* Meaningful naming. Must be clear from the name what any function/variable/class/namespace/file is for. Exception - counter variables such as `i`, `j` if they are incremented/decremented.
* Functions must be named after an action, e.g. `getValue()` or `performThisReallyComplexAction()`.
* Declarations in headers, definitions in source files (cpp), except templates;
* Complete visual studio solution with all source and project files, as well as a release .exe, must be submitted.