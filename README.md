# GenTruthTable
Truth table generator written in C++.

## Install
### Dependency: OptionParser
1. `git clone https://github.com/darshanparajuli/OptionParser.git`
2. `cd OptionParser`
3. `make release`
4. `make install`
### GenTruthTable
1. `git clone https://github.com/darshanparajuli/GenTruthTable`
2. `cd GenTruthTable`
3. `make release`
4. `make install`

## Uninstall
### GenTruthTable
`cd GenTruthTable && make uninstall`
### OptionParser
`cd OptionParser && make uninstall`

## Usage
~~~~
$ GenTruthTable -p "a + b"
+--+--+---+
| a| b|a+b|
+--+--+---+
| 0| 0|  0|
| 0| 1|  1|
| 1| 0|  1|
| 1| 1|  1|
+--+--+---+
~~~~
~~~~
$ GenTruthTable
***************************************************
*                                                 *
*  Truth Table Generator by Darshan Parajuli      *
*                                                 *
*  Syntax:                                        *
*  NOT           => '~'                           *
*  AND           => '*'                           *
*  Or            => '+'                           *
*  XOR           => '<+>'                         *
*  IMPLICATION   => '=>'                          *
*  EQUIVALENCE   => '<>'                          *
*  Tautology     => '1'                           *
*  Contradiction => '0'                           *
*                                                 *
*  Operand names can be [a-zA-Z]+ only.           *
*  Enter 'quit' or 'exit' to exit.                *
*                                                 *
*  Example: (a + b)<+>(~c*d)                      *
*                                                 *
***************************************************

Logical expression >> a <+> b
+--+--+-----+
| a| b|a<+>b|
+--+--+-----+
| 0| 0|    0|
| 0| 1|    1|
| 1| 0|    1|
| 1| 1|    0|
+--+--+-----+

Logical expression >> quit
Bye!
~~~~

## License
MIT License

Copyright (c) 2017 Darshan Parajuli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
