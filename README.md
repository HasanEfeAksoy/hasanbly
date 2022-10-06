# hasanbly
hasanbly basic scripting language
<br>
<br>
<b>v3</b>
<br>
<br>
if you want devoloping this project you can compile like this:
<br>
<b><i>g++ main.cpp -o hsnc.exe -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread</i></b>

<br>
<h1>Get Started</h1>
<br>
Download hsnc.exe file and create your .hsn file (ex: main.hsn) and run with <b><i>hsnc main.hsn</i></b> command.
<br>
<p>Hasanbly is open source, non memory safe (for now) and non parser basic scripting language created with C++.
<br>
It is just <i>interprete</i> and run your codes easily.</p>

<br>
<br>

<h1><b>Usage<b></h1>
<br>
<h3>Hello, World!</h3>
<br>
<p>Firstly, maybe you want writing Hello, World! in console 🙃</p>
<br>
<p><i>
&lt&ltHello, World!;
<br>
EXIT;
</i></p>

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
  <i>
  // This is comment line and
  <br>
  // this lines will remove at pre-interpretation time.
  <br>
  EXIT;
  </i>
  <br>
  <br>
  output: <i></i>
  </p>
</li>

<br>

<li>
  <h4>DEF</h4>
  <br>
  <p>Create a new variable and assign the entered value to the value of the variable. You have to define int or string (int-str). To use the variable, you must know the order of the variable.
  <br>
  <br>
  <i>
  DEF:str empty;
  <br>
  &lt&lt$:str0_;
  <br>
  EXIT;
  </i>
  <br>
  <br>
  output: <i>empty</i>
  </p>
</li>

<br>

<li>
  <h4>&lt</h4>
  <br>
  <p>Prints the written values to the console.
  <br>
  <br>
  <i>
  &ltHello;
  <br>
  EXIT;
  </i>
  <br>
  <br>
  output: <i>Hello</i>
  </p>
</li>

<br>

<li>
  <h4>&lt&lt</h4>
  <br>
  <p>Prints the written values to the console with next line.
  <br>
  <br>
  <i>
  &lt&ltHello;
  <br>
  EXIT;
  </i>
  <br>
  <br>
  output: <i>Hello</i>
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
  <i>
  DEF:str noname;
  <br>
  // entered 'hasanbly'
  <br>
  &gt$:str0_;
  <br>
  &lt&lt$:str0_;
  <br>
  EXIT;
  </i>
  <br>
  <br>
  output: <i>hasanbly</i>
  </p>
</li>

<br>

<li>
  <h4>M</h4>
  <br>
  <p>You can using math operators like = + - * / %. But you have to using variables. You can convert int to string or string to int with use M command.
  <br>
  You can string += string, string += int, int += string, int += int and <br> string = string, string = int, int = string, int = int
  <br>
  <br>
  <i>
  DEF:int 0;
  <br>
  DEF:int 1;
  <br>  
  // $:int0_ = $:int1_
  <br>
  M= $:int0_ $:int1_;
  <br>
  &lt&lt$:int0_;
  <br>
  EXIT;
  </i>
  <br>
  <br>
  output: <i>1</i>
  </p>
</li>

<br>

<li>
  <h4>GOTO</h4>
  <br>
  <p>Going to line. You can using with line number or line number variable value.
  <br>
  <br>
  <i>
  &ltHi; 
  <br>
  GOTO 1;
  <br>
  <br>
  // or
  <br>
  <br>
  //DEF:int 2;
  <br>
  //&ltHi;
  <br>
  //GOTO $:int0_;
  <br>
  EXIT;
  </i>
  <br>
  <br>
  output: <i>HiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHiHi........</i>
  </p>
</li>

<br>

<li>
  <h4>EXIT</h4>
  <br>
  <p>Ends the program. At the end of the program, you have to put EXIT command otherwise the program does not run.
  <br>
  <br>
  <i>
  &ltBye;
  <br>
  EXIT;
  <br>
  &ltBye;
  <br>
  EXIT;
  </i>
  <br>
  <br>
  output: <i>Bye</i>
  </p>
</li>

</ul>

<br>
<br>
<br>

<h1>UNDER DEVOLOPMENT</h1>
