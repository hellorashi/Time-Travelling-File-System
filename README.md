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
- Tiebreaker : In case two files have the same snapshot_timestamp/total_versions, the trees are output according to who is lexicographically smaller in both the two system wide analytics.
