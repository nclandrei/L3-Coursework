# Name: Andrei-Mihai Nicolae
# GUID: 2147392n
# Date: 29/11/2015


func bool invert (int b):
    bool x = false
    switch b:
        case 1:
            x = false
        case 2:
	        x = true
	    case true:
	        x = false
	    case 1:
	        x = true
	    default:
            x = true
    .
    return x
.

proc main ():
.
