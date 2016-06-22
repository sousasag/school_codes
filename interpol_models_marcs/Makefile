FF77=gfortran
CPP=g++

hellomake:
	cat interpol_marcs_moog_base.cpp | sed s@"PATHIN"@"$(CURDIR)"@g > interpol_marcs_moog.cpp
	cat interpol_template_base.com | sed s@"PATHIN"@"$(CURDIR)"@g > interpol_template.com
	$(FF77) -o interpol_modeles interpol_modeles_mine.f
	$(CPP) -o interpol_marcs interpol_marcs_moog.cpp
	cat make_model_marcs_base.bash | sed s@"path"@"$(CURDIR)"@g > make_model_marcs.bash
	chmod 777 make_model_marcs.bash

clean:
	rm -f interpol_modeles interpol_marcs make_model_marcs.bash
