# Making Changes

## Cloning
Before you start making changes, make sure you know what tree of the
engine you're code might be a best fit for.

```
git clone ssh://git.n11.dev/hydra
```
>    The main source tree. This tree is usually where all larger
     implementations, bug fixes, etc. are done. All other trees are
     eventually merged into this tree. All major releases are sourced
     from this tree. Maintained by aristonl/firelscar.

## Branching
When making changes, try to make your changes in a separate local branch since 
any commits you make won't have the same commit ID as the patch you will send 
to the mailing list. Once the maintainer applies your patch, you can then
delete the local branch.

## Coding Style
Hydra/Inferno has a looser coding style than most other projects.
Make sure to check ../CodingStyle for more information. Try to 
follow the style guidelines as much as you can.

## Commiting
When making commit names, try to make sure they are descriptive in what
your commit changes. Make sure to also specify what subsection of the
engine you have made changes to. Last but not least, sign off on your
commit with your name and email address. This improves tracking of who
made/accepted what and follows the Developer's Certificate of Origin.
A shortened version of the Developer's Certificate of Origin is as
follows:

By making a contribution to this project, I certify that:
1. This commit was created in whole by me or in part by me with help from co-authors and I submit this code under the license specified in this file; or...
2. This commit is based upon previous work that is covered under an open source license and I have the right under that license to commit this work with modifications created in whole by me or in part with co-authors, under the same license (unless I am permitted to do otherwise), as in the file; or
3. This code was provided directly to me by some other person who certified (a), (b) or (c) and I have not modified it.
4. I understand that this project and commit is public and a record of the commit, with included personal information that I submit with it, remains online indefinitely.

An example of a commit would be:
```
    mathlib: Added a new operator to ivec4.
    
    Signed-off-by: Ariston Lorenzo <4tl0renz0@gmail.com>
```