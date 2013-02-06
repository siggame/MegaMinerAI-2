rem This will create the directory structure for an update and copy over only files we will want to update
xcopy ..\client deploy-update\client /S /D /I /EXCLUDE:update-excludes.txt
xcopy ..\visualizer deploy-update\visualizer /S /D /I
xcopy ..\client\muparser\makefile deploy-update\client\muparser\makefile /D
xcopy ..\client\sexp\makefile deploy-update\client\sexp\makefile /D