#Hamilton Kreis 

###Spezifikation 
Der Hamiltion Kreis ist ein mathematisches Graphenproblem. Der Hamiltonkreis ist ein geschlossener Pfad in einem Graph, bei dem jeder Knoten nur einmal besucht wird. 
Es wird ein Programm entwickelt, welches einen Graph im DIMACS Format einliest. Diese Problematik in eine aussagenlogische Formel umwandelt und dann die Erf�llbarkeit dieser Formel mit dem MiniSAT Solver errechnet. 

---

###To Do's
    -Namensgebung eines Knoten
    -Namensgebung einer Kante 
    -Einlesen der Textfile 
    -Generierung der Formel 
    -Minisat �ffnen
    -MiniSat starten
    -MiniSat auslesen 
    

###Annahmen 
    - Jeder Knoten wird nur einmal besucht 
    - Jede Kante wird nur einmal benutzt 
    - Startknoten ist gleich Endknoten (Kreis) 


---

###DIMACS FORMAT
    - "c" Kommentar 
    - "p Kanten Knoten" Spezifikation des Problems
    - "e Knoten Knoten" Spezifkikation einer Kante 

---


###References 
    -Visual Studio 2012 
    -GitHub
    -MiniSat
    
    