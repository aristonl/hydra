# Contributing to Hydra
When pushing code to Hydra, please be sure the change(s) you wish to make are in line with the project vision. If you are unsure about this, open an issue, or start a talk in the Discord, so we can discuss it. If you are already confident it's a good fit, you can proceed to open a Pull Request.

## Issue Policy

Unlike many other software projects, we are not focusing on getting the biggest and best community. Its target audience are developers who are developing Hydra or developers who are interested in Hydra. As such, we have limited interest in feature requests from non-community members.

That said, please do file any bugs you find, keeping the following in mind:

* One issue per bug. Putting multiple things in the same issue makes both discussion and completion unnecessarily complicated.
* No build issues (or other support requests). If the GitHub Actions CI build succeeds, the build problem is most likely on your side. 

## Feature Policy

Hydra is strongly user-focused. When building or proposing a new feature, you should imagine how a person would use it. Like a tree falling in the woods with nobody around, does a feature with no user impact really provide value?

Maybe your feature involves a GUI application, or maybe it's more suited to a CLI program. Either way, a useful exercise is to imagine what the user interface would be for your idea. Let's say you want to implement a fan speed monitor in the kernel because you recently read about how the related hardware interfaces work - rather than diving right into the kernel implementation, it would be good to imagine how that might be exposed to a user. You might want to build a simple taskbar widget, or maybe a command-line program, or even something as simple as a `/proc` interface, and fill it with mock data to begin with. Whatever your interface is, it should "fit" with the rest of the system, and it should provide some utility to a user.

Sometimes there is no obvious user-facing interface for a feature. In those cases you should still think about the impact that your feature or change will have on a user. If possible, you should devise a way to measure your feature - maybe it reduces memory consumption, maybe it results in fewer page faults, maybe it makes more efficient use of network resources. Whatever that metric is, try to capture it, and provide a summary in your proposal or pull request. If your numbers win, you have a good chance of getting your feature into the system. If your numbers don't win, you have valuable data that you can use to improve your feature.

If you can think of no user-facing interface for your feature, and no user impact at all, please do feel free to start a discussion about it. But please don't spend your precious time building a feature or implementing a change if there's no way for someone to use it and no way to quantify the overall improvement.
