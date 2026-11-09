(ch_introductie)=
# Introductie

Deze repository bevat een collectie Notebooks waarmee je aan de slag gaat in Q2. Samen met je partner werk je aan de verschillende simulaties waardoor we hopen dat je thermodynamische processen beter gaat beschrijven.

We hebben een automatische checker gebouwd. Die controleert of de codes die in de `main` repository staan goed zijn. Je kunt dit zelf nagaan door in de Appendix te kijken bij `Notebook checks`.

## Stappen
Grofweg doorloop je de volgende stappen:

1. Het gebruik maken (kopieren) van de template door een van de twee partners.
2. Het activeren van GitHub pages.
3. Het beschermen van de main brnach.
4. Het uitnodigen van de partner
5. De repository lokaal beschikbaar maken.
6. Lokaal een nieuwe branch aan maken, aan de slag.

## Markdown files as Notebooks
In deze repository vind je gek genoeg geen Jupter Notebook (.ipynb) bestanden, maar alleen markdown (.md) bestanden. Wat Jupyter heel handig heeft gedaan (Thank you Marc Wouts!!) is een conversie mogelijk maken tussen die bestanden (Jupytext). Jupyter Notebooks bevatten enorm veel metadata en die veranderen steeds als je het Notebook runt. Daardoor is lastig bij te houden wat de wijzigingen zijn die doorgevoerd zijn. Markdown is platte tekst en daardoor makkelijk te lezen.

Als je Jupyter Lab opent (`pixi run editor`), kun je met de rechtermuisknop de markdown file openen als Jupytext Notebook. Dit runt de markdown file alsof het een Jupyter Notebook is.


## Workflow
We willen samen werken aan hetzelfde project, en toch voorkomen dat we ofwel hetzelfde doen, ofwel conflicterende oplossingen voor eenzelfde probleem bedenken. We raden de volgende workflow aan:

1. Lokaal maken beide partners een eigen branch aan gekoppeld aan de Notebook waar aan gewerkt wordt. Bijvoorbeeld: `NB1_Roel` en `NB1_Freek`.
2. Je kunt spieken hoe de ander de problemen oplost, zie de tip hieronder, maar je werkt altijd ALLEEN IN JE EIGEN BRANCH.
3. Ben je klaar met een Notebook? Dan moet je beslissen welke Notebook het best is en toegevoegd wordt aan main. 
4. De beste branch doet een merge request naar main en de ander reviewt deze en klikt op approve.
5. Beide partners gaan naar main, doen een pull. En creeeren een nieuwe branch waarop verder gewerkt wordt, nl `NB2_Roel` en `NB2_Freek`.

```{tip}
:label: tip_checkout

Wil je zien wat de ander doet, dan moet je de branch die online staat lokaal beschikbaar maken. `git branch -r` geeft een overzicht welke branches er remote allemaal bestaan. Nu wil Freek de Notebook van Roel in zien. Dit kan via `git checkout -b NB1_Roel origin/NB1_Roel`. Nu is de branch van Roel ook lokaal beschikbaar voor Freek. Roel kan wel regelmatig werken aan de Notebook. Freek moet, als hij wederom wil kijken of Roel wel aan het werk is, `git pull` doen om de meest recente versie lokaal te krijgen.
```



## GitHub & Pixi
In het dictaat van [Python for physicists](https://contemporary-physicslab.github.io/FYPLC/) hebben we hoofdstukken toegevoegd over het werken met Git en Pixi. Zeker de hoofdstukken over Git dienen gelezen te worden om goed te kunnen werken in deze periode.

## Mystmd preview
Je kunt je gehele werk zien door de volgende code te runnen `pixi run preview`.


## Run editor
De editor kun je openen door de volgende code te runnen: `pixi run editor`.