#!/bin/csh -f
##################################################################################################
# Output turbospectrum/babsma format compatible 
# Output 2 for MOOG (KURUCZ TYPE)
# A control plot (interpol_check.ps) is displayed at the end.
# Extrapolation is not advised, even if allowed by this program.
# Requires an "cubic" set of 8 MARCS binary format models,
# in other words 
# !!!!!   MODELS MUST DIFFER 2 BY 2 BY ONLY ONE PARAMETER !!!!!!  
# !!!!!!! ORDER OF THE INPUT MODELS MATTERS !!!!!!!
# here is the order of the files
# model1: Tefflow logglow zlow
# model2: Tefflow logglow zup
# model3: Tefflow loggup zlow
# model4: Tefflow loggup zup
# model5: Teffup logglow zlow
# model6: Teffup logglow zup
# model7: Teffup loggup zlow
# model8: Teffup loggup zup
######################################################################################################

#set model_path = Testwebformat
#set model_path = /u16/marcs

#CHANGE ONLY THE PATH to the directory. Leave the final directory to "models_speric"
#Examples:
#set model_path = /home/sousasag/MARCS/models_spheric
#set model_path = /usr/local/MARCS_MODELS/models_spheric
set model_path = PATHIN/marcs_models/models_spheric


#MARCS binary format (.true.) or MARCS ASCII web format (.false.)?
set marcs_binary = '.false.'
#set marcs_binary = '.true.'

set model1 =
set model2 =
set model3 =
set model4 =
set model5 = 
set model6 = 
set model7 = 
set model8 = 

#enter here the values requested for the interpolated model 
foreach Tref ()
foreach loggref ()
foreach zref ()
set modele_out = model1.interpol
set modele_out2 = model2.moog
#set modele_out = scratch

#### the test option is set to .true. if you want to plot comparison model (model_test)
set test = '.false.'
set model_test = 'Testwebformat/p5777_g+4.4_m0.0_t01_ap_z+0.00_a+0.00_c+0.00_n+0.00_o+0.00_r+0.00_s+0.00.mod'

# interpolation program (for further details see interpol_modeles.f)
# include the location of the interpolation code:
PATHIN/./interpol_modeles <<EOF
'${model_path}/${model1}'
'${model_path}/${model2}'
'${model_path}/${model3}'
'${model_path}/${model4}'
'${model_path}/${model5}'
'${model_path}/${model6}'
'${model_path}/${model7}'
'${model_path}/${model8}'
'${modele_out}'
'${modele_out2}'
${Tref}
${loggref}
${zref}
${test}
${marcs_binary}
'${model_test}'
EOF

