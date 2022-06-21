//Voriconazole PBPK model for a typical adult male
[CMT]

GUTLUMEN 
GUT 
ADIPOSE 
BRAIN 
HEART 
BONE 
KIDNEY 
LIVER 
LUNG 
MUSCLE 
SPLEEN 
REST 
ART 
VEN
SKIN
URINE 
AUC // these are dummy variables for model validation. 


[PARAM] 
//Tissue volumes (L); source: https://www.ncbi.nlm.nih.gov/pubmed/14506981
Vad      = 18.2   //adipose
Vbo      = 10.5   //bone
Vbr      = 1.45   //brain
VguWall  = 0.65   //gut wall
VguLumen = 0.35   //gut lumen
Vhe      = 0.33   //heart
Vki      = 0.31   //kidneys
Vli      = 1.8    //liver
Vlu      = 0.5    //lungs
Vmu      = 29     //muscle
Vsp      = 0.15   //spleen
Vbl      = 5.6    //blood
Vsk      = 3.3    // skin
Vss 	   = 0.99   // (L/kg)

//Tissue blood flows (L/h); Cardiac output = 6.5 (L/min); source: https://www.ncbi.nlm.nih.gov/pubmed/14506981
Qad = 0.05*6.5*60
Qbo = 0.05*6.5*60
Qbr = 0.12*6.5*60
Qgu = 0.15*6.5*60 
Qhe = 0.04*6.5*60
Qki = 0.19*6.5*60
Qmu = 0.17*6.5*60
Qsp = 0.03*6.5*60
Qha = 0.065*6.5*60  //hepatic artery
Qlu = 6.5*60        //same as cardiac output
Qsk = 0.05*6.5*60  // skin

//partition coefficients
Kpad = 0.191  //adipose:plasma
Kpbo = 0.466 // bone:plasma
Kpbr = 0.856  //brain:plasma
Kpgu = 0.771  //gut:plasma
Kphe = 0.813  //heart:plasma
Kpki = 0.84   //kidney:plasma
Kpli = 0.812  //liver:plasma
Kplu = 0.863  //lungs:plasma
Kpmu = 0.809  //muscle:plasma; optimized
Kpsp = 0.848  //spleen:plasma
Kpsk = 0.767  //skin: plasma
BP = 1       //blood:plasma ratio; 

//other parameters
WEIGHT  = 73          //(kg)
fup     = 1         //fraction of unbound drug in plasma; https://www.ncbi.nlm.nih.gov/pmc/articles/PMC8299544/
ka      = 0.27       //absorption rate constant(/hr) 
fa 		= 1		// fraction that is available for absorption
GFR 	= 6 		// glomerular filtration rate (L/hr)
renal_secretion = 2 // Dunn, 1999; https://pubmed.ncbi.nlm.nih.gov/10400410/


// dummy clearance parameter to scale up secretion
renal_scaling = 1
hepatic_scaling = 1


$MAIN
// parition coefficient of the rest of the tissue; average of all non-adipose tissue Kps
double Kpre = ( Kpbo + Kpbr + Kpgu + Kphe + Kpki + Kpli + Kplu + Kpmu + Kpsp + Kpsk )/10; 


//additional volume derivations
double Vve = 0.705 * Vbl;         //venous blood
double Var = 0.295 * Vbl;         //arterial blood
double Vre = Vss * WEIGHT - 
  (Vli + Vki + Vsp + Vhe + Vlu + Vbo + Vbr + Vmu + Vad + VguWall + Vsk + Vbl); //volume of rest of the body compartment

//additional blood flow derivation
double Qli  = Qgu + Qsp + Qha;
double Qtot = Qli + Qki + Qbo + Qhe + Qmu + Qad + Qbr + Qsk;
double Qre  = Qlu - Qtot;

//renal clearance
// double CLrenal = CL_pd_renal * PTC_num * 60 / 1000 
//				+ OCT2_renal_CLint * OCT2_renal_RAF * PTC_num * 60/1000000 
//				+ MATE_renal_CLint * MATE_renal_RAF * PTC_num * 60/1000000; //(L/hr)
// CLrenal = f_cl * CLrenal;

double CLrenal = (GFR + renal_secretion) * renal_secretion_scaling; //(L/hr) 

//intrinsic hepatic clearance calculation
// double CLintHep = CL_pd_liver * hepatocytes_per_liver * 60/ 1000 
//				+ OCT1_hepatic_CLint * OCT1_hepatic_RAF * hepatocytes_per_liver * 60/1000000 
//				+ MPPGL * HLM_CLint * liver_mass * 60/1000000; //(L/hr)

double CLintHep = 6 * hepatic_secretion_scaling; //(L/hr) 


$ODE
//Calculation of tissue drug concentrations (mg/L)
double Cadipose  = ADIPOSE / Vad;
double Cbone     = BONE / Vbo;
double Cbrain    = BRAIN / Vbr; 
double Cheart    = HEART / Vhe; 
double Ckidney   = KIDNEY / Vki;
double Cliver    = LIVER / Vli; 
double Clung     = LUNG / Vlu; 
double Cmuscle   = MUSCLE / Vmu;
double Cspleen   = SPLEEN / Vsp;
double Crest     = REST / Vre;
double Carterial = ART / Var;
double Cvenous   = VEN / Vve;
double CgutLumen = GUTLUMEN / VguLumen;
double Cgut      = GUT / VguWall;
double Cskin     = SKIN / Vsk; 

//Free Concentration Calculations
double Cliverfree  = fup * Cliver; 
double Ckidneyfree = fup * Ckidney;


//ODEs

dxdt_GUTLUMEN = -ka * fa * GUTLUMEN;

dxdt_GUT =  Qgu*(Carterial - Cgut/(Kpgu/BP)) + ka * fa * GUTLUMEN;

dxdt_ADIPOSE  = Qad * (Carterial - Cadipose / (Kpad / BP)); 

dxdt_BRAIN    = Qbr * (Carterial - Cbrain / (Kpbr / BP));

dxdt_HEART    = Qhe * (Carterial - Cheart / (Kphe / BP));

dxdt_KIDNEY   = Qki * (Carterial - Ckidney / (Kpki / BP))
				- CLrenal * (Ckidneyfree / (Kpki / BP));

dxdt_LIVER    = Qgu * (Cgut / (Kpgu / BP)) 
				+ Qsp * (Cspleen / (Kpsp / BP)) 
				+ Qha * (Carterial)
				- Qli * (Cliver / (Kpli / BP))
				- CLintHep * (Cliverfree / (Kpli / BP)); 

dxdt_LUNG     = Qlu * (Cvenous - Clung / (Kplu / BP));

dxdt_MUSCLE   = Qmu * (Carterial - Cmuscle / (Kpmu / BP));

dxdt_SPLEEN   = Qsp * (Carterial - Cspleen / (Kpsp / BP));

dxdt_BONE     = Qbo * (Carterial - Cbone / (Kpbo / BP));

dxdt_SKIN     = Qsk * (Carterial - Cskin / (Kpsk / BP));

dxdt_REST     = Qre * (Carterial - Crest / (Kpre / BP));

dxdt_VEN      = Qad * (Cadipose / (Kpad / BP)) + 
  Qbr * (Cbrain / (Kpbr / BP)) +
  Qhe * (Cheart / (Kphe / BP)) + 
  Qki * (Ckidney / (Kpki / BP)) + 
  Qli * (Cliver / (Kpli / BP)) + 
  Qmu * (Cmuscle / (Kpmu / BP)) + 
  Qbo * (Cbone / (Kpbo / BP)) + 
  Qsk * (Cskin / (Kpsk / BP)) + 
  Qre * (Crest / (Kpre / BP)) - 
  Qlu * Cvenous;

dxdt_ART      = Qlu * (Clung / (Kplu / BP) - Carterial);

// calculate for the dummy variables
dxdt_AUC = Cvenous/BP; 

dxdt_URINE = CLrenal * (Ckidneyfree / (Kpki / BP)); 


$CAPTURE
Cadipose, Cbone, Cbrain, Cheart, Ckidney, Cliver, Clung, Cmuscle, Cspleen, Crest, Carterial, Cvenous, CgutLumen, Cgut, Cskin