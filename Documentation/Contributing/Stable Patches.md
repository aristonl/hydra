# Stable Patches
To get patches into the stable tree, they must follow these rules:
<br>
 - It must be obviously correct and tested
 - It cannot be bigger than 100 lines, with context.
 - It must only fix one thing.
 - It must fix a real bug that bothers people (not, "This could
   be a problem...")
 - It cannot contain any "trivial" fixes in it (spelling
   changes, whitespace cleanups, etc).
 - It must follow the 'Submitting Patches' rules.
 - The commit must already exist in the N11 tree (main/upstream).

## Sending the Patch

### Option 1
To have the patch automatically included in the stable tree
after being applied in upstream, add the tag
```
Cc: aristonl@n11.dev
```
in the sign-off area. Once the patch is merged it will be
applied to the stable tree without anything else needing to be
done by the author or system maintainer.

### Option 2
After the patch has been merged upstream, send an email to
aristonl@n11.dev containing the patch, commit ID, a reason why
it should be applied, and what kernel version it applies to.
<br>
The upstream commit ID must be specified with a separate line
above the commit text/sign-off area, like this:
```
[ Upstream commit <sha1> ]
```
Additionally, some patches submitted via Option 1 may have patch
prerequisites which can be cherry-picked. This can be specified
in the following format in the sign-off area:
```
Cc: <aristonl@n11.dev> # 0.1.0: ccad068: Meta: Renaming Echo to Hydra pt. 1
Cc: <aristonl@n11.dev> # 0.1.0
Signed-off-by: Ariston Lorenzo <4tl0renz0@gmail.com>
```
This translates to:
```
git cherry-pick ccad068
git cherry-pick <this commit>
```
<br>
Following the submission:
 - The sender will recieve an Acked-by when the patch has been
   accepted into the queue, or a Naked-by if the patch is
   rejected.
 - Once accepted, the patch will be added to the stable queue,
   for further review.
