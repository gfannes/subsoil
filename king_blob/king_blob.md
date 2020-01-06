\pagenumbering{gobble}

![](king_blob.jpg){width=10%} ![](king_blob.jpg){width=10%} ![](king_blob.jpg){width=10%} ![](king_blob.jpg){width=10%} ![](king_blob.jpg){width=10%} ![](king_blob.jpg){width=10%} ![](king_blob.jpg){width=10%} ![](king_blob.jpg){width=10%} ![](king_blob.jpg){width=10%}
=========

## Definities

* Elke _speler_ heeft een _naam_, een _rang_, een _killcount_ en kan _koning_ zijn
* Een _koninkrijk_ is een verzameling van spelers
* De _naam_ van een koninkrijk is de namen van zijn koningen
* Een _blob_ is een deelverzameling van een koninkrijk
* Een _koord_ heeft een vastgesteld maximale lengte

## Basis regels

* De eerste speler van een nieuw koninkrijk is de koning
* De koning(en) van een koninkrijk beslis(sen) over de opsplitsing van het koninkrijk in blobs (zowel opsplitsen en samensmelten)
* Als 2 blobs samensmelten, wordt(en) de speler(s) met laagste rang koning
* Als de koning(en) een blob wijzigen, worden de killcounts van de respectievelijke spelers op 0 gezet
* Elke beslissing van de koning(en) moet unaniem zijn
* Alle spelers van een blob zijn steeds verbonden met elkaar via een koord
* Een blob _zonder_ koning is verbonden met minstens 1 boom via een koord
* Een speler kan enkel een speler van een _ander_ koninkrijk doden
* Als speler _a_ uit blob _A_ gedood wordt door speler _b_ uit blob _B_, verplaatst _a_ van _A_ naar _B_
* Als een speler tot een nieuw koninkrijk toetreedt, wordt zijn rang ingesteld op de kardinaliteit van de _blob_ 
* Als de koning sterft, wordt(en) de speler(s) met laagste rang _van die blob_ koning

## Start van het spel

* Een spel met _N_ spelers start met _N_ eenmans-koninkrijken, elk met rang 0

## Einde van het spel

* Het spel stopt als er nog maar 1 koninkrijk is of het eindsignaal weerklinkt
* De eindstand wordt bepaald door de rang: less is more

## Extra regel

* Als een speler zijn killcount op 2 staat en geen koning is, start hij een nieuw eenmans-koninkrijk

