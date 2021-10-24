# Contributing

Thank you for taking the time to help us develop Hydra!

This document has set guidelines on contributing to Hydra (Echo/Inferno/BOB). If anyone has any changes to this document, please open a Pull Request.

## Table of Contents
[Styleguide](#Styleguide)
- [Git Commit Messages](#git-commit-messages)

## Styleguide
To keep things in an organized fashion, we have some rules on how we would like certain things to be formatted.
### Coding Style
Please read Documentation/CodingStyle.md
### Git Commit Messages
When commiting to Echo, your commits must have good clarity in what they implement/fix/enhance. All commits should start with what section of Hydra you have changed.
<br>
Ex: `BOB: Added ....` or `LibC: Fixed ...`
<br>
We would like for all big changes to be seperated into pieces so that if a build fails, we know which specific change made it occur. So, if your commit has many changes to the point where it's hard getting into detail what changes have occured, Try to seperate your commits in pieces.
