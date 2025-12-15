
Back:
  - state: `{
	  cube: int[4][10][10][10] (MAYBE flattened),
	  cursor: int[3],
    funcs (?)
    bindings (?)
    log: 
	}`
  On init, pass front, state pointer
  Sends code translations:
    - Value bindings
    - List of defined functions (?)
    - Operations (+,-,*,/,%, MAYBE bitwise ops too)
    - Calls
    - Conditions
    - Loops
  Saves a log of every operation in a reversible way (for time travel)
    - 


Front:
  Reads state via `getValue`
  Receives code translations from backend
