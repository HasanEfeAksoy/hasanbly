# hasanbly
hasanbly basic scripting language
<br> 
<br>
<b>current version 1.4</b>
<br>
[version 0.0, made on 05.10.2022]
<br>
<br>
if you want developing this project you can compile like this:
<br>
<b><i><pre>g++ main.cpp -o hsnc -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread</pre></i></b>

<br>
<h1>Get Started</h1>
<br>
<b>You can get it with the command <pre>git clone https://github.com/HasanEfeAksoy/hasanbly.git</pre></b>
<br>
Or get it as zip. Download hsnc.exe and create your .hsn file. (main.hsn)
<br>
<br>
Run with <b><i><pre>hsnc main.hsn</pre></i></b> command.
<br>
(note: Example main.hsn file may be do not run on MacOS and Linux. Please delete file and create again.)
<br>
<br>

<b>
<p>Hasanbly is open source, not used parser basic scripting language.
<br>
Created with C++.
<br>
<h4>It is just <i>interprete</i> and run your codes easily.</h4></p>
</b>
  
<br>
<br>

<h1><b>Usage<b></h1>
<br>
<h3>Hello, World!</h3>
<br>
<p>First, you may want to print Hello, World! to the console 🙃</p>
<br>
<pre>
<p><i>
&lt&ltHello, World!;
<br>
EXIT;
</i></p>
</pre>

<br>
<br>
------------------------------------------------------
<h3>Commands</h3>
<br>
<ul>

<li>
  <h4>//</h4>
  <br>
  <p>Comment line. The // operator only makes one line completely uninterpretable. So it cannot be used on a used line. 
  <br>
  <br>
  <pre>
  <i>
  // This is comment line
  <br>
  // These lines will be ignored by the interpreter.
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i></i></pre>
  </p>
</li>

<br>

<li>
  <h4>EXIT</h4>
  <br>
  <p>Ends the program. At the end of the program, you have to put EXIT command otherwise the program does not run.
  <br>
  <br>
  <pre>
  <i>
  &ltBye;
  <br>
  EXIT;
  <br>
  &ltBye;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>Bye</i></pre>
  </p>
</li>

<br>

<li>
  <h4>DEF</h4>
  <br>
  <p>Create a new variable and assign the entered value to the value of the variable. You have to define int or string or double (int-str-dbl).
  <br>
  [Camel case variables are preferred]
  <br>
  <br>
  <pre>
  <i>
  DEF:str:name=empty;
  <br>
  &lt&lt$:str:name_;
  <br>
  DEF:int:number=15;
  <br>
  &lt&lt$:int:number_;
  <br>
  DEF:dbl:myDouble=4.5;
  <br>
  &lt&lt$:dbl:myDouble_;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>empty<br>15<br>4.500000</i></pre>
  </p>
</li>

<br>

<li>
  <h4>&lt</h4>
  <br>
  <p>Prints the written values to the console.
  <br>
  <br>
  <pre>
  <i>
  &ltHello;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>Hello</i></pre>
  </p>
</li>

<br>

<li>
  <h4>&lt&lt</h4>
  <br>
  <p>Prints the written values to the console with next line.
  <br>
  <br>
  <pre>
  <i>
  &lt&ltHello;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>
  output: <i>Hello</i>
  </pre>
  <br>
  </p>
</li>

<br>

<li>
  <h4>&gt</h4>
  <br>
  <p>Assigns the entered value to the value of the written variable.
  <br>
  <br>
  <pre>
  <i>
  DEF:str:text=noname;
  <br>
  // entered 'hasanbly'
  <br>
  &gt$:str:text_;
  <br>
  &lt&lt$:str:text_;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>hasanbly</i></pre>
  </p>
</li>

<br>

<li>
  <h4>M</h4>
  <br>
  <p>You can using math operators like = + - * / %, basic trigonometric processes ex.(M.SIN $:dbl:myNum1_ $:dbl:myNum2_;) [note : just for radians double variables], M.ABS, M.POW, M.SQRT.
  <br>
  But you have to using variables. You can convert type of variables with use M= command.
  <br>
  <br>
  <pre>
  <i>
  DEF:int:first=0;
  <br>
  DEF:int:second=1;
  <br>  
  // $:int:first_ = $:int:second_
  <br>
  M= $:int:first_ $:int:second_;
  <br>
  &lt&lt$:int:first_;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>1</i></pre>
  </p>
</li>

<br>

<li>
  <h4>GOTO</h4>
  <br>
  <p>Going to line. You can using with line number or line number variable value.
  <br>
  <br>
  <pre>
  <i>
  &ltHi; 
  <br>
  GOTO 1;
  <br>
  <br>
  // or
  <br>
  <br>
  //DEF:int:line:2;
  <br>
  //&ltHi;
  <br>
  //GOTO $:int:line_;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>HiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHi........</i></pre>
  </p>
</li>

<br>

<br>

<li>
  <h4>IF</h4>
  <br>
  <p>Like box office checkpoint. If true: can pass, else: go to a line.
  <br>
  You have to use == != &lt&lt &gt&gt &lt= &gt= and must use variable (not pure values) and use ELSE:&ltpure number&gt
  <br>
  IMPORTANT: you have to string-string or int-int or double-double when using == or !=. but if using &lt&lt &gt&gt &lt= &gt= you have to input parameters just int-int/double or double-double/int.
  <br>
  <br>
  <pre>
  <i>
  DEF:int:num1=0;
  <br>
  DEF:int:num2=0;
  <br>
  // if 0 == 0
  <br>
  // it returns true. so the interpreter does not go to line 8. it does not anything right now.
  <br>
  // but if you put != the interpreter goes to line 8.
  <br>
  IF== $:int:num1_ $:int:num2_ ELSE:8;
  <br>
  &nbsp;&nbsp;&nbsp;&nbsp;&lt&ltHello everyone;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>Hello everyone</i></pre>
  </p>
</li>

<br>

<li>
  <h4>INC</h4>
  <br>
  <p>Increase the variable. You can use for int and double.
  <br>
  <br>
  <pre>
  <i>
  DEF:int:number=0;
  <br>
  &lt&lt$:int:number_;
  <br>
  INC $:int:number_;
  <br>
  &lt&lt$:int:number_;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>0<br>1</i></pre>
  </p>
</li>

<br>

<li>
  <h4>DEC</h4>
  <br>
  <p>Decrease the variable. You can use for int and double.
  <br>
  <br>
  <pre>
  <i>
  DEF:int:number=1;
  <br>
  &lt&lt$:int:number_;
  <br>
  DEC $:int:number_;
  <br>
  &lt&lt$:int:number_;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>1<br>0</i></pre>
  </p>
</li>

<br>

<li>
  <h4>NULL</h4>
  <br>
  <p>Set value of null to variable.
  <br>
  if variable is string, new value is ''. (nothing)
  <br>
  if variable is int, new value is 0.
  <br>
  if variable is double, new value is 0.0.
  <br>
  <br>
  <pre>
  <i>
  DEF:int:number=1;
  <br>
  &lt&lt$:int:number_;
  <br>
  NULL $:int:number_;
  <br>
  &lt&lt$:int:number_;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>1<br>0</i></pre>
  </p>
</li>

<br>

<li>
  <h4>FREE</h4>
  <br>
  <p>Delete the variable and set value of memory to null.
  <br>
  <br>
  <pre>
  <i>
  DEF:int:myAwasomeNumber=5;
  <br>
  &lt&lt$:int:myAwesomeNumber_;
  <br>
  FREE $:int:myAwesomeNumber_;
  <br>
  &lt&lt$:int:myAwesomeNumber_;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>5<br>ERROR</i></pre>
  </p>
</li>

<br>

<li>
  <h4>INDEX</h4>
  <br>
  <p>Copy n'th index of first parameter and set value to second parameter. Just using with strings
  <br>
  <br>
  <pre>
  <i>
  DEF:str:name=hasanbly;
  <br>
  DEF:str:myIndex=nullForNow;
  <br>
  &lt&lt$:str:myIndex_;
  <br>
  INDEX[2] $:str:name_ $:str:myIndex;
  <br>
  &lt&lt$:str:myIndex_;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>nullForNow<br>s</i></pre>
  </p>
</li>

<br>

<li>
  <h4>SLEEP</h4>
  <br>
  <p>Sleep main process. Just one parameter and it can be pure integer number or integer variable. Time type is millisecond.
  <br>
  <br>
  <pre>
  <i>
  DEF:str:name=hasanbly;
  <br>
  SLEEP 1000;
  <br>
  // sleeping 1000 millisecond (1 second)
  <br>
  &lt&lt$:str:name_;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>hasanbly</i></pre>
  </p>
</li>

<br>

<li>
  <h4>RAND</h4>
  <br>
  <p>Random integer number generator. Have to using integer variables. 3 parameter (destination, min, max)
  <br>
  <br>
  <pre>
  <i>
  DEF:int:dest=0;
  <br>
  DEF:int:min=3;
  <br>
  DEF:int:max=8;
  <br>
  // random int number between 3-8 (3,4,5,6,7,8)
  <br>
  RAND $:int:dest_ $:int:min_ $:int:max_;
  <br>
  &lt&lt$:int:dest_;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>6</i></pre>
  </p>
</li>

<br>

<li>
  <h4>STRLEN</h4>
  <br>
  <p>Returns string's length. Have to using 2 (str, int) variables. 2 parameters are (source, destination)
  <br>
  <br>
  <pre>
  <i>
  DEF:str:text=hello;
  <br>
  DEF:int:length=0;
  <br>
  STRLEN $:str:text_ $:int:length_;
  <br>
  &lt&lt$:int:length_;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>5</i></pre>
  </p>
</li>

<br>

<li>
  <h4>CLEAR</h4>
  <br>
  <p>Takes no parameters and clears the console.
  <br>
  <br>
  <pre>
  <i>
  &lt&lt^$^$Hello World!^$^$;
  <br>
  CLEAR;
  <br>
  &lt&lt^$^$Hello World!^$^$;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>$$Hello World!$$</i></pre>
  </p>
</li>

<br>

<li>
  <h4>OSNAME</h4>
  <br>
  <p>Assigns your current OS name to a string variable.
  <br>
  <br>
  <pre>
  <i>
  DEF:str:myOSname=;
  <br>
  OSNAME $:str:myOSname_;
  <br>
  &lt&lt$:str:myOSname_;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>Windows</i></pre>
  </p>
</li>

<br>

<li>
  <h4>REPLACE</h4>
  <br>
  <p>Copy all of second parameter and change n'th index of first parameter. Just using with strings
  <br>
  <br>
  <pre>
  <i>
  DEF:str:name=hasanbly;
  <br>
  DEF:str:newWord=HAHAHA;
  <br>
  &lt&lt$:str:name_;
  <br>
  REPLACE[3] $:str:name_ $:str:newWord_;
  <br>
  &lt&lt$:str:name_;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>hasanbly<br>hasHAHAHAnbly</i></pre>
  </p>
</li>

<br>

<li>
  <h4>TERMINAL</h4>
  <br>
  <p>Runs bash and batch codes. Just one parameter and it can be string variable. But be carefull because you may get some errors and program will stop if you write wrong command.
  <br>
  <br>
  <pre>
  <i>
  // example for Windows
  <br>
  DEF:str:myAwesomeCommand=dir;
  <br>
  // example for UNIX types
  <br>
  // DEF:str:myAwesomeCommand=ls;
  <br>
  TERMINAL $:str:myAwesomeCommand_;
  <br>
  EXIT;
  </i>
  </pre>
  <br>
  <br>
  <pre>output: <i>[&lt your current folder contents &gt]</i></pre>
  </p>
</li>

</ul>

<br>
<br>
<br>

<h1>UNDER DEVOLOPMENT</h1>
