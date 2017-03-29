# phlawd db maker
This is from the PHLAWD project and the NCBI GenBank SQLite database created was taken out so that it could be used for other projects.

## Dependencies
This requires that you have wget. For linux that is simple. For mac, you will need to `brew install wget`. You will also need cmake and sqlite3 libraries and hopefully that is it. You can install cmake with `brew install cmake` or you can download the GUI from the cmake website.

## Installation
Installation should be relatively simple. Then you just run `cmake .` in the phlawd_db_maker directory. This should make the necessary files. Then you run `make` and it should compile and make the `phlawd_db_maker` executable.

If you run into problems, it make be the sqlitewrapper. To fix that, just go into the deps/sqlitewrapper directory and type `make clean` and then `make`. Then copy the `libsqlitewrapper.a` into the relevant `deps` directory (so `mac` if you are running a mac and `linux` if you are running a linux). 

## Usage
All you need to do to run this is pick the division you want (so `pln` for example) and then run
`./phlawd_db_maker pln ~/Desktop/pln.db`
where the second argument is the location.

## Database structure
In case you would like to use this for your own tasks, here are the SQLite commands that are called to give you an idea of the tables and indices
```
create table taxonomy (id INTEGER PRIMARY KEY,ncbi_id INTEGER,name VARCHAR(255),name_class VARCHAR(32),node_rank VARCHAR(32),parent_ncbi_id INTEGER,edited_name VARCHAR(255),left_value INTEGER,right_value INTEGER);" );
CREATE INDEX taxonomy_left_value on taxonomy(left_value);
CREATE INDEX taxonomy_name on taxonomy(name);
CREATE INDEX taxonomy_ncbi_id on taxonomy(ncbi_id);
CREATE INDEX taxonomy_parent_ncbi_id on taxonomy(parent_ncbi_id);
CREATE INDEX taxonomy_right_value on taxonomy(right_value);
CREATE INDEX taxonomy_edited_name on taxonomy(edited_name);
create table sequence (id INTEGER PRIMARY KEY,ncbi_id INTEGER,accession_id VARCHAR(128),identifier VARCHAR(40),description TEXT,seq LONGTEXT);
CREATE INDEX sequence_ncbi_id on sequence(ncbi_id);
CREATE INDEX sequence_accession_id on sequence(accession_id);
CREATE INDEX sequence_identifier on sequence(identifier);
create table information (id INTEGER PRIMARY KEY, name VARCHAR(128), value VARCHAR(128));
```

Primarily, there are three tables, `taxonomy`, `sequence`, and `information`. The `taxonomy` table has the following elements:
- id
- ncbi_id (of course, doesn't have to be ncbi but this will be the unique id that is referenced elsewhere)
- name
- name\_class 
- node\_rank
- parent\_ncbi\_id (parent unique id)
- edited\_name (without special characters)
- left\_value (for recursive calls)
- right\_value (for recursive calls)

The `sequence` table has the following elements:
- id
- ncbi\_id (key to `taxonomy` table)
- accession_\id (can be used for a number of things, but used by NCBI)
- identifier (often GI number)
- description
- seq (the sequence)

_Caution:_ This will download many large files from NCBI in order to make the database. They will be deleted but they need to be downloaded first (so don't do this in your Dropbox DIR).
