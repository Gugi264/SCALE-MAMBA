#!/bin/bash

# List of database files
databases=("mydb1.db" "mydb2.db" "mydb3.db" "mydb4.db" "mydb5.db" "mydb6.db")
id=$1
# Loop through each database file and run the sqlite3 command
for db in "${databases[@]}"
do
	sqlite3 "$db" "DELETE FROM triples WHERE id>$1;" &
done
