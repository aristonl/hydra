# Email Clients for Hydra/Inferno development
This document contains info on what recommended email clients you should use
for development of Hydra.

## General Info
Since Hydra/Inferno is mainly hosted on a custom Git server, we can't use things
available on GitHub such as pull requests and issues so especially for patching
we use email to send patches that will be applied. We strongly enforce use of
``text/plain`` in your email since it may affect sending your patches and 
developers using different clients might have problems dealing with HTML
clients. https://useplaintext.email/ may be useful in how to configure your
preferred email client.

Patches sent by your preferred email client should be untouched by the client
For example, they should not modify tabs or spaces, should not add extra
unnecessary lines, etc..

## Sending Patches

### Git
The recommended way to send patches is through ``git send-email``. You can read
on how to do this in 4. Submitting Patches.rst.

### Mutt
Mutt is a command-line based email client built for Unix. Using mutt for
handling patches from the mailing list makes life easier due to the shear
hackibility of the program. This is also the recommended client used for
applying patches from the mailing list straight to your local git 
repository.

Try to use the successor of Mutt, NeoMutt.

First things first, connect mutt (`~/.muttrc` or `~/.neomuttrc`) with your Email 
Server (using Gmail as an example):
```
set from = "username@gmail.com"
set realname = "Real Name"

set imap_user = "username@gmail.com"
set imap_pass = "password"

set smtp_url = "smtps://username@gmail.com:2525"
set smtp_pass = "password"

set folder = "imaps://imap.gmail.com"
set spoolfile = "+INBOX"
set record="+[Gmail]/Sent Mail"
set postponed="+[Gmail]/Drafts"

set header_cache = "~/.mutt/cache/headers"
set message_cachedir = "~/.mutt/cache/bodies"
set certificate_file = "~/.mutt/certificates"
```

Then make sure to set your editor to your preferred text editor (Neovim as
example):
```
$ set editor = "nvim"
```

Then you can send your patch inline through ``git format-patch`` and then
pass it to mutt:
```
$ mutt -H 0001-some-bug-fix.patch
```

## Applying Patches

### Mutt / Git
All you have to do for this is to add a macro to your ~/.muttrc.
```
macro index ,l '| git am -s'\n
```
Once you review your commit, go back to the main screen, highlight the patch
and then type ,l.
