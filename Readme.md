#Hamilton Kreis 

###Spezifikation 
Der Hamiltion Kreis ist ein mathematisches Graphenproblem. Der Hamiltonkreis ist ein geschlossener Pfad in einem Graph, bei dem jeder Knoten nur einmal besucht wird. 
Es wird ein Programm entwickelt, welches einen Graph im DIMACS Format einliest. Diese Problematik in eine aussagenlogische Formel umwandelt und dann die Erfüllbarkeit dieser Formel mit dem MiniSAT Solver errechnet. 

---

    

###Annahmen 
    - Jeder Knoten wird nur einmal besucht 
    - Jede Kante wird nur einmal benutzt 
    - Startknoten ist gleich Endknoten (Kreis) 


---
###Knoten 
    - Zusammgesetzer String aus time + id 
    
###Kante 
    - Implikation  time+id ->time+1+id 

###DIMACS FORMAT
    - "c" Kommentar 
    - "p Kanten Knoten" Spezifikation des Problems
    - "e Knoten Knoten" Spezifkikation einer Kante 

---

###To Do's
    -Namensgebung eines Knoten
    -Namensgebung einer Kante 
    -Einlesen der Textfile 
    -Generierung der Formel 
    -Minisat öffnen
    -MiniSat starten
    -MiniSat auslesen 

###References 
    -Visual Studio 2012 
    -GitHub
    -MiniSat
    
    