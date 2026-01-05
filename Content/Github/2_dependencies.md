---
downloads:
  - file: ./requirements.txt
    title: Software dependencies
authors: Freek Pols
updated: december 3, 2025
--- 

# Installeren extra software

Om de oefeningen in deze cursus te kunnen maken heb je extra software nodig.
Dit zijn voornamelijk Python pakketten.
Deze kun je installeren met pip (de package installer for Python).

Voor windows:

`pip install -r requirements.txt`

of voor MAC:

`pip3 install -r requirements.txt`

of als de bovenstaande niet werken:

`py -m pip install -r requirements.txt`

## Jupyter lab

Tijdens het tentamen heb je Jupyter Lab gebruikt om de opdrachten te maken.
Jupyter lab is een interactieve omgeving waarin je code kunt schrijven en uitvoeren.
Daarnaast kun je er ook tekst in markdown schrijven. 

Omdat we gebruik maken van Jupyter Book, is het handig om Jupyter lab te installeren/gebruiken met de MyST extensie.
Als je bovenstaande command hebt uitgevoerd, zou je Jupyter lab met MyST ondersteuning moeten hebben.
Jupyter lab start je dan door VSC te openen en in de terminal te openen en het volgende commando uit te voeren:

```bash
jupyter lab
```

Er wordt dan een server gestart waarna je in je browser Jupyter lab kunt gebruiken.
De losse notebooks kun je dan openen en bewerken in Jupyter lab. 

Ook is het mogelijk de gehele portfolio (Jupyter Book) lokaal te bouwen en te bekijken.
Dit kan met het volgende commando in de terminal (als je in de folder staat waar het myst.yml file staat):

```bash
jupyter book start
```

Als je dat commando hebt gegeven start het boek met bouwen, waarna je een mededeling krijgt dat je het boek kunt bekijken op een lokaal adres (meestal http://localhost:3000/).

Op de volgende pagina vind je meer informatie over Jupyter Book en MyST markdown. 
