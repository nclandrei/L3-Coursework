# Name: Andrei-Mihai Nicolae
# GUID: 2147392n
# Date: 08/11/2015


func bool invert (bool b):
    bool x = false
    switch b:
        case true:
            x = false
        default:
            x = true
    .
    return x
.

