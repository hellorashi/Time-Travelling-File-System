# Time-Travelling File System

A **version control system** built in C++ from scratch. 

It lets you:
- Create files
- Insert / update content
- Take snapshots (immutable versions)
- Rollback to older versions
- View history
- See the most recent files or the biggest version trees

---

## Getting Started

### 1. Compile the Code
Open terminal and type the shell script :

```bash
chmod +x run.sh

```

This makes it executable.

### 2. Run the Program

```bash
./run.sh

```

The following line will show up : "Compilation successful!"

You will now be inside the program prompt where you can type commands.

Tested on macOS

---

##  Tree
- Each file is represented as a tree.
- name : file name
- root : pointer to the root
- active_version : pointer to the active_version
- updated_timestamp : stores the updated timestamp
- version_map : custom map to store the versions by their unique version ID
- biggest_tree_index, recent_files_index : to keep track of the file's positions in the heaps 
- total_versions : to keep track of total number of versions


##  TreeNode
- int version_id : each node has its unique ID
- content
- message : we can insert a message when we snapshot a node.
- created_timestamp : time of creation
- snapshot_timestamp : time of snapshot
- parent : pointer to the parent node
- children : collection of all the children of the node
## Commands

### Core File Operations
| Command | Example | What it Does |
|---------|---------|--------------|
| `CREATE` | `CREATE myfile` | Create a new file with version ID 0, outputs "File myfile is created" |
| `READ` | `READ myfile` | Show content of the current version |
| `INSERT` | `INSERT myfile Hello World` | Append text to the file, outputs "Inserted Hello World into myfile" |
| `UPDATE` | `UPDATE myfile Final Version` | Replace entire content, outputs "Updated myfile" |
| `SNAPSHOT` | `SNAPSHOT myfile "First Draft"` | Save current version as immutable, outputs "Snapshotted!" |
| `ROLLBACK` | `ROLLBACK myfile` or `ROLLBACK myfile 5`| Go back to a previous version or parent |
| `HISTORY` | `HISTORY myfile` | Show all snapshots from root to current |

### System-Wide Analytics
| Command | Example | What it Does |
|---------|---------|--------------|
| `RECENT_FILES` | `RECENT_FILES 3` | Show top N most recently modified files |
| `BIGGEST_TREES` | `BIGGEST_TREES 2` | Show top N files with most versions |

### Exiting the Program
```bash
EXIT
```
Stops the program.

---

## Example Session
```
CREATE report
INSERT report This
INSERT report is
INSERT report draft1
SNAPSHOT report "Saved first draft"
UPDATE report FinalVersion
SNAPSHOT report "Final version saved"
READ report
HISTORY report
BIGGEST_TREES 1
RECENT_FILES 1
EXIT
```

**Sample Output:**
```
File report is created
Inserted This into report
Inserted is into report
Inserted draft1 into report
Snapshotted!
Updated report
Snapshotted!
FinalVersion
The version ID is : 0 | Snapshot timestamp is : .. | Creation timestamp is : ..| Message is : initial snapshot :)
The version ID is : 1 | Snapshot timestamp is : .. | Creation timestamp is : ..| Message is : Saved first draft
The version ID is : 2 | Snapshot timestamp is : .. | Creation timestamp is : ..| Message is : Final version saved
report
byeee!
```

---

##  Notes
- All data structures (**Tree, Heap, HashMap**) are implemented manually.
- For a file, Version IDs are unique per version starting from 0.
- Snapshots are immutable; non-snapshots can still be edited.
- the root has the snapshot message :"initial snapshot :)" and the content is : "".
- adding a space before each updation if the previous content isn't empty.
- Note that in the history function, I am just using the version ID because a node needs to be snapshotted in order to have a child. We are just printing the nodes from the root to the active version so it is necessary that all the nodes which come strictly on this specific path must be snapshotted, so no need to compare snapshot time. Also note that this is not true if the nodes are not in a single path between the root and the active version ie a node in an upper level can be unsnapshotted whereas a node in the lower level is snapshotted. But in this case we only have to output the nodes which come in the way. It will print their verson ID, snapshot timestamp, creation timestamp and snapshot message. So basically it will print all the nodes starting from the root to the active_version which fall in this path ie dfs traversal.
- Hashing for the map mymap which is used to store filenames with their Tree* is done by multiplying the ascii values of filename characters by powers of 23, I chose 23 because it is a prime number so probabilty of strings having similar index is less. The vector used for hashing has size = 1000, we can also increase this size or use a dynamic vector which would double when it gets filled up to a certain percentage. For small-medium scaled inputs, our idea will do. 
- We have made two heaps : biggest_trees and recent_files. They consist of all the classical heap fucntions like heapifyup, heapifydown, insert, popfirst etc. along with a few other functions.
- Tiebreaker : In case two files have the same snapshot_timestamp/total_versions, the trees are output according to who is lexicographically smaller in both the two system wide analytics.
- Used stringstream for input parsing

##  Error handling

Program handles invalid commands gracefully
- When the user tries to create a file already created : "File <filename> already exists!"
- When user tries to do an operation on a file that doesnt exist : "File doesn't exist!".
- When user tries to snapshot an already snapshotted version : "Already a snapshot!".
- If no argument is provided along the ROLLBACK filename then it rolls back to the active version's parent and prints "Rolled back to the parent!".
- When user tries to rollback to the root's parent : "Rollback not possible, already at root!".
- When user tries to rollback to an index out of range : "Index out of range".
- When user calls BIGGEST_TREES or RECENT_FILES with greater number of files than those present :  "Number is greater than the number of files, here are all the files though!" and then all the files in the correct order will get printed. 
- When user calls BIGGEST_TREES or RECENT_FILES with <=0 argument value : "Invalid number".
- When user calls BIGGEST_TREES or RECENT_FILES without any argument : "Sorry, positive integer argument is required!" 
- When user calls BIGGEST_TREES or RECENT_FILES with anything other than strictly digits : "Sorry, only positive integer arguments allowed!"
- When user inputs an invalid command : "Unknown command: " <command>.

##  Complexity Analysis
- CREATE - O(log n) where n is number of files
- INSERT, UPDATE - O(log n) because biggest_trees_index needs updation in case of new node formation and recent_files_index due to change in modification time(excluding string concatenation).
- HISTORY - O(h) where h is number of nodes on the path between root and active_version.
- READ, ROLLBACK, SNAPSHOT- O(1)
- RECENT_FILES - O(k log n) where k is arg
- BIGGEST_TREES - O(k log n) where k is arg

