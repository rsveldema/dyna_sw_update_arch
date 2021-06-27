# Infrastructure for zero-downtime firmware update.

## The Traditional solution

The problem we are trying to solve in here is to avoid downtime when switching from one firmware version to the next.
Traditionly, firmware is split in an 'current' space and a 'new' space.
Lets call then 'A' and 'B' and we have a pointer 'P' pointing to the current space.

Lets assume we are currently running the code in the 'A' space.
When doing a firmware update, the new code is placed in the 'B' space.
After finishing the installation in the 'B' space the 'A' and 'B' spaces are swapped by setting the 'P' pointer to 'B' instead of 'A' and the system is rebooted.
When it comes up again after reboot, the system will execute the new firmware.

On the next firmware update the A-B spaces are swapped again and P reset back to 'A', ad-infinitum.

During the reboot the system is unavailable for a short while.
For real-time systems such reboot times are unacceptable.
For example, a car or pace maker should not reboot during operation as otherwise the system will stop responding/acting during that time
causing life endangerment.

## Our solution.

We keep the code running and add the changed code only the the reserved 'B' space.
Only single function pointers to 'A' space are changed to point to 'B' space.
The pointer 'P' is never changed and the system is never rebooted!

## Problem 1: Structure layout compatibility between versions

When crafting a firmware update, we'll end up with a package of new functions to be deployed on the 
device. We, however, must make sure that code can continue to run in the presence of changed data layouts.

Consider, for example:
```C
    struct Version1 { int x, y; } // old code used this
    struct Version2 { int x, z, y; } // new code uses this
```

What happens if something from Version1 is cast to Version2?
```C
    Version1 a {1, 2};
    Version2 *b = (Version2*) &a;
    assert(a.x == b->x); // OK, overlaps in memory
    assert(a.y == b->y); // FAIL, does not overlap in memory
```

## Solution: Indirection





