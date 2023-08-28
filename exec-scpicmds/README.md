# execScpiCmds

This tool allows to send SCPI (or ZSCPI, as the syntax in ZERA-specific) messages over TCP/IP. These commands are read from a file in which each line contains a message. Further it allows for adding specific commands to control the execution of the SCPI-messages, e.g. looping.

## Command Syntax
In this section the commands are described.

### VAR
**Syntax:**
```> VAR <VAR_NAME> <VAR_TYPE> <VALUE>```
**Arguments:**
* ```<VAR_NAME>``` Variable name which is a string of following characters (as regular expression): ```[0-9A-Za-z_]+```.
* ```<VAR_TYPE>``` One of type {```INT```, ```FLOAT```, ```BOOL```, ```STRING```}.
* ```<VALUE>``` Interpretable to the corresponding type of ```<VAR_NAME>```. For Boolean values only ```TRUE``` or ```FALSE``` is accepted. Strings need to be set in ```"``` and quotes in strings can be escaped using ```\"```.

**Description:**
The ```VAR``` command creates a new variable. All these variables are globally, so there's no scope for them. All variables found in the input file are parsed, independendly of their location (e.g. inside a ```LOOP``` command).

**Examples:**
```> VAR MYVAR1 INT 42```
```> VAR MYVAR2 INT 3.14159```
```> VAR MYVAR3 BOOL TRUE```
```> VAR MYVAR4 STRING "Test"```

### SET
**Syntax:**
```> SET <LEFT_VAR_NAME> <RIGHT_VALUE>```

**Arguments:**
* ```<LEFT_VAR_NAME>``` Existing variable which gets ```<RIGHT_VALUE>``` assigned to it.
* ```<RIGHT_VALUE>``` One of the following types.
  * ```<CONST_VALUE>``` A const value that is interpretable to the ```<LEFT_VAR_NAME>``` variables corresponding type. The same rules as for ```<VALUE>``` in the ```VAR``` command apply.
  * ```<VARIABLE>``` Existing variable of the same type as ```<LEFT_VAR_NAME>``` from which the value is assigned from.

**Description:**
Sets the variable to value. The value can be const or also a variable.

**Examples:**
```> SET MYVAR1 43```
```> SET MYVAR2 2.71828```
```> SET MYVAR3 FALSE```
```> SET MYVAR4 Test2```

### ADD
**Syntax:**
```> ADD <LEFT_VAR_NAME> <RIGHT_VALUE>```

**Arguments:**
* ```<LEFT_VAR_NAME>``` Existing variable which assign the value to.
* ```<RIGHT_VALUE>``` One of the following types.
  * ```<CONST_VALUE>``` A const value that is interpretable to the ```<LEFT_VAR_NAME>``` variables corresponding type. The same rules as for ```<VALUE>``` in the ```VAR``` command apply.
  * ```<VARIABLE>``` Existing variable of the same type as ```<LEFT_VAR_NAME>``` from which the value is assigned from.

**Description:**
Adds the variable to value. The value can be const or also a variable. Boolean variables are not allowed. Strings are concatenated.

**Examples:**
```
> SET MY_VAR_INT 42
> ADD MY_VAR_INT -1
# The following command outputs 41
> PRINT MY_VAR_INT
```
```
> SET MY_VAR_STRING "Hello"
> ADD MY_VAR_STRING " World!"
# The following command outputs "Hello World!"
> PRINT MY_VAR_STRING
```

### LOOP
**Syntax:**
```
> LOOP [<N_REP>]
...
> END
```

**Arguments:**
* ```<N_REP>``` Optional number of repetitions. A const value interpretable as unsigned integer or a variable holding such a value. Is this value is not specified, the loop is an endless loop.

**Description:**
Repeats its commands (between the ```LOOP``` and its corresponding ```END``` command) ```<N_REP>``` times. ```LOOP``` is a container (also as ```IF``` is) and therefore (container) commands can be nested into it.

**Example:**
```
> VAR REP_CNT INT 3
> LOOP REP_CNT
  *STB?
  > LOOP 2
    *IDN?
  > END
> END
```

### BREAK
**Syntax:**
```BREAK```

**Arguments:**
No arguments.

**Description:**
Breaks all execution recursively of all commands in the command stack up to the innermost ```LOOP``` command. It has the same behavior known from programming languages like C. It is only valid when inside a ```LOOP``` node.

**Example:**
```
> LOOP 10
  *STB?
  > BREAK
  # Following line is unreachable and will never get executed
  *IDN?
> END
```

### EXIT
**Syntax:**
```EXIT```

**Arguments:**
No arguments.

**Description:**
Breaks all execution recursively of all commands in the commans stack up to the very top. I.e. it stops all execution. It has the same behavior known from programming languages like C. It can get called from everywhere.

**Example:**
```
*STB?
> EXIT
# Following line is unreachable and will never get executed.
*IDN?
```

### PRINT
**Syntax:**
```PRINT <STR> [<STR> [...]]```

**Arguments:**
* ```<STR>```String or an existing variable. At least one string needs to be passed to this command. The same rules as for strings in ```<VALUE>``` in the ```VAR``` command apply.

**Description:**
Prints const strings and variables to the command line.

**Example:**
```
> VAR MYVAR INT 42
# Following line prints "MYVAR = 42"
> PRINT "MYVAR = " MYVAR
```

### IF
**Syntax:**
```
> IF <COND>
...
[> ELSE
...]
> END
```

**Arguments:**
* ```<COND>``` One of the following types:
  * ```<CONST_BOOL>``` Const boolean value, either```TRUE``` or ```FALSE```.
  * ```<BOOL_EXPR>``` Boolean expression which performs a comparison between two values/variables. It has the syntax ```<LEFT_VAR_NAME> {LT, GT, LE, GE, EQ, NE} <RIGHT_VALUE>```:
    * ```<LEFT_VAR_NAME>``` Existing variable which gets compared to ```<RIGHT_VALUE>```. Only variables of the same type can get compared.
    * ```{LT, GT, LE, GE, EQ, NE}``` Comparison rules between ```<LEFT_VAR_NAME>``` and ```<RIGHT_VALUE>```:
      * ```LT``` Lower than. Applies for ```INT``` and ```FLOAT```.
      * ```GT``` Greater than. Applies for ```INT``` and ```FLOAT```.
      * ```LE``` Less or equal. Applies for ```INT``` and ```FLOAT```.
      * ```GE``` Greater or equal. Applies for ```INT``` and ```FLOAT```.
      * ```EQ``` Equal. Applies for ```INT```, ```FLOAT```, ```BOOL``` and ```STRING```.
      * ```NE``` Not equal. Applies for ```INT```, ```FLOAT```, ```BOOL``` and ```STRING```.
    * ```<RIGHT_VALUE>``` Existing variable or a const value that is interpretable to the ```<LEFT_VAR_NAME>``` variables corresponding type. Gets compared to ```<LEFT_VAR_NAME>```.

**Description:**
The boolean value of ```<COND>``` gets evaluated first. If it evaluates to ```TRUE``` the commands in the ```IF``` block get executed, but not the ones in the ```ELSE``` block. If it evaluates to ```FALSE``` the commands in the ```ELSE``` block get executed, but not the ones in the ```IF``` block. The ```ELSE``` block is optional. It has the same behavior known from programming languages like C. ```IF``` is a container (also as ```LOOP``` is) and therefore (container) commands can be nested into it.

**Examples:**
```
> VAR CNT INT 5
> IF CNT GT 3
  *IDN?
> ELSE
  *STB?
> END
```
```
*IDN*
# The following IF FALSE comments out the command in its block (remember, VAR commands will always be evaluated)
> IF FALSE
  *STB?
> END
```
