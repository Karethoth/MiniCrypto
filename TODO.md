# TODO
## Priority
- Create menu for different workspaces / node_contexts
- Make saving and loading workspaces possible
- All challenges so far
  - Make saved workspaces for each of them
- New node types needed
  - Duplicator node if connecting data out pins to multiple inputs is not possible otherwise
  - Float/Number input and display
  - File I/O
    - Extra for automatic refresh when the file is modified
- Modifications to old nodes
  - Add more data transform options
- Figure out something for batches of data

## Bugs
- Data out -pins ought to be able to connect to multiple pins
- When a link is destroyed, destroy the event piping between the relevant pins
    - Testable with connecting text input to text display and breaking the link. The text display should not update when the text input is changed.
