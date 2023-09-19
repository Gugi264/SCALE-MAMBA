#!/bin/bash

# List of database files
databases=("mydb1.db" "mydb2.db" "mydb3.db" "mydb4.db" "mydb5.db" "mydb6.db")

# Loop through each database file and run the sqlite3 command
for db in "${databases[@]}"
do
	sqlite3 "$db" "SELECT id
	FROM (
	    SELECT id, LAG(id, 1) OVER (ORDER BY id) AS prev_id
	        FROM triples
		) AS t1
		WHERE t1.prev_id IS NOT NULL AND t1.id != t1.prev_id + 1
		ORDER BY id
		LIMIT 1;
		" &
done
