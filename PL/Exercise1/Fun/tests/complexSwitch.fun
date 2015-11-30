# Name: Andrei-Mihai Nicolae
# GUID: 2147392n
# Date: 29/11/2015

func int testSwitch (int n):
    int r = 0
    int s = 0
    switch n:
        case 1:
            r = 2
            s = 4
        case 2: 
            r = 5
        case 3:
	    r = 4
        case 4:
            r = 6
            s = 2
        default:
            s = 1
    .
    write(s)
    return r
.

proc main ():
    int n = testSwitch (4)
    write (n)
.
