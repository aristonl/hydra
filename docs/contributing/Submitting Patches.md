# Submitting Patches

Once you have created a patch, you can submit it to the main project
via patches. Read EmailClients.rst for more information.

## Formatting your patches
This step is optional, but recommended. This creates the .patch file
and can save it into a directory. This is done via git.
```
$ git format-patch -1 HEAD -o patches/
```

This patch creates a patch file from the first commit from HEAD (aka
latest commit at HEAD). The -o option specifies the directory to save
to (i.e. patches/).

If you need to make a revision of your patch based on replies you've recieved,
you can amend a previous commit with `git commit --amend` and append `-v2` to
the format patch command used prior.

### Setup your git config
An example of a git config file using Gmail as your SMTP.

```
[sendemail]
    smtpserver = smtp.gmail.com
    smtpport = 587
    smtpuser = your_username
    smtpPass = your_password
    smtpencryption = tls
```

## Send your patch to the mailing list
To send your patch to the mailing list there are three ways of doing
so. The first two is to use git send-email through already formatted
patches or to generate the patch without saving to a file and sending
it. The third way is to use another email client (ie. neomutt) and send
the patch via the email client. You can find how to do the latter in
EmailClients.rst. Make sure you know what maintainer you need to send
the message to before you send it. Check MAINTAINERS for more info.

### git send-email with formatted patches
If you've already formatted your patches and saved them to a directory
(ie. patches/), you can send the patches either as a single patch or as
a patch series.

To send a single patch, simply run:
```
$ git send-email --to="Some Maintainer <example@n11.dev>" --cc=hydra-inferno@googlegroups.com patches/0001-Add-a-new-function.patch
```
To send a patch series, run:
```
$ git send-email --to=hydra-inferno@googlegroups.com patches/
```
Once you send your patches, wait for a maintainer to review them. They
will sign off on your patch, apply it to their tree and you can delete
your change branch and git fetch to get the new commit.
