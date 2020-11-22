### Contraint: create a template listmap data container without std::iterators, vectors, maps, pairs, or lists.
### Description
Creates a cyclic listmap container, taking file or cin as input (written as '-' in the command line). Sort keys lexographically.

As always, no memory leaks.
### How to run
Example:
```
./keyvalue filename1.in filename2.in - filename3.in
```


### Commands by example
```
keyname = 5     # inserts a key-value pair w/ 'keyname' as the key, or modify it
keyname =       # deletes the key-value pair
= 5             # print all pairs with the value 5
=               # print all pairs
keyname         # print the pair with the keyname
#               # print the comment
```
