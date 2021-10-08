# Contributing

Thank you for taking the time to help us develop Hydra!

This document has set guidelines on contributing to Hydra (Echo/Inferno/BOB). If anyone has any changes to this document, please open a Pull Request.

## Table of Contents
[Styleguide](#Styleguide)
- [Git Commit Messages](#git-commit-messages)

## Styleguide
To keep things in an organized fashion, we have some rules on how we would like certain things to be formatted.
### Coding Style
The number one thing important to us when formatting your code is try to minimize the amount of lines as much as possible. We want to make sure that we don't use unneccesary data space in the file. When doing this, make sure you minimize the lines but try to make sure that everything still can seperated into parts for clarity in the code. We would also appreciate adding comments to parts of the code that maybe you have written so it can help us MAINTAINERS understand what a certain part of your code does.
### Git Commit Messages
When commiting to Echo, your commits must have good clarity in what they implement/fix/enhance. All commits should start with what section of Hydra you have changed.
<br>
Ex: `BOB: Added ....` or `LibC: Fixed ...`
<br>
We would like for all big changes to be seperated into pieces so that if a build fails, we know which specific change made it occur. So, if your commit has many changes to the point where it's hard getting into detail what changes have occured, Try to seperate your commits in pieces.
