# phlawd db maker
This is from the PHLAWD project and the NCBI GenBank SQLite database created was taken out so that it could be used for other projects.

## Dependencies
This requires that you have wget. For linux that is simple. For mac, you will need to `brew install wget`. You will also need cmake and sqlite3 libraries and hopefully that is it. You can install cmake with `brew install cmake` or you can download the GUI from the cmake website.

## Installation
Installation should be relatively simple. Then you just run `cmake .` in the phlawd_db_maker directory. This should make the necessary files. Then you run `make` and it should compile and make the `phlawd_db_maker` executable.

## Usage
All you need to do to run this is pick the division you want (so `pln` for example) and then run
`./phlawd_db_maker pln ~/Desktop/pln.db`
where the second argument is the location.