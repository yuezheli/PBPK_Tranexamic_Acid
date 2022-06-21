# Physiological based pharmacokinetic modeling and simulation for the distribution of tranexamic acid in human

## Goal

The goal of this Physiological based pharmacokinetic (PBPK) modeling and simulation exercise is to understand the factor that impact tranexamic acid (TXA) distribution in human. In particular, the distribution of TXA distribuition in skin after oral intake of the drug. I would like to know more under the dosing regimen of Transino II (by Daiichi Sankyo Healthcare, which aims to treat melasma), how TXA would distribute and the exposure comparison between different organs

Note this is purelu a modeling and simulation (M&S) exercise. The result should be taken with a healthy dose of skeptism, and should not be used for medical advice. 

## Set up

The simulation is run with Rstudio (version) and the following packages (version) ....

- tidyverse
- mrgsolve
- mrggsave
...

## Model Development

The PBPK model is borrowed from [Utsey et al., 2020](https://dmd.aspetjournals.org/content/48/10/903) with an additional compartment added to model TXA absoption with first-order kinetics. 

The base model is built with following assumptions: 


In addition, there are 2 parameters unknown: 

- blood:plasma ratio
- TXA adsoption rate in the gut lumen

To tune these 2 parameters, we use the observed ata from from [Pilbrant et al., 1981](https://pubmed.ncbi.nlm.nih.gov/7308275/) with both IV and oral observed data from 3 healthy male wolunteers. I conclude the (parameters) provide a reasonable fit, and the pharmacokinetics is overall inline with what is described in [Dunn 1999](https://pubmed.ncbi.nlm.nih.gov/10400410/) and I will be using them for the following simulation. 

(add pic)


## Oral dosing from Transino II

The recommended oral dosing of Transino II roughlt translated into 1.5g of TXA per 12 hour. 

## 


## Sensitivity Analysis

### Blood:plasma ratio

### Absorption rate

### Dosing amount

### Body weight

### Renal Impairment

## Conclusion and Discussion

Overall, the plasma concentraion following the Transino II recommended dosing scheme would lead to safe plasma TXA concentration with no apparent risk of clotting ...


We acknowledge the science behind our work isn't new, as [Kane et al., 2021](https://pubmed.ncbi.nlm.nih.gov/34087356/) published a similar study on TXA with more extensive discussion on different route of administration. But I concede this repo provides an open-source implementation is is beneficial for the science. 

## Content of the folder

folders: 

- doc (where reference papers are stored)
