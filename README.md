# Programming II - HW3 | Simple File Archiver

-------

## Usage:

    ./project3

    
|Mode|Input|output stdout (success)|output stdout (failure)|
|-------|--------|-------------|-------------|
|open|o `<dbname>`| |Error opening `<dbname>` **or**  Invalid db file `<dbname>`|
|import|i `<fname>` `<name>`| |File `<fname>` not found **or** Object `<name>` already in db. **or** No open db file.|
|find|f `<fname>`|## `<object name>` ... `<object name>`|No open db file.|
|export|e `<name>` `<fname>`| |Object `<name>` not in db. **or** Cannot open file `<fname>`. **or** No open db file.|
|delete|d `<name>`| |Object `<name>` not in db. **or** No open db file.|
|close|c| |No open db file.|
|quit|q| | |

### Function description:

-  **o(open) `<dbname>`**: Open and check database file. The user gives the name of the file dbname
in which the database is (will) be stored. If a base file is already open, it is closed before the
new one is opened. If the file does not exist, it is created and initialized appropriately. If it exists
but is not properly structured (no expected type meta-info - see below), then an error message
is printed. If the file cannot be opened/created, then an error message is printed.

- **i(mport) `<fname>` `<objname>`**: Import object. The user gives the name of the file fname that
contains the object he wants to import into the database, and the name objname with which he
wants to save the object in the database. If the file fname provided by the user does not exist,
an error message is printed. If an object with the same name name already exists in the base,
then an error message is printed.

- **f(ind) `<objname>`**: Search for objects. The user gives the objname of the object they are looking
for. The program searches for all objects in the database with names that contain (anywhere in
them) the name given by the user, and prints their full names. If the user enters then the "*" 
program prints the names of all objects present in the database.

- **e(xport) `<objname>` `<fname>`**: Export object. The user gives the name of the object objname to
export and the name of the file fname in which to save it. The program locates the object in the
database and copies its contents to the file specified by the user. If the object does not exist in
the base, an error message is printed. If the file specified by the user does not exist, it is
created by the program. If the file cannot be created or already exists, an error message is
printed. In any case, the object remains in the base.

- **d(elete) `<objname>`**: Delete object. The user gives the name of the object he wants to delete
from the database. The program finds the object, deletes it, and the base file is truncated
accordingly. If the object does not exist, an error message is printed

- **c(lose)**: Close base file.

- **q(uit)**: Quit. The program terminates after first closing the base file (if the user has not already
closed the base file via close).

-------

## User Manual

**Compilation**: make project3

**Testing**: make run

**Clean**: make clean

-------------
