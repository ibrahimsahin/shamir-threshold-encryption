1.  It  will calculate  the  main key for a given party  and crypt a plain text with main key.  This 
function will run with parameter “–e”
2.  It will find sub-polynomials of  parties and will write them to text file. This function will 
run with parameter “-l”

In encryption  process the program will take  three  argument text  file  names:  [keys_file], 
[plaintext_file]  and  [cipher_text_file].  The  program  will  be  run  from  command  line  for 
encryption mode in format below:
shamir   -e [keys_file] [plaintext_file] [cipher_text_file]
The party keys file will  include  party‟s (x, f(x))  couples. The  x  value will be party‟s id 
and  f(x)  value will be the party‟s key. So every line  in this text file  will consist a party‟s id and 
party‟s key in “party  id:  party  key” format.  For given example the party  list text file will be like 
this: 
1 : 1494
2 : 1942
3 : 2578
4 : 3402
5 : 4414
6 : 5614
The party key file will include just enough point count to calculate the polynomial. If the 
file includes six points, our polynomial will be 5
th
degree. For A (n, t) threshold, the text file will 
include  t  parties.  Then the  program will calculate I
i
(sub-polynomial)  for each group  as given in 
example, and the main key using 
Ei=0 to t Ii 
formula.