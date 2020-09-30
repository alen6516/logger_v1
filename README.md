# my logger version 1
using one global logger

## 2 ways to print log
* with an independent log thread
    * caller create a log_node, and append it to the list (producer)
    * log thread consume the list
* without an independent log thread
    * caller write to file and stdout by itself
 
## module name
e.g.
    source file: sflow_rcv.c
    then extract "sflow" as the module name
