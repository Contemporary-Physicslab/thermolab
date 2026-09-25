---
jupytext:
  formats: ipynb,md:myst
  text_representation:
    extension: .md
    format_name: myst
    format_version: 0.13
    jupytext_version: 1.19.5


---

# Cappucino

## Introductie
Heb je al eens bedacht dat het verwarmen van melk voor een cappucino eigenlijk heel snel gaat zonder dat de melk verdunt wordt?
De melk wordt ook niet echt plaatselijk verhit door een warmte element.
In plaats daarvan wordt er stoom door de melk geblazen.
De stoom condenseert in de melk en geeft daarbij zijn latente warmte af.
Hierdoor warmt de melk snel op zonder dat er (significant veel) water aan toegevoegd wordt.

```{warning}
Stoom is heel heet en kan voor vervelende brandplekken zorgen.
Wees dus voorzichtig bij het uitvoeren van dit practicum.

Daarnaast, nadat de kookplaat uitgaat is er een onderdruk in de kolf.
Hang dan ook de tube uit het water wanneer je de kookplaat uitzet, anders kan de kolf imploderen.
```

In dit practicum gaan we de verdampingswarmte van water bepalen door middel van een zelfgebouwde cappucino machine.
Het principe is als volgt: We gebruiken de opstelling weergegeven in {numref}`Figuur {number} <fig_cappucino>` waarbij we water verwarmen in een kolf met behulp van een kookplaat.
Door de warmte gaat het water koken en ontstaat er stoom.
De stoom stroomt via een tube naar een afgesloten maatbeker met water.
De stoom condenseert in het maatbeker en geeft daarbij zijn latente warmte af.
Door de temperatuurstijging van het water te meten, evenals de hoeveelheid gram water dat verdampt is, kunnen we de verdampingswarmte van water bepalen.

```{figure} figures/verdampingswarmte_setup.png
:width: 70%
:label: fig_cappucino

Een schematische weergave van de cappucino opstelling.
```

## Theorie

De latente verdampingswarmte van water bedraagt 2257 kJ/kg, dit is veel meer dan de specifieke warmtecapaciteit van water (4.18 kJ/kgK).
Wanneer we de waterdamp (g) door koud water leiden, gaan we er van uit dat de waterdamp condenseert en daarbij zijn latente warmte afgeeft aan het koude water.
Door te bepalen hoeveel gram water verdampt is en hoeveel de temperatuur van het koude water stijgt, kunnen we de latente warmte van verdamping bepalen:

$$ Q_{condensatie} = m_{damp} L = m_{water} c \Delta T $$

met $m_{damp} = \Delta m_{kolf}$.

## Methoden en materialen

### Materialen
- Warmteplaat
- Kolf met stop en tube
- Maatcilinder
- Thermometer
- Weegschaal
- Water


### Procedure

- Vul de kolf met ongeveer 100 mL water: bepaal precies de massa water ($m_{w_1}$).  
- Vul de maatcilinder met ongeveer 100 mL water: bepaal precies de massa water ($m_{bad}$).
- Bepaal de temperatuur van dit waterbad ($T_{bad_1}$).
- Zet de kolf op de warmteplaat en zet de warmteplaat aan - maximale stand 3, zorg ervoor dat de tube goed in het waterbad hangt.
- Wacht tot de temperatuur van het waterbad met ongeveer 20 K is gestegen.
- Stop de meting / zet de warmteplaat uit. Noteer meteen de temperatuur($T_{bad_2}$) en haal de tube uit het water! 

```{warning}
Pas op stoom is heet!
```

- Bepaal precies de massa water in de kolf ($m_{w_2}$).
- Verwerk je resultaten hieronder om de latente warmte van verdamping van water te bepalen.

+++

## Resultaten

```{code-cell}
m_w_1 =       # kg
m_bad =       # kg
T_bad_1 =     # K


m_w_2 =       # kg
T_bad_2 =     # K
```

## Discussie en conclusie
