Katherine Scott
kas2221@columbia.edu
HW5

1.
I did everything by the book for this program. The correlation window
size is 11 (still slow, but not much you can do) and the search
window is 22 pixels centered around the point. 

In areas where we can't get a good template match because of lack of 
texture the needle map isn't very reliable and points all over the
place.

To fix this I added a couple little tweaks. First, if the kernel has
a low variance in color I reject it and set the needle map to have no
length. I also reject kernel matches that are below a certain
threshold (i.e. the match is maximal, but not that good). This results
in a cleaner needle map where a lot of the "noise" needles are
removed. If you want to turn this feature off comment out line 1597
and 1623 and 1624. 
