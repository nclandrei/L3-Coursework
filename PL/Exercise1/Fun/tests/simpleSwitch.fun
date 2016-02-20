# Name: Andrei-Mihai Nicolae
# GUID: 2147392n
# Date: 29/11/2015

func int test (bool n):
     int r = 0
     int s = 0
     switch n:
        case true:
            r = 2
            s = 4
        case false: 
            r = 5
        default :
	    r = 4 
     .
     write(s)
     return r
.

proc main ():
.
