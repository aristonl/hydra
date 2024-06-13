Creating Pull Requests
======================
This document details how to create and submit pull requests to other maintainers.

Creating a Branch
-----------------
You first need to have all the changes you wish to include in the pull request on a
separate branch. This branch will usually be based off of the tree you intend to send
the pull request to.

Tagging
-------
In order to create the pull request, you need to tag the branch. It is recommended that
the tag name is meaningful, in a way that it's easy for you and other to understand. The
recommended way of doing this is to include the name of the subsystem of origin and the
targeted release version. For example, a pull request with miscellaneous stuff for lib,
targeted at v1.0 would be named as ``lib-misc-1.0``. If such tag would be produced from
a branch named ``lib-misc-next``, you would be using the following command::

        git tag lib-misc-1.0 lib-misc-next

that will create the tag based on the last commit in the branch.

When you run the above command ``git`` will drop you into an editor and ask
you to describe the tag.  In this case, you are describing a pull request,
so outline what is contained here, why it should be merged, and what, if
any, testing has been done.  All of this information will end up in the tag
itself, and then in the merge commit that the maintainer makes if/when they
merge the pull request. So write it up well, as it will be in the kernel
tree for forever.


The tag message format is just like a git commit id.  One line at the top
for a "summary subject" and be sure to sign-off at the bottom.

Now that you have a local signed tag, you need to push it up to where it
can be retrieved::

	git push origin lib-misc-1.0

Creating a Pull Request
-----------------------
Now all you have to do is to create the pull request using ``git request-pull`` like
so::

        git request-pull master git@github.com:gingerwitch64/hydra-tcm.git lib-misc-1.0

We heavily recommend the use of ``ssh`` as it is used as the main protocol for
``n11.dev`` but it is not required.

A pull request is submitted in the same way as an ordinary patch. Send as inline email 
to the maintainer and CC the mailing list. Pull requests to us typically have a 
subject line something like::

	[GIT PULL] <subsystem> changes for v1.0

Merging Pull Requests
---------------------
Once the maintainer recieves the pull request, they can run::

  git pull git@github.com:gingerwitch64/hydra-tcm.git tags/lib-misc-1.0

Which will (in most cases) create a merge commit or fast-forward depending on if the
commits share the same parent commit.
